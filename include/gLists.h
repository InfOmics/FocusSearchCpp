/*
 * gLists.h
 *
 *  Created on: Feb 27, 2012
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


#ifndef GLISTS_H_
#define GLISTS_H_

#include "gdex_domain.h"
#include "gdex_predicate.h"
#include "match_state.h"
#include "vars_ordering.h"

#include "size_t.h"

#include "debug.h"


////M2:	DlistRecType 				= RECORD
////M2:										value: CARDINAL;
////M2:										nextValue: DlistPtrType
////M2:								  END;
//class 								Dlist_rec_t{
//public:
//	cardinal_t value;
//	Dlist_rec_t* nextValue;
//
//	Dlist_rec_t(){
//		value  = 0;
//		nextValue = NULL;
//	}
//
//	~Dlist_rec_t(){
//	}
//};

//M2:	DlistPtrType 				= POINTER TO DlistRecType;
//typedef Dlist_rec_t* 				Dlist_ptr_t;

//M2:	selistsType  				= ARRAY[0..alphaUB], [0..betaUB] OF DlistPtrType;
typedef Dlist_rec_t***				seLists_t;

//M2:	cardPtrType  				= POINTER TO CARDINAL;
//typedef cardinal_t* 				card_ptr_t;

//M2:	adjVarRecType 				= RECORD
//M2:										iMjPtr, jMiPtr: bitMatrixPointerType;
//M2:										lineNo, adjVar: CARDINAL;
//M2:			  							valuePtr: cardPtrType;
//M2:			  							next: adjVarPtrType
//M2:								  END;
class						adjVar_rec_t{
public:
	bitMatrix_t 	*iMjPtr, *jMiPtr;
	cardinal_t		lineNo, adjVar;
	cardinal_t 		*valuePtr;
	adjVar_rec_t	*next;

//	cardinal_t e;
//	u_size_t iMjPtr_size;
//	cardinal_t node;
//
//	ETYPE_t direction;

	adjVar_rec_t(){
		iMjPtr = NULL;
		jMiPtr = NULL;
		lineNo = 0;
		adjVar = 0;
		valuePtr = NULL;
		next = NULL;

//		e = 0;
//		iMjPtr_size = 0;
//		node = 0;
//
//		direction = ETYPE_N;
	}

	~adjVar_rec_t(){
	}

	void print(){
		std::cout<<"(";
		if(iMjPtr == NULL)
			std::cout<<"NULL, ";
		if(jMiPtr == NULL)
			std::cout<<"NULL, ";
		std::cout<<lineNo<<", ";
		std::cout<<adjVar<<", ";
		if(valuePtr == NULL)
			std::cout<<"NULL, ";
//		std::cout<<e<<", ";
//		std::cout<<node<<", ";
		std::cout<<")";
	}
};

//M2:	adjVarPtrType 				= POINTER TO adjVarRecType;
//typedef adjVar_rec_t*				adjVar_ptr_t;



class o_gLists_t{
public:
//	DLists_t			Dlists;
	seLists_t			seLists;
	adjVar_rec_t		**adjVarInfo;

	//M2: degree: cardPatternType;
	cardinal_t 			*degree;

	focusgraph_t& pattern;
	focusgraph_t& reference;

	o_gLists_t(focusgraph_t& _pattern, focusgraph_t& _reference)
				: pattern(_pattern), reference(_reference){
//		Dlists = NULL;
		seLists = NULL;
		adjVarInfo = NULL;
	};

	~o_gLists_t(){
#ifdef DEBUG_DELETE
		std::cout<<"~o_gLists_t>\n";
#endif
//		//TODO
		delete [] degree;
//
////#ifdef DEBUG_DELETE
////		std::cout<<"\tdel dlists\n";
////#endif
////		if(Dlists != NULL){
////			Dlist_rec_t *c_d, *p_d;
////			for(u_size_t i=0; i<pattern.nof_nodes; i++){
////				if(Dlists[i] != NULL){
////					p_d = NULL;
////					c_d = Dlists[i];
////					while(c_d != NULL){
////						if(p_d != NULL) delete p_d;
////						p_d = c_d;
////						c_d = c_d->nextValue;
////					}
////					if(p_d != NULL) delete p_d;
////				}
////			}
////			delete [] Dlists;
////		}
//
#ifdef DEBUG_DELETE
		std::cout<<"\tdel adjVarInfo\n";
#endif

		if(adjVarInfo != NULL){
			adjVar_rec_t *c_d, *p_d;
			for(u_size_t i=0; i<pattern.nof_nodes; i++){
				if(adjVarInfo[i] != NULL){
					p_d = NULL;
					c_d = adjVarInfo[i];
					while(c_d != NULL ){
						if(p_d != NULL  &&  p_d!=adjVarInfo[i]) delete p_d;
						p_d = c_d;
						c_d = c_d->next;
					}
					if(p_d != NULL  &&  p_d!=adjVarInfo[i]) delete p_d;
					//adjVarInfo[i] = NULL;
				}
			}
			delete [] adjVarInfo;
		}


#ifdef DEBUG_DELETE
		std::cout<<"\tdel seLists\n";
#endif
		if(seLists != NULL){
			Dlist_rec_t *c_d, *p_d;
			for(u_size_t i=0; i<pattern.nof_nodes; i++){
				if(seLists[i] != NULL){// new Dlist_rec_t*[reference.nof_nodes];
					for(u_size_t j=0; j<reference.nof_nodes; j++){
						if(seLists[i][j] != NULL){
							p_d = NULL;
							c_d = seLists[i][j];
							while(c_d != NULL){
								if(p_d != NULL) delete p_d;
								p_d = c_d;
								c_d = c_d->nextValue;
							}
							if(p_d != NULL) delete p_d;
						}
					}
					delete [] seLists[i];
				}
			}
			delete [] seLists;
		}
#ifdef DEBUG_DELETE
		std::cout<<"~o_gLists_t<\n";
#endif
	}

//	void init_DLists(focusgraph_t& pattern){
//		Dlists = new Dlist_rec_t*[pattern.nof_nodes];
//		for(u_size_t i=0; i<pattern.nof_nodes; i++){
//			Dlists[i] = NULL;
//		}
//	};

//	void init_DLists(focusgraph_t& pattern, o_domains_t& o_domains){
//		Dlists = new Dlist_rec_t*[pattern.nof_nodes];
//		for(u_size_t i=0; i<pattern.nof_nodes; i++){
//			Dlists[i] = NULL;
//		}
//	}

	void init_seLists(focusgraph_t& pattern, focusgraph_t& reference){
		seLists = new Dlist_rec_t**[pattern.nof_nodes];
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			seLists[i] = new Dlist_rec_t*[reference.nof_nodes];
			for(u_size_t j=0; j<reference.nof_nodes; j++){
				seLists[i][j] = NULL;
			}
		}
	};
	void init_adjVarInfo(focusgraph_t& pattern){
		//M2: adjVarInfo: ARRAY[0..alphaUB] OF adjVarPtrType;
		adjVarInfo = new adjVar_rec_t*[pattern.nof_nodes];
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			adjVarInfo[i] = NULL;
		}
	};

	void init_degree(focusgraph_t& pattern){
		//degree = new cardinal_t[pattern.nof_nodes];
		degree = (cardinal_t*)calloc(pattern.nof_nodes, sizeof(cardinal_t));
	}



	void mcSeLists(
			o_domains_t& o_domains,
			o_match_state_t& o_mstate,
			o_vars_order_t& o_varsorder);

	void mcAdjVarInfo(
			o_predicate_t& o_predicates, bool ordered);


//	void print_Dlists(){
//		std::cout<<"##DLists::\n";
//		for(int i=0; i<pattern.nof_nodes; i++){
//			std::cout<<"node["<<i<<"]: ";
//			if(Dlists[i]== NULL){
//				std::cout<<"no Dlists";
//			}else{
//				Dlist_rec_t* d = Dlists[i];
//				std::cout<<"[";
//				while(d!=NULL){
//					std::cout<<d->value<<" ";
//					d=d->nextValue;
//				}
//				std::cout<<"]";
//			}
//			std::cout<<"\n";
//		}
//	}

	void print_adjVarInfo(){
		std::cout<<"##adjVarInfo::\n";
		if(adjVarInfo == NULL)std::cout<<"NULL\n";
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			std::cout<<"node["<<i<<"]";
			if(adjVarInfo[i] == NULL)std::cout<<"NULL\n";
			else{
				std::cout<<"{\n";
				adjVar_rec_t* a = adjVarInfo[i];
				while(a != NULL){
					std::cout<<"(";
					std::cout<<a->adjVar<<", ";
					std::cout<<a->lineNo<<", ";
					if(a->valuePtr == NULL)std::cout<<"NULL, ";
					else std::cout<<*(a->valuePtr)<<", ";
					if(a->iMjPtr == NULL)std::cout<<"NULL, ";
					if(a->jMiPtr == NULL)std::cout<<"NULL, ";
					std::cout<<"iMjPtr: "<<a->iMjPtr<<", ";
					std::cout<<"jMiPtr: "<<a->jMiPtr<<", ";
//					std::cout<<a->e<<",";
//					std::cout<<a->iMjPtr_size<<", ";
//					std::cout<<a->node<<", ";
					std::cout<<")\n";
					a = a->next;
				}
				std::cout<<"}\n";
			}
		}
	}

	void print_adjVarInfo(adjVar_rec_t* a){
		if(a == NULL)std::cout<<"adjVarInfo = NULL\n";
		else{
			std::cout<<"(";
			std::cout<<a->adjVar<<", ";
			std::cout<<a->lineNo<<", ";
			if(a->valuePtr == NULL)std::cout<<"NULL, ";
			else std::cout<<*(a->valuePtr)<<", ";
			if(a->iMjPtr == NULL)std::cout<<"NULL, ";
			if(a->jMiPtr == NULL)std::cout<<"NULL, ";
			std::cout<<"iMjPtr: "<<a->iMjPtr<<", ";
			std::cout<<"jMiPtr: "<<a->jMiPtr<<", ";
//			std::cout<<a->e<<",";
//			std::cout<<a->iMjPtr_size<<", ";
//			std::cout<<a->node<<", ";
			std::cout<<")\n";
		}
	}

	void print_adjVarInfo_Ms(adjVar_rec_t* a){
		std::cout<<"adjVarInfo_Ms:\n";
		for(int i=0; i<reference.nof_nodes;i++){
			std::cout<<"\tiMj["<<i<<"]\n";
			(*(a->iMjPtr)[i]).print_info();
			std::cout<<"\tjMi["<<i<<"]\n";
			(*(a->jMiPtr)[i]).print_info();
		}
	}

	void print_M(bitMatrix_t *M){
		for(cardinal_t i=0; i<reference.nof_nodes; i++){
			std::cout<<"printM:i:"<<i<<"\n";
			std::cout<<"\tM["<<i<<"]\n";
			(*M[i]).print_info();
		}
	}

	void print_degree(){
		std::cout<<"##degree\n";
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			std::cout<<"["<<i<<":"<<degree[i]<<"]";
		}
		std::cout<<"\n";
	}
};


#endif /* GLISTS_H_ */
