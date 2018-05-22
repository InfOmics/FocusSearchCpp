/*
 * fsearch.h
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

#ifndef FSEARCH_H_
#define FSEARCH_H_

#include "debug.h"

//#define FS_CALLS
//#define FS_INS
//#define FS_AREAS
//#define FS_DOMAINS
//#define FS_IINS
//#define PRINT_MATCH

#include "gdex_domain.h";
#include "vars_ordering.h"
#include "gdex_predicate.h"
#include "match_state.h"
#include "gLists.h"
#include "gsequence.h"

#include "size_t.h"


//M2: futRecordType 				= RECORD
//M2:                     					iFuture: CARDINAL;
//M2: 		    							pastListPtr: adjVarPtrType;
//M2: 		    							nextFuture: futPtrType;
//M2: 		  						END;
class								fut_rec_t{
public:
	cardinal_t		iFuture;
	adjVar_rec_t	*pastListPtr;
	fut_rec_t		*nextFuture;

	fut_rec_t(){
		iFuture = 0;
		pastListPtr = NULL;
		nextFuture = NULL;
	}

	~fut_rec_t(){
	}


	void print(){
		std::cout<<"("<<iFuture<<", ";
		if(pastListPtr == NULL)
			std::cout<<"NULL, ";
		else
			std::cout<<pastListPtr<<", ";
		if(nextFuture == NULL)
			std::cout<<"NULL";
		else
			std::cout<<nextFuture<<", ";
		std::cout<<")";
	}
};
//M2: futPtrType 					= POINTER TO futRecordType;
//typedef fut_rec_t*				fut_ptr_t;
//M2: setsAt					:ARRAY[0..alphaUB] OF futPtrType;


class o_fosearch_t{

	focusgraph_t& 		pattern;
	focusgraph_t& 		reference;
	o_domains_t&		o_domains;
	o_predicate_t&		o_predicates;
	o_gLists_t&			o_glists;
	o_vars_order_t&		o_varsorder;
	o_match_state_t&	o_mstate;
	o_gsequence_t&		o_sequence;


	//M2: VAR
	//M2:   successor, predecessor: cardPatternType;
	//M2:   dD: domainArrayType;
	//M2:   domain: domainType;
	//M2:   setsAt: ARRAY[0..alphaUB] OF futPtrType;
	//M2:   next: ARRAY[0..alphaUB] OF DlistPtrType;
	//M2:   before: ARRAY[0..alphaUB] OF adjVarPtrType;
	//M2:   after:  ARRAY[0..alphaUB] OF futPtrType;
	//M2:   valuePtr, initialDlist: DlistPtrType;
	//M2:   i, firstVar, lastVar: CARDINAL;
	//M2:   consistent, found: BOOLEAN;
	//M2:   v: CARDINAL;
public:
	cardinal_t 		*lambda;
	fut_rec_t 		**after;
	adjVar_rec_t 	**before;
	cardinal_t		*predecessor;
	cardinal_t		*successor;
	fut_rec_t		**setsAt;
	domain_t		*dD;
	cardinal_t		firstVar, lastVar;
	bool			foundOriginal;
	bool			found;
	bool			consistent;

	u_size_t 		nodeVisited;
	u_size_t		valuesVisited;
	u_size_t		nodeFails;
	u_size_t		valuesFails;

	bool			lastVarOut;

public:

	o_fosearch_t(
			focusgraph_t& 		_pattern,
			focusgraph_t& 		_reference,
			o_domains_t&		_o_domains,
			o_predicate_t&		_o_predicates,
			o_gLists_t&			_o_glists,
			o_vars_order_t&		_o_varsorder,
			o_match_state_t&	_o_mstate,
			o_gsequence_t&		_o_sequence
			)
				: 	pattern		(_pattern),
					reference	(_reference),
					o_domains	(_o_domains),
					o_predicates	(_o_predicates),
					o_glists	(_o_glists),
					o_varsorder	(_o_varsorder),
					o_mstate	(_o_mstate),
					o_sequence	(_o_sequence)
	{
		//predecessor = new cardinal_t[pattern.nof_nodes];
		predecessor = (cardinal_t*)calloc(pattern.nof_nodes, sizeof(cardinal_t));
		//successor = new cardinal_t[(int)pattern.nof_nodes];
		successor = (cardinal_t*)calloc(pattern.nof_nodes, sizeof(cardinal_t));
		setsAt = new fut_rec_t*[pattern.nof_nodes];
		before = new adjVar_rec_t*[pattern.nof_nodes];
		after = new fut_rec_t*[pattern.nof_nodes];
		//lambda = new cardinal_t[pattern.nof_nodes];
		lambda = (cardinal_t*)calloc(pattern.nof_nodes, sizeof(cardinal_t));

		for(int i=0; i<pattern.nof_nodes;i++){
			predecessor[i] = MAXCARD;
			successor[i] = MAXCARD;
			//lambda[i] = MAXCARD;
			setsAt[i] = NULL;
			before[i] = NULL;
			after[i] = NULL;
		}

		nodeVisited = nodeFails = valuesVisited = valuesFails = 0;

		lastVarOut = false;
	}

	~o_fosearch_t(){
#ifdef DEBUG_DELETE
		std::cout<<"~o_fosearch_t>\n";
#endif
		//Dlist_rec_t *initd = NULL;
		//conclude(initd);

		delete [] lambda;
		delete [] predecessor;
		delete [] successor;

		conclude();

#ifdef DEBUG_DELETE
		std::cout<<"~o_fosearch_t<\n";
#endif
	}

	void print(){
		std::cout<<"#predecessor:\n\t";
		for(u_size_t i=0; i<pattern.nof_nodes; i++)
			std::cout<<"["<<i<<":"<<predecessor[i]<<"]";
		std::cout<<"\n";

		std::cout<<"#successor:\n\t";
		for(u_size_t i=0; i<pattern.nof_nodes; i++)
			std::cout<<"["<<i<<":"<<successor[i]<<"]";
		std::cout<<"\n";

		std::cout<<"#lambda:\n\t";
		for(u_size_t i=0; i<pattern.nof_nodes; i++)
			std::cout<<"["<<i<<":"<<lambda[i]<<"]";
		std::cout<<"\n";

		std::cout<<"#setsAt:\n\t";
		if(setsAt!=NULL){
			for(u_size_t i=0; i<pattern.nof_nodes; i++){
				std::cout<<"["<<i<<":";
				if(setsAt[i] == NULL)
					std::cout<<"NULL]";
				else
					setsAt[i]->print();std::cout<<"]";
			}
		}
		else std::cout<<"NULL";
		std::cout<<"\n";

		std::cout<<"#after:\n\t";
		if(after!=NULL){
			for(u_size_t i=0; i<pattern.nof_nodes; i++){
				std::cout<<"["<<i<<":";
				if(after[i] == NULL)
					std::cout<<"NULL]";
				else
					after[i]->print();std::cout<<"]";
			}
		}
		else std::cout<<"NULL";
		std::cout<<"\n";

		std::cout<<"#before:\n\t";
		if(before!=NULL){
			for(u_size_t i=0; i<pattern.nof_nodes; i++){
				std::cout<<"["<<i<<":";
				if(before[i] == NULL)
					std::cout<<"NULL]";
				else
					before[i]->print();std::cout<<"]";
			}
		}
		else std::cout<<"NULL";
		std::cout<<"\n";
	}


	void checkIsomorphism(){
#ifdef FS_CALLS
		std::cout<<">>fsearch::checkIsomorphism\n";
#endif

#ifdef PRINT_MATCH
		std::cout<<"{";
#endif

		for(u_size_t  i = 0; i < pattern.nof_nodes; i++){
			o_mstate.isomorphism[i] = *(o_mstate.valueAt[i]);

#ifdef PRINT_MATCH
			std::cout<<"("<<i<<","<<o_mstate.isomorphism[i]<<")";
#endif
		}
#ifdef PRINT_MATCH
		std::cout<<"}\n";
#endif
		o_mstate.isomorphisms ++;
		//M2: PROCEDURE checkIsomorphism(VAR isomorphisms: CARDINAL; VAR foundOriginal: BOOLEAN);
		//M2: VAR
		//M2: 		  isomorphism: cardPatternType;
		//M2: 		  i: CARDINAL;
		//M2: 		BEGIN
		//M2: 		  FOR i:= alphaUB TO 0 BY -1 DO isomorphism[i]:= valueAt[i]^ END;
		//M2: 		  IF NOT foundOriginal AND matchedOriginal(isomorphism) THEN foundOriginal:= TRUE END;
		//M2: 		  INC(isomorphisms);
		//M2: 		END checkIsomorphism;
#ifdef FS_CALLS
		std::cout<<"<fsearch::checkIsomorphism\n";
#endif
	}


	void reviewAfter();
	void initialise();
	//M2: PROCEDURE lastInstantiation(valuePtr: DlistPtrType; domain: domainType; VAR found: BOOLEAN);
	void lastInstantiation(
			Dlist_rec_t	*valuePtr,
			domain_t&	domain);
	//M2: PROCEDURE reduce(i, v: CARDINAL; VAR dD: domainArrayType; VAR allowed: domainType; VAR consistent: BOOLEAN);
	void reduce(
			cardinal_t	i,
			cardinal_t	v,
			domain_t	*dD,
			domain_t&	allowed);
	void fosearch();


	void free_initd(Dlist_rec_t *initd){
		Dlist_rec_t *p_i, *c_i;
		p_i = NULL;
		c_i = initd;
		while(c_i != NULL){
			if(p_i != NULL) delete p_i;
			p_i = c_i;
			c_i = c_i->nextValue;
		}
		if(p_i != NULL) delete p_i;
	}

private:
	void conclude(){
#ifdef DEBUG_DELETE
		std::cout<<"~fsearch.conclude\n";
#endif
		//valueAt
		//o_mstate.free_valueAt(pattern.nof_nodes);

		//setsAt
			//pastListPtr
		//after

		fut_rec_t *c_p, *p_p;
		adjVar_rec_t *c_a, *p_a;

		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			p_p = NULL;
			c_p = setsAt[i];
			while(c_p != NULL){
				//TODO
//				p_a = NULL;
//				c_a = c_p->pastListPtr;
//				while(c_a != NULL){
//					if(p_a != NULL) delete p_a;
//					p_a = c_a;
//					c_a = c_a->next;
//				}
//				if(p_a != NULL){
//					delete p_a;
//					p_a = NULL;
//				}
//
				if(p_p != NULL) delete p_p;
				p_p = c_p;
				c_p = c_p->nextFuture;
			}
			if(p_p != NULL){
				delete p_p;
				p_p = NULL;
			}

			p_p = NULL;
			c_p = after[i];
			while(c_p != NULL){

				if(p_p != NULL) delete p_p;
				p_p = c_p;
				c_p = c_p->nextFuture;
			}
			if(p_p != NULL){
				delete p_p;
				p_p = NULL;
			}
		}

#ifdef DEBUG_DELETE
		std::cout<<"<~fsearch.conclude\n";
#endif
	}

	//M2: PROCEDURE conclude;
	//M2: VAR
	//M2: futPtr, chopFutPtr: futPtrType;
	//M2: pastPtr, chopPastPtr: adjVarPtrType;
	//M2: ptr, chopPtr: DlistPtrType;
	//M2: i: CARDINAL;
	//M2: BEGIN
	//M2: FOR i:= 0 TO alphaUB DO
	//M2: DISPOSE(valueAt[i]); futPtr:= setsAt[i];
	//M2: WHILE futPtr # NIL DO
	//M2: 	  pastPtr:= futPtr^.pastListPtr;
	//M2: WHILE pastPtr # NIL DO
	//M2: chopPastPtr:= pastPtr; pastPtr:= pastPtr^.next; DISPOSE(chopPastPtr);
	//M2: END;
	//M2: chopFutPtr:= futPtr; futPtr:= futPtr^.nextFuture; DISPOSE(chopFutPtr);
	//M2: END;
	//M2: setsAt[i]:= NIL;
	//M2: futPtr:= after[i];
	//M2: 	WHILE futPtr # NIL DO
	//M2: chopFutPtr:= futPtr;  futPtr:= futPtr^.nextFuture; DISPOSE(chopFutPtr);
	//M2: END;
	//M2: END;
	//M2: ptr:= initialDlist;
	//M2: WHILE ptr # NIL DO chopPtr:= ptr; ptr:= ptr^.nextValue; DISPOSE(chopPtr) END;
	//M2: <* IF NOT SINGLE THEN *>
	//M2: IF NOT foundOriginal THEN
	//M2: meepLn('Original isomorphism NOT found');
	//M2: WrStr('   Original isomorphism NOT found'); WrLn; HALT
	//M2: END;
	//M2: <* END *>
	//M2: END conclude;

};


#endif /* FSEARCH_H_ */
