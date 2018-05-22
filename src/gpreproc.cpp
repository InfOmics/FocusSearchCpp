/*
 * gpreproc.cpp
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

#include "gpreproc.h"

#include <queue>

//#define PP_DL_CALLS
//#define PP_DL_AREAS
//#define PP_DL_INSTRCS
//#define PP_DL_CICLES

void preprocess(focusgraph_t& 		pattern,
				o_domains_t& 		o_domains,
				o_gLists_t&			o_glists,
				o_match_state_t&	o_mstate){

#ifdef PP_DL_CALLS
	std::cout<<">>preprocess\n";
#endif
	//M2:	TYPE
	//M2:	  QType = SET OF [0..alphaUB];
	//M2:	VAR
	//M2:	  matPtr: bitMatrixPointerType;
	//M2:	  otherVarPtr: adjVarPtrType;
	//M2:	  uPtr, prevPtr, chopPtr: DlistPtrType;
	//M2:	  Q: QType;
	//M2:	  B: domainType;
	//M2:	  i, j, k, u, initialTotalDcard, finalTotalDcard, dCard, nbInQ: CARDINAL;
	//M2:	  firstAdjacent: BOOLEAN;
		bitMatrix_t *matPtr, *matPtr2;
		adjVar_rec_t *otherVarPtr;
		Dlist_rec_t *uPtr, *prevPtr, *chopPtr;
		std::deque<cardinal_t> q;
		domain_t q_contains;
	//	domain_t b;
		//cardinal_t i, j, k, u, initialTotalDcard, finalTotalDCard, dCard;
		cardinal_t i, j, u, dCard;
		bool firstAdjacent;

	//M2:	BEGIN
	//M2:	  initialTotalDcard:= 0; qInitialise; Q:= QType{0..alphaUB};
	//M2:	  FOR i:= 0 TO alphaUB DO
	//M2:	    INC(initialTotalDcard, Dcards[i]);
	//M2:	    IF Dcards[i] = 0 THEN consistent:= FALSE; RETURN END;
	//M2:	    IF Dcards[i] = 1 THEN insertFirst(i) ELSE qInsert(i) END;
	//M2:	  END;
#ifdef PP_DL_AEREAS
		std::cout<<"@preprocess:init_queue\n";
#endif
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			//initialTotalDcard += Dcards[i];
			if(o_domains.Dcards[i] == 0){
				o_mstate.consistent = false;
#ifdef PP_DL_CALLS
	std::cout<<"<preprocess:false_1\n";
#endif
				return;
			}
			if(o_domains.Dcards[i] == 1){
#ifdef PP_DL_INSTRCS
	std::cout<<"prepocess:push_front["<<i<<"]\n";
#endif
				q.push_front(i);
			}
			else{
#ifdef PP_DL_INSTRCS
	std::cout<<"prepocess:push_back["<<i<<"]\n";
#endif
				q.push_back(i);
			}
			q_contains.set(i, true);
		}

#ifdef PP_DL_AREAS
		std::cout<<"@preprocess:function\n";
#endif
	//M2:	  nbInQ:= nAlpha; allowed:= domainType{0..betaUB};
	//	domain_t allowed;//from input

	//M2:	  REPEAT
	//M2:	    qRemove(j); EXCL(Q, j); DEC(nbInQ);
	//M2:	    otherVarPtr:= adjVarInfo[j];  dCard:= Dcards[j];
		while(! q.empty()){
			j = q.front(); q.pop_front(); q_contains.set(j, false);
#ifdef PP_DL_CICLES
	std::cout<<"prepocess:dequeued["<<j<<"]\n";
#endif
			otherVarPtr = o_glists.adjVarInfo[j];

			dCard = o_domains.Dcards[j];
	//M2:	    IF otherVarPtr = NIL THEN
	//M2:	      prevPtr:= NIL; uPtr:= Dlists[j];
	//M2:	      WHILE uPtr # NIL DO
	//M2:		  	u:= uPtr^.value;
	//M2:		  	IF u IN Dsets[j] THEN prevPtr:= uPtr; uPtr:= uPtr^.nextValue
	//M2:	        ELSE
	//M2:		    	chopPtr:= uPtr; uPtr:= uPtr^.nextValue;
	//M2:		    	IF prevPtr = NIL THEN Dlists[j]:= uPtr ELSE prevPtr^.nextValue:= uPtr END;
	//M2:		    	DISPOSE(chopPtr); DEC(dCard);
	//M2:		  	END;
	//M2:	      END;
			if(otherVarPtr == NULL){

#ifdef PP_DL_INSTRCS
	std::cout<<"IF\n";
#endif
				prevPtr = NULL;
				uPtr = o_domains.Dlists[j];
				while(uPtr != NULL){
					u = uPtr->value;
					if(o_domains.Dsets[j].get(u)){
						prevPtr = uPtr;
						uPtr = uPtr->nextValue;
					}
					else{
						chopPtr= uPtr;
						uPtr = uPtr->nextValue;
						if(prevPtr == NULL){
							o_domains.Dlists[j] = uPtr;
						}
						else{
							prevPtr->nextValue = uPtr;
						}
						delete chopPtr;

						dCard--;
					}
				}

#ifdef PP_DL_INSTRCS
	std::cout<<"<IF\n";
#endif
			}else{

#ifdef PP_DL_INSTRCS
	std::cout<<"ELSE\n";
#endif
	//M2:	    ELSE
	//M2:	      firstAdjacent:= TRUE;
				firstAdjacent = true;
	//M2:	      WHILE otherVarPtr # NIL DO
				while(otherVarPtr != NULL){
	//M2:	        i:= otherVarPtr^.adjVar;
	//M2:	        matPtr:= otherVarPtr^.iMjPtr;
	//M2:	        B:= domainType{};  prevPtr:= NIL; uPtr:= Dlists[j];

#ifdef PP_DL_INSTRCS
	std::cout<<"WHILE-0\n";
#endif

					i = otherVarPtr->adjVar;

					matPtr = otherVarPtr->iMjPtr;
					matPtr2 = otherVarPtr->jMiPtr;

					domain_t B;
					prevPtr = NULL;
					uPtr = o_domains.Dlists[j];

#ifdef PP_DL_INSTRCS
	std::cout<<"WHILE-1\n";
#endif
	//M2:	        IF firstAdjacent THEN
	//M2:	          	WHILE uPtr # NIL DO
	//M2:		    		u:= uPtr^.value;
	//M2:		    		IF u IN Dsets[j] THEN B:= B + matPtr^[u]; prevPtr:= uPtr; uPtr:= uPtr^.nextValue
	//M2:	            	ELSE
	//M2:		      			chopPtr:= uPtr; uPtr:= uPtr^.nextValue;
	//M2:		      			IF prevPtr = NIL THEN Dlists[j]:= uPtr ELSE prevPtr^.nextValue:= uPtr END;
	//M2:		      			DISPOSE(chopPtr); DEC(dCard);
	//M2:		    		END;
	//M2:	      		END;
	//M2:		  		firstAdjacent:= FALSE;

#ifdef PP_DL_INSTRCS
	std::cout<<"*>\n";

	std::cout<<"initB[";B.print_numbers();std::cout<<"\n";
#endif

					if(firstAdjacent){
						while(uPtr != NULL){
							u = uPtr->value;
							if(o_domains.Dsets[j].get(u)){
								//B |= *((matPtr)[u]);

//								if(otherVarPtr->direction == (ETYPE_N) ){
//									B |= *((matPtr)[u]);
//									B |= *((matPtr2)[u]);
//								}
//								else if(otherVarPtr->direction == (ETYPE_F)){
//									B |= *((matPtr)[u]);
//								}
//								else{
//									B |= *((matPtr2)[u]);
//								}

								B |= *((matPtr)[u]);

#ifdef PP_DL_INSTRCS
								std::cout<<"@u["<<u<<"]";(matPtr)[u]->print_numbers();std::cout<<"\n";
#endif
								//B |= *((matPtr2)[u]);


								prevPtr = uPtr;
								uPtr = uPtr->nextValue;
							}
							else{
								chopPtr = uPtr;
								uPtr = uPtr->nextValue;
								if(prevPtr == NULL){
									o_domains.Dlists[j] = uPtr;
								}
								else{
									prevPtr->nextValue = uPtr;
								}
								delete chopPtr;

								dCard--;
							}
						}
						firstAdjacent = false;
					}else{
	//M2:	        ELSE
	//M2:		  		WHILE uPtr # NIL DO B:= B + matPtr^[uPtr^.value]; uPtr:= uPtr^.nextValue END;
	//M2:	        END;
						while(uPtr != NULL){
							//B |= *(matPtr)[uPtr->value];

							//B |= *(matPtr[uPtr->value]);
//							if(otherVarPtr->direction == (ETYPE_N) ){
//								B |= *((matPtr)[uPtr->value]);
//								B |= *((matPtr2)[uPtr->value]);
//							}
//							else if(otherVarPtr->direction == (ETYPE_F)){
//								B |= *((matPtr)[uPtr->value]);
//							}
//							else{
//								B |= *((matPtr2)[uPtr->value]);
//							}

							B |= *((matPtr)[uPtr->value]);
#ifdef PP_DL_INSTRCS
							std::cout<<"B[";B.print_numbers();std::cout<<"\n";
							std::cout<<"@uptrValue["<<uPtr->value<<"]";(matPtr)[uPtr->value]->print_numbers();std::cout<<"\n";
							std::cout<<"B[";B.print_numbers();std::cout<<"\n";
#endif
							//B |= *((matPtr2)[u]);


							uPtr = uPtr->nextValue;
						}
					}
#ifdef PP_DL_INSTRCS
		std::cout<<"*<\n";
					std::cout<<"-\n";
#endif
#ifdef PP_DL_INSTRCS
	std::cout<<"WHILE-2\n";
#endif

	//M2:	        B:=  Dsets[i] * B;
	//M2:	        IF Dsets[i] # B THEN
	//M2:		  		IF B = domainType{} THEN consistent:= FALSE;  RETURN END;
	//M2:		  		Dsets[i]:= B;
	//M2:		  		IF NOT (i IN Q) THEN qInsert(i); INCL(Q, i); INC(nbInQ) END;
	//M2:	        END; (* IF Dsets[i] # *)
	//M2:	        otherVarPtr:= otherVarPtr^.next
					B &= o_domains.Dsets[i];

#ifdef PP_DL_INSTRCS
					std::cout<<"&=B[";B.print_numbers();std::cout<<"\n";
#endif

					if(o_domains.Dsets[i] != B){
						if(B.is_empty()){
							o_mstate.consistent = false;
#ifdef PP_DL_CALLS
	std::cout<<"<prepro:false\n";
#endif
							return;
						}
						o_domains.Dsets[i] = B;


#ifdef PP_DL_INSTRCS
						std::cout<<"_______set o_domains.Dsets["<<i<<"]= ";B.print_numbers();std::cout<<"\n";
#endif


						if(! q_contains.get(i)){
#ifdef PP_DL_INSTRCS
		std::cout<<"prepro:push_back("<<i<<")\n";
#endif
							q.push_back(i);
							q_contains.set(i, true);
						}
					}

#ifdef PP_DL_INSTRCS
	std::cout<<"WHILE-3\n";
#endif
					otherVarPtr = otherVarPtr->next;

#ifdef PP_DL_INSTRCS
	std::cout<<"WHILE-4\n";
	if(otherVarPtr == NULL) std::cout<<"otherVarPtr == NULL\n";
	else std::cout<<"otherVarPtr != NULL\n";
	std::cout<<"B[";B.print_numbers();std::cout<<"]\n";
#endif
	//M2:	      END; (* WHILE *)
				}
	//M2:	    END;
#ifdef PP_DL_INSTRCS
	std::cout<<"<ELSE\n";
#endif
			}
	//M2:	    Dcards[j]:= dCard;
	//M2:	    IF dCard = 1 THEN  (* enforce allDifferent *)
	//M2:	      u:= Dlists[j]^.value; (* The single value in D_j *)
	//M2:	      EXCL(allowed, u);
	//M2:	      FOR k:= 0 TO alphaUB DO
	//M2:			IF (k # j) AND (u IN Dsets[k]) THEN
	//M2:		  		EXCL(Dsets[k], u);
	//M2:		  		IF Dsets[k] = domainType{} THEN consistent:= FALSE; RETURN END;
	//M2:		  		IF NOT (k IN Q) THEN qInsert(k); INCL(Q, k); INC(nbInQ) END;
	//M2:			END;
	//M2:	      END;
	//M2:	    END;
			o_domains.Dcards[j] = dCard;

#ifdef DR_DOMAINS
	o_domains.print_domains(pattern.nof_nodes);
#endif
#ifdef PP_DL_AREAS
	std::cout<<"@prepro:enforcing_alldiff\n";
#endif
			if(dCard == 1){
				u = o_domains.Dlists[j]->value;
				o_mstate.allowed.set(u, false);
				for(cardinal_t k=0; k<pattern.nof_nodes; k++){
					if(k!=j &&  o_domains.Dsets[k].get(u)){
						o_domains.Dsets[k].set(u, false);
						if(o_domains.Dsets[k].is_empty()){
							o_mstate.consistent = false;
							return;
						}
						if(!( q_contains.get(k) )){
#ifdef PP_DL_INSTRCS
		std::cout<<"prepro:push_back("<<i<<")\n";
#endif
							q.push_back(k);
							q_contains.set(k, true);
						}
					}
				}
			}
#ifdef DR_DOMAINS
	o_domains.print_domains(pattern.nof_nodes);
#endif
	//M2:	  UNTIL nbInQ = 0;
		}
		o_mstate.consistent = true;

#ifdef PP_DL_CALLS
	std::cout<<"<prepro:true\n";
#endif
	//M2:	  consistent:= TRUE; finalTotalDcard:= Dcards[0];
	//M2:	  FOR i:= 1 TO alphaUB DO INC(finalTotalDcard, Dcards[i]) END;
	//M2:	END preprocess;
};


