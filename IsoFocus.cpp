/*
 * IsoFocus.cpp
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


/*
 * Global directives and variables semantics:
 * - alpha	 			= pattern
 * - beta 	 			= target
 * - PACKEDSET 			translated into SurrograteBitSet
 * - Dset 				is an array of bitvector representing domains
 */


#include "gdex_graph.h"
#include "gdex_domain.h"
#include "gdex_predicate.h"
#include "vars_ordering.h"
#include "match_state.h"
#include "gLists.h"
#include "mp_Tree.h"
#include "prematch.h"
#include "gpreproc.h"
#include "gsequence.h"
#include "iso_check.h"
#include "fMcGraph.h"
#include "fsearch.h"

#include "graph_driver.h"

#include "timer.h"

#include <iostream>

#include "mp_GFUGraphReader.h"
#include "mp_GFDGraphReader.h"
#include "mp_IDer.h"

#include "mp_sbitset.h"


//////M2: FROM gdex   IMPORT domainArrayType, cardPatternType;
////#include "gdex.h"
//////M2: FROM gLists IMPORT clearAll, clearDlists;
////#include "gLists.h"
//////M2: FROM fsearch  IMPORT fosearch;
////#include "fsearch.h"
//////M2: FROM fMcGraph IMPORT prepare;
////#include "fMcGraph.h"
//

void usage(char* cmd){
	std::cout<<"Usage: "<<cmd;
	std::cout<<" [mono] [gfu | gfd | egfu | egfd | vfu]  reference.file pattern.file\n";
	exit(1);
};

