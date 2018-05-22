/*
 * gsequence.h
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


#ifndef GSEQUENCE_H_
#define GSEQUENCE_H_

#include "limits.h"

//#include "gdex_graph.h"
//#include "gdex_domain.h"
#include "gLists.h"
#include "match_state.h"
#include "vars_ordering.h"

#include "debug.h"
//
//#define GSEQ_CICLES
//#define GSEQ_CALLS
//#define GSEQ_I_INS



//M2: TYPE
//M2:   vPtrType = POINTER TO vRecType;
//M2:   vRecType = RECORD
//M2:                  adjacentListPtr: adjVarPtrType;
//M2:                  varNo, degree, dbranches, nbAdjacentCovered:  CARDINAL;
//M2: 		 			previous, next: vPtrType;
//M2: 		 			covered: BOOLEAN
//M2: 	       		END;
class 			vRec_t{
public:
	adjVar_rec_t 	*adjacentListPtr;
	cardinal_t 		varNo, degree, dbranches, nbAdjacentCovered;
	vRec_t 			*previuos, *next;
	bool 			covered;

	vRec_t(){
		adjacentListPtr = NULL;
		varNo = degree = dbranches = nbAdjacentCovered = 0;
		previuos = next = NULL;
		covered = false;
	}

	~vRec_t(){
		//TODO
	}
};
//typedef vRec_t* v_ptr_t;
//M2: vIndexType = ARRAY[0..alphaUB] OF vPtrType;
//typedef v_ptr_t* vIndex_t;
typedef vRec_t		**vIndex_t;





class o_gsequence_t{
	//M2: CONST
	//M2:   maxRandomOverlap = nAlpha DIV 4 + 20 ;  (* A guess *)
	//M2:   nil = 9999;
	//M2:   dummy = 99999;
#define DUMMY MAXCARD;
	//M2:   lastVar = 30;  (* The number of variables whose predecessors are counted *)
	//M2:   overlapThreshold = 2;  (* count variables having fewer than this number of predecessors *)
private:
	class _iternal_s{
	public:
		cardinal_t	maxOverlap;
		vIndex_t 	vIndex;
		vRec_t		**overlapLists;
		cardinal_t	nextOrd;
		cardinal_t	iChosen;
		cardinal_t  maxRandomOverlap;

		_iternal_s(cardinal_t  _maxRandomOverlap){
			maxRandomOverlap = _maxRandomOverlap;

			maxOverlap = 0;
			vIndex = NULL;
			overlapLists = NULL;
			nextOrd = iChosen = 0;
		}

		~_iternal_s(){
#ifdef DEBUG_DELETE
		std::cout<<"~_iternal_s>\n";
#endif
//M2: FOR i:= 0 TO maxRandomOverlap DO DISPOSE(overlapLists[i]) END;
//M2: FOR i:= 0 TO alphaUB DO IF vIndex[i] # NIL THEN DISPOSE(vIndex[i]) END END;
			if(vIndex != NULL){
				for(cardinal_t i = 0; i<maxRandomOverlap; i++){
					if(vIndex[i]!=NULL){
						delete vIndex[i];
//						if(vIndex[i]->next == vIndex[i]){
//							delete vIndex[i];
//						}
//						else{
//							vRec_t *p_i = NULL, *c_i = vIndex[i];
//							while(c_i!=NULL){
//								if(p_i != NULL) delete p_i;
//								p_i = c_i;
//								c_i = c_i->next;
//							}
//							if(p_i != NULL)
//								delete p_i;
//						}
						vIndex[i] = NULL;
					}
				}
			}
			if(overlapLists != NULL){
				for(cardinal_t i = 0; i<maxRandomOverlap; i++){
					if(overlapLists[i]!=NULL){
						delete overlapLists[i];
//						if(overlapLists[i]->next == overlapLists[i]){
//							delete overlapLists[i];
//						}
//						else{
//							vRec_t *p_i = NULL, *c_i = overlapLists[i];
//							while(c_i!=NULL){
//								if(p_i != NULL) delete p_i;
//								p_i = c_i;
//								c_i = c_i->next;
//							}
//							if(p_i != NULL)
//								delete p_i;
//						}
						overlapLists[i] = NULL;
					}
				}
			}
#ifdef DEBUG_DELETE
		std::cout<<"~_iternal_s<\n";
#endif
		}
	};

	void mcRecords(_iternal_s& _i, cardinal_t *branches);
	void updateCover(_iternal_s& _i);
	void selectNextVar(_iternal_s& _i);
	void initialise(_iternal_s& _i);


public:

	focusgraph_t&		pattern;
	o_vars_order_t& 	o_varsorder;
	o_match_state_t& 	o_mstate;
	o_gLists_t&			o_glists;
	o_domains_t&		o_domains;
	cardinal_t 			maxRandomOverlap;

	o_gsequence_t(
			focusgraph_t& 		_pattern,
			o_vars_order_t& 	_o_varsorder,
			o_match_state_t& 	_o_mstate,
			o_gLists_t&			_o_glists,
			o_domains_t&		_o_domains
			)
				: pattern(_pattern),
				  o_varsorder (_o_varsorder),
				  o_mstate (_o_mstate),
				  o_glists (_o_glists),
				  o_domains (_o_domains)
	{
		//maxRandomOverlap = pattern.nof_nodes / 4 + 20;  //BAD GUESS!!!!!!!!!!!!!
		maxRandomOverlap = pattern.nof_nodes;
	};

	~o_gsequence_t(){
#ifdef DEBUG_DELETE
		std::cout<<"~o_gsequence_t>\n";
#endif
#ifdef DEBUG_DELETE
		std::cout<<"~o_gsequence_t<\n";
#endif
	}


	void mcSequence(){
		//M2: maxOverlap:= 2; initialise(iChosen, terminal);
		//M2: IF terminal THEN disposeSequenceRecords; RETURN END;
		//M2: WHILE nextOrd <= alphaUB DO
		//M2: 	updateCover(iChosen);
		//M2: 	selectNextVar(iChosen);
		//M2:	varAtOrd[nextOrd]:= iChosen; ordOfVar[iChosen]:= nextOrd; INC(nextOrd);
		//M2: END;
		//M2: disposeSequenceRecords;

		_iternal_s _i(maxRandomOverlap);
			_i.maxOverlap = 2;
			_i.vIndex = new vRec_t*[pattern.nof_nodes];
			_i.overlapLists = new vRec_t*[maxRandomOverlap];
			for(cardinal_t i=0; i<maxRandomOverlap; i++){
				_i.overlapLists[i] = NULL;
			}
			for(u_size_t i=0; i<pattern.nof_nodes; i++){
				_i.vIndex[i] = NULL;
			}

			_i.iChosen = 0;
			_i.nextOrd = 0;

//		cardinal_t maxOverlap = 2;
//		cardinal_t iChosen;
//		cardinal_t *nextOrd;
//
//		vIndex_t vIndex = new vRec_t*[pattern.nof_nodes];
//		vRec_t** overlapLists = new vRec_t*[maxRandomOverlap];

//		initialise(	pattern, iChosen, terminal, Dcards, varAtOrd,
//					ordOfVar, adjVarInfo, degree, &maxOverlap,
//					nextOrd, vIndex, overlapLists);
		initialise(_i);


#ifdef DR_DOMAINS
		std::cout<<"@mcSequence:after_initiliase\n";
		o_varsorder.print_ordOfVar(pattern);
		o_varsorder.print_varAtOrd(pattern);
#endif


		if(o_mstate.terminal){
#ifdef GSEQ_CALLS
			std::cout<<"<mcSequence: o_mstate.terminal\n";
#endif
			return;
		}

#ifdef DL_INSTRCS
		std::cout<<"mcSequence: _i.nextOrd:"<<_i.nextOrd<<"\n";
#endif
		while(_i.nextOrd < pattern.nof_nodes){
#ifdef GSEQ_CICLES
			std::cout<<"-mcSequence:while: _i.nextOrd:"<<_i.nextOrd<<"\n";
#endif
//			updateCover(*iChosen, adjVarInfo, pattern, vIndex, &maxOverlap, overlapLists);
//			selectNextVar(iChosen, &maxOverlap, overlapLists, vIndex);
			updateCover(_i);
			selectNextVar(_i);

#ifdef GSEQ_CICLES
			std::cout<<"-mcSequence:while: _i.nextOrd:"<<_i.nextOrd<<"\n";
			std::cout<<"-mcSequence:while: _i.iChosen:"<<_i.iChosen<<"\n";
#endif

			o_varsorder.varAtOrd[_i.nextOrd] = _i.iChosen;
			o_varsorder.ordOfVar[_i.iChosen] = _i.nextOrd;
			(_i.nextOrd)++;
		}
#ifdef DR_DOMAINS
		std::cout<<"@mcSequence:end\n";
		o_varsorder.print_ordOfVar(pattern);
		o_varsorder.print_varAtOrd(pattern);
#endif
	}
};



#endif /* GSEQUENCE_H_ */
