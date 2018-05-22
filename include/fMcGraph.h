/*
 * fMcGraph.h
 *
 *  Created on: Mar 8, 2012
 *      Author: bovi
 */

/*
Copyright (c) 2013 by Rosalba Giugno

FocusSearchC++ is part of the RI Project.
FocusSearchC++ is a C++ implementation of the original software developed in
Modula2 and provided by the authors of:
Ullmann JR: Bit-vector algorithms for binary constraint satisfaction and
subgraph isomorphism. J. Exp. Algorithmics 2011, 15:1.6:1.1-1.6:1.64.

FocusSearchC++ is provided under the terms of The MIT License (MIT):

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


//#include "defdebug.h"

#ifndef FMCGRAPH_H_
#define FMCGRAPH_H_

#include "gdex_domain.h"
#include "vars_ordering.h"
#include "gdex_predicate.h"
#include "match_state.h"
#include "gLists.h"
#include "gsequence.h"

#include "gpreproc.h"
#include "iso_check.h"

#include "prematch.h"

//#define DL_AREAS
//////////#ifdef DR_DOMAINS
//#define FM_DR_DOMAINS
////#endif

#define TIMERS

#ifdef TIMERS
#include "timer.h"
#endif

void mcConstraints(
		focusgraph_t& 		pattern,
		focusgraph_t& 		reference,
		o_domains_t&		o_domains,
		o_predicate_t&		o_predicates,
		o_gLists_t&			o_glists,
		o_vars_order_t&		o_varsorder,
		o_match_state_t&	o_mstate,
		o_gsequence_t&		o_sequence){
	//M2: VAR
	//M2:   consistent, terminal: BOOLEAN;
	//M2:   beforeTime, afterTime: LONGREAL;
	bool terminal = false;
	//M2: BEGIN
	//M2:   McEmpty(Dsets);	//not needed
	//M2:   mcDomains(Dsets, failed);

#ifdef TIMERS
	TIMEHANDLE start_t = start_time();
#endif

#ifdef DL_AREAS
	std::cout<<"@prepare:prematch\n";
#endif
	//o_glists.init_DLists(pattern);

#ifdef TIMERS
	//std::cout<<"init_DLists: "<<end_time(start_t)<<" sec.\n";
	o_mstate.time_dlists = end_time(start_t);
	start_t = start_time();
#endif

#ifdef FM_DR_DOMAINS
	o_domains.print_Dlists();
#endif


	//prematch(pattern, reference, o_domains);
	PREMATCH::o_prematch_t o_prematch(o_domains, pattern, reference);
	o_prematch.run();


#ifdef TIMERS
	//std::cout<<"prematch: "<<end_time(start_t)<<" sec.\n";
	o_mstate.time_prematch = end_time(start_t);
	start_t = start_time();
#endif
#ifdef FM_DR_DOMAINS
	o_domains.print_domains(pattern.nof_nodes);
	o_domains.print_Dlists();
#endif
	//M2:   IF failed THEN clearDlists; disposeValueAt; RETURN END;
	//M2:   mcMatrices;
#ifdef DL_AREAS
	std::cout<<"@prepare:mcMatrices\n";
#endif
	o_predicates.mcMatrices(pattern, reference);
#ifdef TIMERS
	//std::cout<<"mcMatrices: "<<end_time(start_t)<<" sec.\n";
	o_mstate.time_mcmatrices = end_time(start_t);
	start_t = start_time();
#endif
#ifdef DR_DOMAINS
	o_domains.print_domains(pattern.nof_nodes);
	o_domains.print_Dlists();
#endif
	//M2:   mcAdjVarInfo(eAlpha, alphaEdges);
#ifdef DL_AREAS
	std::cout<<"@prepare:mcAdjvarInfo\n";
#endif
	o_glists.init_adjVarInfo(pattern);
	o_glists.init_degree(pattern);
#ifdef FM_DR_DOMAINS
	o_glists.print_adjVarInfo();
	o_glists.print_degree();
#endif
	o_glists.mcAdjVarInfo(o_predicates, pattern.is(ordered));
#ifdef TIMERS
	//std::cout<<"mcAdjVarInfo: "<<end_time(start_t)<<" sec.\n";
	o_mstate.time_mcadjvarinfo = end_time(start_t);
	start_t = start_time();
#endif
#ifdef FM_DR_DOMAINS
	o_glists.print_adjVarInfo();
	o_glists.print_degree();
#endif

#ifdef FM_DR_DOMAINS
	o_domains.print_domains(pattern.nof_nodes);
	o_domains.print_Dlists();
#endif

	//M2:   preprocess(Dsets, Dlists, consistent);
#ifdef DL_AREAS
	std::cout<<"@prepare:preprocess\n";
#endif

	//o_mstate.ini_allowed(pattern);

	preprocess(pattern, o_domains, o_glists, o_mstate);

#ifdef TIMERS
	//std::cout<<"preprocess: "<<end_time(start_t)<<" sec.\n";
	o_mstate.time_preproces = end_time(start_t);
	start_t = start_time();
#endif

#ifdef DL_AREAS
	std::cout<<"@prepare:preprocess-end\n";
#endif

#ifdef FM_DR_DOMAINS
	o_domains.print_domains(pattern.nof_nodes);
	o_domains.print_Dlists();
#endif

	//M2:   IF NOT consistent THEN failed:= TRUE; clearDlists; disposeValueAt; RETURN
	//M2:   ELSE failed:= FALSE
	//M2:   END;
	if(!o_mstate.consistent){

#ifdef DL_AREAS
	std::cout<<"_________domains not consistent\n";
#endif

		o_mstate.failed = true;
		//clearDlists()
		//disposeValueAt();
		//dispose by decostructors
		return;
	}
	o_mstate.failed = false;
	//M2:   mcSequence(varAtOrd, ordOfVar, terminal);

#ifdef DL_AREAS
	std::cout<<"@prepare:mcSequence\n";
#endif
	o_sequence.mcSequence();

#ifdef TIMERS
	//std::cout<<"mcSequence: "<<end_time(start_t)<<" sec.\n";
	o_mstate.time_mcsequence = end_time(start_t);
	start_t = start_time();
#endif

#ifdef DL_AREAS
	std::cout<<"@prepare:mcSequence.done\n";
#endif

#ifdef FM_DR_DOMAINS
	o_domains.print_domains(pattern.nof_nodes);
	o_domains.print_Dlists();
#endif
	//M2:   IF terminal THEN
	//M2: 	checkIsomorphism; noSearch:= TRUE; clearDlists; disposeValueAt; RETURN
	//M2:   ELSE noSearch:= FALSE;
	//M2:   END;

	//o_mstate.init_isomorphism(pattern);

#ifdef DL_AREAS
	std::cout<<"@prepare:checkForTerminal\n";
#endif

	if(o_mstate.terminal){
#ifdef DL_AREAS
	std::cout<<"@prepare:terminal\n";
#endif
		checkIsomorphism(pattern, o_domains, o_mstate);
		o_mstate.nosearch = true;
		//clearDlists()
		//diposeValueAt()
		//dispose by decostructor
		return;
	}
	else{
		o_mstate.nosearch = false;
	}

	//M2:   mcSeLists(Dsets, Dlists, varAtOrd, ordOfVar);
#ifdef DL_AREAS
	std::cout<<"@prepare:mcSeLists\n";
#endif
	//o_mstate.init_valueAt(pattern);
	//o_mstate.init_selector(pattern);
	o_glists.init_seLists(pattern, reference);

#ifdef FM_DR_DOMAINS
	o_mstate.print_valueAt(pattern);
	o_mstate.print_selector(pattern);
	o_mstate.print_isomorphism(pattern);
	o_varsorder.print_varAtOrd(pattern);
	o_varsorder.print_ordOfVar(pattern);
#endif

	o_glists.mcSeLists(o_domains, o_mstate, o_varsorder);

#ifdef TIMERS
	//std::cout<<"mcSeLists: "<<end_time(start_t)<<" sec.\n";
	o_mstate.time_mcselists = end_time(start_t);
	start_t = start_time();
#endif

#ifdef DL_AREAS
	std::cout<<"@prepare:mcSeLists.done\n";
#endif

	//M2: END mcConstraints;
};

void prepare(
		focusgraph_t& 		pattern,
		focusgraph_t& 		reference,
		o_domains_t&		o_domains,
		o_predicate_t&		o_predicates,
		o_gLists_t&			o_glists,
		o_vars_order_t&		o_varsorder,
		o_match_state_t&	o_mstate,
		o_gsequence_t&		o_sequence
		){
	mcConstraints(pattern, reference, o_domains, o_predicates, o_glists, o_varsorder, o_mstate, o_sequence);
};


#endif /* FMCGRAPH_H_ */
