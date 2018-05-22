/*
 * gdex_predicate.h
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


#ifndef GDEX_PREDICATE_H_
#define GDEX_PREDICATE_H_

//#include "gdex_graph.h"
#include "gdex_domain.h"

#include "debug.h"

#include <set>


//M2: bitMatrixType 			= ARRAY[0..betaUB] OF domainType;
typedef domain_t				*bitMatrix_t;

//M2: bitMatrixPointerType 		= POINTER TO bitMatrixType;
//typedef bitMatrix_t*			bitMatrix_ptr_t;

//M2: RbType 					= ARRAY[1..eBetaMax] OF bitMatrixPointerType;	Mij indexed by edge attributes
//								[1..eBetaMax]bitMatrixPointerType( [0..betaUB]domainType( [0..betaUB] ) )
//								[1..eBetaMax][0..betaUB]

typedef bitMatrix_t 			**predicate_Mij_t; 		//=  SurrogateBitSet*** [attr_id, i](j)
//								bitMatrix_ptr_t*	[1..eBetaMax]
//								bitMatrix_t**		[1..eBetaMax]
//								domain_t***			[1..eBetaMax]
//								SurrogateBitSet***	[1..eBetaMax]

class o_predicate_t{
public:
	predicate_Mij_t Rb1;
	predicate_Mij_t Rb2;

	int _l1;
	int _l2;

	o_predicate_t(){_l1=0; _l2=0;};

	~o_predicate_t(){
#ifdef DEBUG_DELETE
		std::cout<<"~o_predicate_t>\n";
#endif
		delete [] Rb1;
		delete [] Rb2;


//		if(Rb1 != NULL){
//			for(u_size_t i=0; i<_l1; i++){
//				if(Rb1[i] != NULL){
//					for(u_size_t j=0; j<_l2; i++){
//						if(Rb1[i][j] != NULL){
//							delete Rb1[i][j];
//							//Rb1[i][j] = NULL;
//						}
//					}
//				}
//			}
//			delete [] Rb1;
//		}
//		if(Rb2 != NULL){
//			for(u_size_t i=0; i<_l1; i++){
//				if(Rb2[i] != NULL){
//					for(u_size_t j=0; j<_l2; i++){
//						if(Rb2[i][j] != NULL){
//							delete Rb2[i][j];
//							//Rb2[i][j] = NULL;
//						}
//					}
//				}
//			}
//			delete [] Rb2;
//		}
#ifdef DEBUG_DELETE
		std::cout<<"~o_predicate_t<\n";
#endif
	}

	void print_predicates(){
		std::cout<<"Predicate Mij\n";
		for(u_size_t a=0; a<_l1; a++){
			for(u_size_t i=0; i<_l2; i++){
				std::cout<<"["<<a<<"]["<<i<<"]:["; Rb1[a][i]->print_numbers();std::cout<<"]\n";
				std::cout<<"p["<<a<<"]: "<<Rb1[a]<<"\n";
				std::cout<<"p["<<a<<"]["<<i<<"]: "<<Rb1[a][i]<<"\n";
			}
		}
		std::cout<<"Predicate Mji\n";
		for(u_size_t a=0; a<_l1; a++){
			for(u_size_t i=0; i<_l2; i++){
				std::cout<<"["<<a<<"]["<<i<<"]:["; Rb2[a][i]->print_numbers();std::cout<<"]\n";
				std::cout<<"p["<<a<<"]: "<<Rb2[a]<<"\n";
				std::cout<<"p["<<a<<"]["<<i<<"]: "<<Rb2[a][i]<<"\n";
			}
		}
	}


	void init_predicate_Mij_t(
			predicate_Mij_t& 	p,
			int 				l1_size,
			int 				l2_size
			){
#ifdef DL_CALLS
		std::cout<<">>gdex_predicate:init_predicateMijt("<<l1_size<<", "<<l2_size<<")\n";
#endif
		p = new  bitMatrix_t*[l1_size];
		for(u_size_t a=0; a<l1_size; a++){
			(p)[a] = new bitMatrix_t[l2_size];
			for(u_size_t i=0; i<l2_size; i++){
				(p)[a][i] = new domain_t();
			}
		}
#ifdef DL_CALLS
		std::cout<<"<gdex_predicate:init_predicateMijt()\n";
#endif
	};

	void print_predicate_Mij_t(
			predicate_Mij_t& 	p,
			int 				l1_size,
			int 				l2_size
			){
		std::cout<<"#Predicate\n";
		for(u_size_t a=0; a<l1_size; a++){
			for(u_size_t i=0; i<l2_size; i++){
				//(p)[a][i] = new SurrogateBitSet();
				std::cout<<"["<<a<<"]["<<i<<"]:["; p[a][i]->print_numbers();std::cout<<"]\n";
				std::cout<<"p["<<a<<"]: "<<p[a]<<"\n";
				std::cout<<"p["<<a<<"]["<<i<<"]: "<<p[a][i]<<"\n";
				//p[a][i]->print_info();
			}
		}
	};

	bool mcMatrices(focusgraph_t& pattern, focusgraph_t& reference);
};

#endif /* GDEX_PREDICATE_H_ */
