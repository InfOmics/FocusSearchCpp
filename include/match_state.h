/*
 * match_state.h
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


#ifndef MATCH_STATE_H_
#define MATCH_STATE_H_

#include "gdex_graph.h"
#include "gdex_domain.h"

#include "debug.h"

//#define M_CALLS


//M2: cardPtrType  = POINTER TO CARDINAL;
//M2: ARRAY[0..alphaUB] OF cardPtrType;
//M2: valueAt, selector: ARRAY[0..alphaUB] OF cardPtrType;


//M2: PROCEDURE McEmpty(VAR Dsets: domainArrayType);
//M2: VAR
//M2:   i, q, u: CARDINAL;
//M2: BEGIN
//M2:   FOR i:= 0 TO alphaUB DO
//M2:     Dsets[i]:= domainType{}; NEW(valueAt[i]);
//M2:     FOR u:= 0 TO betaUB DO seLists[i, u]:= NIL END;
//M2:     adjVarInfo[i]:= NIL;
//M2:   END;
//M2: EXCEPT
//M2:   meepCard('McEmpty fails when q = ', q, 4); peepLn; HALT;
//M2: END McEmpty;





class o_match_state_t{
public:

	cardinal_t		**valueAt;
	cardinal_t		**selector;
	u_size_t 		free_size;

	domain_t 		allowed;
	bool 			consistent;
	bool 			failed;
	bool			nosearch;
	bool			terminal;

	cardinal_t		*isomorphism;
	cardinal_t		isomorphisms;



	double time_dlists, time_prematch, time_mcmatrices, time_mcadjvarinfo, time_preproces, time_mcsequence, time_mcselists;


private:
	void ini_allowed(focusgraph_t& pattern, focusgraph_t& reference){
		allowed.setAll(reference.nof_nodes, true);
//		allowed.resize(pattern.nof_nodes);
//		for(u_size_t i=0; i<reference.nof_nodes; i++){
//			allowed.set(i, true);
//		}
	}


	void init_valueAt(focusgraph_t& pattern){
		//TODO check for delete on assignment
		valueAt = new cardinal_t*[pattern.nof_nodes];
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			//valueAt[i] = NULL;
			valueAt[i] = new cardinal_t();
		}
	}

	void init_selector(focusgraph_t& pattern){
		//TODO check for delete on assignment
		selector = new cardinal_t*[pattern.nof_nodes];
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			//selector[i] = NULL;
			selector[i]  = new cardinal_t();
		}
	}

	void init_isomorphism(focusgraph_t& pattern){
		//isomorphism = new cardinal_t[pattern.nof_nodes];
		isomorphism = (cardinal_t*)calloc(pattern.nof_nodes, sizeof(cardinal_t));
	}

public:

	o_match_state_t(focusgraph_t& pattern, focusgraph_t& reference){
		consistent 	= true;
		failed 		= false;
		nosearch 	= false;
		isomorphisms = 0;

		ini_allowed(pattern, reference);
		init_valueAt(pattern);
		init_selector(pattern);
		init_isomorphism(pattern);

		free_size = pattern.nof_nodes;


		time_dlists = time_prematch = time_mcmatrices = time_mcadjvarinfo = time_preproces = time_mcsequence = time_mcselists = 0;
	}

	void free_valueAt(u_size_t size){
#ifdef DEBUG_DELETE
		std::cout<<"~o_match_state_t.free_valueAt>\n";
#endif
		if(valueAt!=NULL){
			for(u_size_t i=0; i<size; i++){
				if(valueAt[i] != NULL){
					delete valueAt[i];
					valueAt[i] = NULL;
				}
			}
			delete [] valueAt;
		}
		valueAt = NULL;
#ifdef DEBUG_DELETE
		std::cout<<"~o_match_state_t.free_valueAt<\n";
#endif
	}
	void free_selector(u_size_t size){
#ifdef DEBUG_DELETE
		std::cout<<"~o_match_state_t.free_selector>\n";
#endif
		if(selector!=NULL){
			for(u_size_t i=0; i<size; i++){
				if(selector[i] != NULL){
					delete selector[i];
					selector[i] = NULL;
				}
			}
			delete [] selector;
		}
		selector = NULL;
#ifdef DEBUG_DELETE
		std::cout<<"~o_match_state_t.free_selector<\n";
#endif
	}


	~o_match_state_t(){
#ifdef DEBUG_DELETE
		std::cout<<"~o_match_state_t>\n";
#endif
//		delete [] isomorphism;

//		free_valueAtT();
//		free_selector();
#ifdef DEBUG_DELETE
		std::cout<<"~o_match_state_t<\n";
#endif
	}




	void print_valueAt(focusgraph_t& pattern){
		std::cout<<"#m_state:valueAt:";
		if(valueAt==NULL)
			std::cout<<"NULL";
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			if(valueAt[i] == NULL)
				std::cout<<"["<<i<<":NULL]";
			else
				std::cout<<"["<<i<<":"<<(*(valueAt[i]))<<"]";
		}
		std::cout<<"\n";
	}
	void print_selector(focusgraph_t& pattern){
		std::cout<<"#m_state:selector:";
		if(selector==NULL)
			std::cout<<"NULL";
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			if(selector[i] == NULL)
				std::cout<<"["<<i<<":NULL]";
			else
				std::cout<<"["<<i<<":"<<(*(selector[i]))<<"]";
		}
		std::cout<<"\n";
	}
	void print_isomorphism(focusgraph_t& pattern){
		std::cout<<"#m_state:isomorphism:";
		if(isomorphism==NULL)
			std::cout<<"NULL";
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			std::cout<<"["<<i<<":"<<isomorphism[i]<<"]";
		}
		std::cout<<"\n";
	}
};



#endif /* MATCH_STATE_H_ */