int main(int argc, char** argv){

	double time_read = 0, time_prepro = 0, time_match = 0, time_tot = 0;
	u_size_t c_isos = 0, c_nvisited = 0, c_nfails = 0, c_valvisited = 0, c_valfails = 0;
	double time_dlists = 0, time_mcadjvarinfo = 0, time_mcmatrices = 0, time_mcselists = 0, time_mcsequence = 0, time_prematch = 0, time_preproces = 0;


	TIMEHANDLE start_tot, start_p;
	double end_tot, end_read, end_prepro, end_match;


	start_tot = start_time();

	if(argc != 5){
		usage(argv[0]);
	}

	IDer ider;


	GRAPH_FILE_TYPE gft = GFT_GFU;

	if(strcmp(argv[2], "gfu") == 0){
		gft = GFT_GFU;
	}
	else if(strcmp(argv[2], "gfd") == 0){
		gft = GFT_GFD;
	}
	if(strcmp(argv[2], "egfu") == 0){
		gft = GFT_EGFU;
	}
	else if(strcmp(argv[2], "egfd") == 0){
		gft = GFT_EGFD;
	}
	else if(strcmp(argv[2], "fgff") == 0){
		gft = GFT_FGFF;
	}
	else if(strcmp(argv[2], "vfu") == 0){
		gft = GFT_VFU;
	}
	else{
		usage(argv[0]);
	}

#ifdef DEBUG_MAIN
	std::cout<<"___ READ QUERY ___\n";
#endif

	start_p = start_time();

	std::ifstream p_is;
	open_ifstream(p_is, argv[4], gft);
	if(! p_is.is_open()){
		std::cout<<argv[4]<<": file is not opened!\n";
		exit(1);
	}
	GraphReader* p_greader = get_graph_reader(p_is, gft);
	DynamicGraph* d_ref = p_greader->read();
	focusgraph_t p_graph(*d_ref, ider);
#ifdef DEBUG_GRAPH_PRINT
	p_graph.print();
#endif
	d_ref->freeContent();
	delete d_ref;

	end_read = end_time(start_p);
	time_read += end_read;

#ifdef DEBUG_MAIN
	std::cout<<"___ READ TARGET ___\n";
#endif

	std::ifstream r_is;
	open_ifstream(r_is, argv[3], gft);
	if(! r_is.is_open()){
		std::cout<<argv[3]<<": file is not opened!\n";
		exit(1);
	}
	GraphReader* r_greader = get_graph_reader(r_is, gft);

#ifdef DEBUG_MAIN
	int iloop = 0;
#endif
	while(true){
#ifdef DEBUG_MAIN
	std::cout<<"___ READ D TARGET ___\n";
	std::cout<<"iloop: "<<iloop<<"\n";
	iloop++;
#endif
		start_p = start_time();
		DynamicGraph* r_ref = r_greader->read();
		if(r_ref == NULL)
			break;
		focusgraph_t r_graph(*r_ref, ider);
		r_ref->freeContent();
		delete r_ref;
		end_read = end_time(start_p);
		time_read += end_read;


		start_p = start_time();

#ifdef DEBUG_MAIN
	std::cout<<"___ INIT OBJS ___\n";
#endif

		o_domains_t			o_domains(p_graph);
		o_predicate_t		o_predicates;
		o_gLists_t			o_glists(p_graph, r_graph);
		o_vars_order_t		o_varsorder(p_graph);
		o_match_state_t		o_mstate(p_graph, r_graph);
		o_gsequence_t		o_sequence(p_graph,	o_varsorder, o_mstate, o_glists, o_domains);

#ifdef DEBUG_MAIN
	std::cout<<"___ PREPARE ___\n";
#endif

		prepare(
				p_graph,		r_graph,	o_domains,
				o_predicates,	o_glists,	o_varsorder,
				o_mstate,		o_sequence);

		end_prepro = end_time(start_p);
		time_prepro += end_prepro;

		start_p = start_time();

#ifdef DEBUG_MAIN
	std::cout<<"___ INIT FOSEARCH ___\n";
#endif

		end_match = 0;

		o_fosearch_t o_fosearch(
					p_graph,		r_graph,	o_domains,
					o_predicates,	o_glists,	o_varsorder,
					o_mstate,		o_sequence
					);

		if(o_mstate.failed){
			//o_domains.print_domains(p_graph.nof_nodes);
			o_mstate.isomorphisms = 0;
		}
		else if(o_mstate.nosearch){
			//single solution
			o_mstate.isomorphisms = 1;
		}
		else{
#ifdef DEBUG_MAIN
	std::cout<<"___ SEARCH ___\n";
#endif
			start_p = start_time();

			o_fosearch.fosearch();

			end_match = end_time(start_p);
			time_match += end_match;
		}

#ifdef DEBUG_MAIN
	std::cout<<"___ DONE ___\n";
#endif


		c_isos += o_mstate.isomorphisms;
		c_nvisited += o_fosearch.nodeVisited;
		c_nfails += o_fosearch.nodeFails;
		c_valvisited += o_fosearch.valuesVisited;
		c_valfails += o_fosearch.valuesFails;
		time_dlists += o_mstate.time_dlists;
		time_mcadjvarinfo +=  o_mstate.time_mcadjvarinfo;
		time_mcmatrices += o_mstate.time_mcmatrices;
		time_mcselists += o_mstate.time_mcselists;
		time_mcsequence += o_mstate.time_mcsequence;
		time_prematch += o_mstate.time_prematch;
		time_preproces += o_mstate.time_preproces;

#ifdef DEBUG_MAIN
	std::cout<<"___ DONE iLOOP___\n";
	std::cout<<"\tisos:"<<c_isos<<"\n";
#endif
	}

#ifdef DEBUG_MAIN
	std::cout<<"___ DONE LOOP___\n";
#endif

	end_tot = end_time(start_tot);
	time_tot += end_tot;


	std::cout<<"#"<<argv[4]<<"\t#"<<argv[3];
	std::cout<<"\t"<<time_read<<"\t"<<time_prepro<<"\t"<<time_match<<"\t"<<time_tot;
	std::cout<<"\t"<<c_isos;
	std::cout<<"\t"<<c_nvisited<<"\t"<<c_nfails;
	std::cout<<"\t"<<c_valvisited<<"\t"<<c_valfails;
	std::cout<<"\t"<<time_dlists;
	std::cout<<"\t"<<time_mcadjvarinfo;
	std::cout<<"\t"<<time_mcmatrices;
	std::cout<<"\t"<<time_mcselists;
	std::cout<<"\t"<<time_mcsequence;
	std::cout<<"\t"<<time_prematch;
	std::cout<<"\t"<<time_preproces;
	std::cout<<"\t";

#ifdef DEBUG_MAIN
	std::cout<<"___ END ___\n";
#endif

//
//
//	//M2: CONST
//	//M2:   noOfTrials = 25;
//	//M2: VAR
//	//M2:   Dsets: domainArrayType;
//	//M2:   varAtOrd, ordOfVar: cardPatternType;
//	//M2:   elapsedTime: LONGREAL;
//	//M2:   whichTrial, nodesVisited, isomorphisms: CARDINAL;
//	//M2:   failed, foundOriginal, noSearch: BOOLEAN;
//
//
//	//M2: FOR whichTrial:= 1 TO noOfTrials DO
//	//M2:  WrCard(whichTrial, 5);
//	//M2:  prepare(Dsets, varAtOrd, ordOfVar, failed,  noSearch);
//	//M2:  IF failed THEN
//	//M2:    meepLn(' NO isomorphism because of empty domain'); WrStr(' Insoluble'); WrLn;
//	//M2:    timeTaken(elapsedTime); annaLngReal(2, elapsedTime);
//	//M2:  ELSIF noSearch THEN
//	//M2:    timeTaken(elapsedTime); WrStr('  Iso found at prepro. No search.'); WrLn;
//	//M2:    annaCard(1,1); annaLngReal(2, elapsedTime);
//	//M2:  ELSE
//	//M2:    fosearch(Dsets, varAtOrd, ordOfVar, nodesVisited, isomorphisms, foundOriginal);
//	//M2:    IF NOT foundOriginal THEN WrStr('  Failed to find original'); WrLn;
//	//M2:  meepLn('  Failed to find Original'); HALT
//	//M2:    END;
//	//M2:    timeTaken(elapsedTime);
//	//M2:    WrStr('  Isos = '); WrCard(isomorphisms, 8);
//	//M2:    WrStr(',    Time = '); WrReal(elapsedTime, 7,11);
//	//M2:    annaCard(1,isomorphisms); annaLngReal(2, elapsedTime); annaCard(3, nodesVisited);
//	//M2:    annaWrAv(2);
//	//M2:    clearDlists;
//	//M2:  END;
//	//M2:  clearAll;
//	//M2: END;
//
	
	exit(0);
};

