/*
 * fsearch.cpp
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



#include "fsearch.h"


void o_fosearch_t::reviewAfter(
		){
#ifdef FS_CALLS
	std::cout<<">>o_fosearch_t::reviewAfter\n";
#endif
//	PROCEDURE reviewAfter(VAR lambda: cardPatternType);
//	VAR
//	  afterPtr, revisedList, nextAfterPtr: futPtrType;
//	  pastPtr, iPastPtr: adjVarPtrType;
//	  i, iOrd: CARDINAL;
//	  foundPast: BOOLEAN;
	fut_rec_t *afterPtr, *revisedList, *nextAfterPtr;
	adjVar_rec_t *pastPtr, *iPastPtr;
	u_size_t i;
	cardinal_t iOrd;
	bool foundPast;

//	BEGIN
//		FOR i:= 0 TO alphaUB DO
	for(u_size_t i=0; i<pattern.nof_nodes; i++){
//			afterPtr:= after[i]; revisedList:= NIL;  iOrd:= ordOfVar[i];
		afterPtr = after[i];
		revisedList = NULL;
		iOrd = o_varsorder.ordOfVar[i];
//			WHILE afterPtr # NIL DO
		while(afterPtr != NULL){
//				nextAfterPtr:= afterPtr^.nextFuture;
			nextAfterPtr = afterPtr->nextFuture;
//					IF i = lambda[afterPtr^.iFuture] THEN foundPast:= FALSE
			if(i == lambda[afterPtr->iFuture]){
				foundPast = false;
			}
//					ELSE
			else{
//						pastPtr:= before[afterPtr^.iFuture];
				pastPtr = before[afterPtr->iFuture];
//						LOOP
				while(true){
//							IF pastPtr = NIL THEN foundPast:= FALSE; EXIT END;
					if(pastPtr == NULL){
						foundPast = false;
						break;
					}
//							IF pastPtr^.adjVar = i THEN iPastPtr:= pastPtr
					if(pastPtr->adjVar == i){
						iPastPtr = pastPtr;
					}
//							ELSIF ordOfVar[pastPtr^.adjVar] < iOrd THEN foundPast:= TRUE; EXIT
					else if(o_varsorder.ordOfVar[pastPtr->adjVar] < iOrd){
						foundPast = true;
						break;
//							END;
					}
//						pastPtr:= pastPtr^.next
					pastPtr = pastPtr->next;
//						END;
				}
//					END;
			}
//				IF foundPast THEN (* link into revised list *)
			if(foundPast ){
//					afterPtr^.pastListPtr:= iPastPtr;
//					afterPtr^.nextFuture:= revisedList; revisedList:= afterPtr
				afterPtr->pastListPtr = iPastPtr;
				afterPtr->nextFuture = revisedList;
				revisedList = afterPtr;
//				ELSE DISPOSE(afterPtr)
			}else{

				delete afterPtr;
				afterPtr = NULL;

//				END;
			}
//				afterPtr:= nextAfterPtr
			afterPtr = nextAfterPtr;
//			END;
		}
//			after[i]:= revisedList
			after[i] = revisedList;
//if(revisedList == NULL)std::cout<<"after["<<i<<"] = revisedList(NULL)\n";
//else std::cout<<"after["<<i<<"] = revisedList(!=NULL)\n";
//		END;
	}
//	END reviewAfter;
#ifdef FS_CALLS
	std::cout<<"<o_fosearch_t::reviewAfter\n";
#endif
};

//M2: PROCEDURE initialise;
void o_fosearch_t::initialise(){
#ifdef FS_CALLS
	std::cout<<">>o_fosearch_t::initialise\n";
#endif

//#ifdef FS_DOMAINS
//	print();
//#endif
	//M2: 	VAR
	//M2: 	  lambda: cardPatternType;
	//M2: 	  adjVarPtr, nextAdjVarPtr, pastList, ptr, prevPtr: adjVarPtrType;
	//M2: 	  futureList, futPtr, afterPtr: futPtrType;
	//M2: 	  i, ord, iOrd, highestOrd, adjacentVar, ordOfAdjVar, headVar: CARDINAL;
	//card_pattern_t lambda;
	adjVar_rec_t *adjVarPtr, *nextAdjVarPtr, *pastList,* ptr, *prevPtr;
	fut_rec_t *futureList, *futPtr, *afterPtr;
	cardinal_t i, ord, iOrd, highestOrd, adjacentVar, ordOfAdjVar, headVar;
	//int i, ord, iOrd, highestOrd, adjacentVar, ordOfAdjVar, headVar;

	//M2: 	BEGIN (* For directed arcs, ensure that in iMjPtr, i is always the first in sequence. *)
	//M2: 	  firstVar:= varAtOrd[0]; lastVar:= varAtOrd[alphaUB];
	//M2: 	  i:= varAtOrd[0]; predecessor[i]:= nil; successor[i]:= varAtOrd[1];
	firstVar = o_varsorder.varAtOrd[0];

	lastVar = o_varsorder.varAtOrd[pattern.nof_nodes - 1];

	i = o_varsorder.varAtOrd[0];
	predecessor[i] = MAXCARD;
	successor[i] = o_varsorder.varAtOrd[1];
	//M2: 	  FOR ord:= 1 TO alphaUB - 1 DO
	//M2: 	    i:= varAtOrd[ord];
	//M2: 	    predecessor[i]:= varAtOrd[ord - 1];  successor[i]:= varAtOrd[ord + 1];
	//M2: 	  END;
	for(ord = 1; ord < pattern.nof_nodes -1; ord++){
		i = o_varsorder.varAtOrd[ord];
		predecessor[i] = o_varsorder.varAtOrd[ord - 1];
		successor[i] = o_varsorder.varAtOrd[ord + 1];
	}
	//M2: 	  i:= varAtOrd[alphaUB]; predecessor[i]:= varAtOrd[alphaUB - 1]; successor[i]:= nil;
	//M2: 	  i:= 0;
	//M2: 	  WHILE Dcards[varAtOrd[i]] = 1 DO INC(i) END;
	//M2: 	  IF i > 0 THEN
	//M2: 	    firstVar:= varAtOrd[i]; predecessor[firstVar]:= nil;
	//M2: 	  END;
	//M2: 	  FOR i:= 0 TO alphaUB DO setsAt[i]:= NIL END;
	i = o_varsorder.varAtOrd[pattern.nof_nodes - 1];
	predecessor[i] = o_varsorder.varAtOrd[pattern.nof_nodes - 2];
	successor[i] = MAXCARD;
	i = 0;

	while(o_domains.Dcards[o_varsorder.varAtOrd[i]] == 1 && i<(pattern.nof_nodes)){
#ifdef FS_IINS
		std::cout<<"i:firstVar: "<<i<<"\n";
#endif
		i++;
	}
	if(i==pattern.nof_nodes){
		i--;
	}

	if(i > 0){
		firstVar = o_varsorder.varAtOrd[i];
		predecessor[firstVar] = MAXCARD;
	}
	for(i=0; i<pattern.nof_nodes; i++){
		setsAt[i] = NULL;
	}

	//M2: 	  FOR i:= 0 TO alphaUB DO
	//M2: 	    iOrd:= ordOfVar[i];
	//M2: 	    IF iOrd > 0 THEN
	for(i=0; i<pattern.nof_nodes; i++){

		iOrd = o_varsorder.ordOfVar[i];

#ifdef FS_IINS
		std::cout<<"i= "<<i<<"; iOrd= "<<iOrd<<"\n";
#endif

		if(iOrd > 0){
	//M2: 	      NEW(futPtr);  headVar:= nil;                                                   --ia
	//M2: 	      WITH futPtr^ DO iFuture:= i; nextFuture:= NIL END;
	//M2: 	      adjVarPtr:= adjVarInfo[i]; highestOrd:= 0;
	//M2: 	      pastList:= NIL;  futureList:= NIL;
			futPtr = new fut_rec_t();
			headVar = MAXCARD;
				futPtr->iFuture = i;
				futPtr->nextFuture = NULL;
			adjVarPtr = o_glists.adjVarInfo[i];
			highestOrd = 0;
			pastList = NULL;
			futureList = NULL;
	//M2: 	      WHILE adjVarPtr # NIL DO
	//M2: 	        WITH adjVarPtr^ DO
	//M2: 	          adjacentVar:= adjVar; ordOfAdjVar:= ordOfVar[adjacentVar];
	//M2: 		  	  valuePtr:= valueAt[adjacentVar]; nextAdjVarPtr:= next
	//M2: 		 	END;
			while(adjVarPtr != NULL){
				adjacentVar = adjVarPtr->adjVar;

#ifdef FS_IINS
				std::cout<<"adjVar= "<<adjacentVar<<"\n";
#endif
				ordOfAdjVar = o_varsorder.ordOfVar[adjacentVar];
				adjVarPtr->valuePtr = o_mstate.valueAt[adjacentVar];
				nextAdjVarPtr = adjVarPtr->next;
	//M2: 	        IF ordOfAdjVar < iOrd THEN
	//M2: 	          	IF ordOfAdjVar >= highestOrd THEN
	//M2: 		    		headVar:= adjacentVar; highestOrd:= ordOfAdjVar
	//M2: 		  	  	END;
	//M2: 		   	  	(* link into pastList in sequence of decreasing ord *)
	//M2: 		  		IF pastList = NIL THEN adjVarPtr^.next:= NIL; pastList:= adjVarPtr;
	//M2: 		  		ELSIF ordOfAdjVar > ordOfVar[pastList^.adjVar] THEN (* first in pastList *)
	//M2: 		     		adjVarPtr^.next:= pastList; pastList:= adjVarPtr;
	//M2: 		  		ELSE ptr:= pastList^.next; prevPtr:= pastList;
	//M2: 		    		LOOP
	//M2: 		      			IF (ptr = NIL) OR (ordOfAdjVar > ordOfVar[ptr^.adjVar])  THEN
	//M2: 		        		prevPtr^.next:= adjVarPtr; adjVarPtr^.next:= ptr;  EXIT
	//M2: 	   	      			END;
	//M2: 		      			prevPtr:= ptr; ptr:= ptr^.next
	//M2: 		    		END;
	//M2: 		  		END;
				if(ordOfAdjVar < iOrd){

#ifdef FS_IINS
					std::cout<<"ordOfAdjVar < iOrd : "<<ordOfAdjVar<<" < "<<iOrd<<"\n";
#endif
					if(ordOfAdjVar >= highestOrd){
						headVar = adjacentVar;
						highestOrd = ordOfAdjVar;
					}
					if(pastList == NULL){
						adjVarPtr->next = NULL;
						pastList = adjVarPtr;
					}
					else if(ordOfAdjVar > o_varsorder.ordOfVar[pastList->adjVar]){
						adjVarPtr->next = pastList;
						pastList = adjVarPtr;
					}
					else{
						ptr = pastList->next;
						prevPtr = pastList;
						while(true){
							if(ptr == NULL  || ( ordOfAdjVar > o_varsorder.ordOfVar[ptr->adjVar])){
								prevPtr->next = adjVarPtr;
								adjVarPtr->next = ptr;
								break;
							}
							prevPtr = ptr;
							ptr = ptr->next;
						}
					}
				}
	//M2: 			ELSE
	//M2: 		  		NEW(afterPtr); DISPOSE(adjVarPtr);
	//M2: 		  		WITH afterPtr^ DO iFuture:= adjacentVar; nextFuture:= futureList END;
	//M2: 		  		futureList:= afterPtr; (* link into futureList *)
	//M2: 	        END;
				else{
					afterPtr = new fut_rec_t();

					//delete adjVarPtr;

					afterPtr->iFuture = adjacentVar;
					afterPtr->nextFuture = futureList;
					futureList = afterPtr;
				}
	//M2: 	        adjVarPtr:= nextAdjVarPtr
				adjVarPtr = nextAdjVarPtr;
	//M2: 	      END;
			}
	//M2: 	      before[i]:= pastList;  after[i]:= futureList;
	//M2: 	      futPtr^.pastListPtr:= pastList;
	//M2: 	      (* now link futPtr into list of sets for headVar *)
	//M2: 	      IF headVar # nil THEN                                                             --ib
	//M2: 	        futPtr^.nextFuture:= setsAt[headVar]; setsAt[headVar]:= futPtr
	//M2: 	      END;
	//M2: 	      lambda[i]:= headVar;
			before[i]= pastList;
			after[i] = futureList;

//if(futureList == NULL)std::cout<<"after["<<i<<"] = futureList(NULL)\n";
//else std::cout<<"after["<<i<<"] = futureList(!=NULL)\n";

			futPtr->pastListPtr = pastList;
			if(headVar != MAXCARD){
				futPtr->nextFuture = setsAt[headVar];
				setsAt[headVar] = futPtr;
			}
			lambda[i] = headVar;
	//M2: 	    ELSE before[i]:= NIL; after[i]:= NIL;
	//M2: 	    END;
		}else{
			before[i] = NULL;
			after[i] = NULL;
//std::cout<<"after["<<i<<"] = NULL\n";
		}
	//M2: 	  END;
	}
	//M2: 	  FOR i:= 0 TO alphaUB DO dD[i]:= domainType{} END;
	//M2: 	  dD[firstVar]:= Dsets[firstVar]; reviewAfter(lambda); foundOriginal:= FALSE;
//	for(i=0; i<pattern.nof_nodes; i++){
//		dD[i] = new domain_t();
//	}
	dD[firstVar] = o_domains.Dsets[firstVar];
	//reviewAfter(lambda, pattern, o_varsorder.ordOfVar, after, before);

#ifdef FS_DOMAINS
	print();
#endif

	reviewAfter();

	foundOriginal = false;
	//M2: 	END initialise;
};


//M2: PROCEDURE lastInstantiation(valuePtr: DlistPtrType; domain: domainType; VAR found: BOOLEAN);
void o_fosearch_t::lastInstantiation(
		Dlist_rec_t	*valuePtr,
		domain_t&	domain
		){
#ifdef FS_CALLS
	std::cout<<">>o_fosearch_t::lastInstantiation\n";
#endif
	//M2:   WHILE valuePtr # NIL DO
	//M2: 		IF valuePtr^.value IN domain THEN
	//M2: 		   valueAt[lastVar]^:= valuePtr^.value; found:= TRUE;
	//M2: 		  (*  peepStr('FOUND: '); seeSolution; *)
	//M2: 		  <* IF SINGLE THEN *> foundOriginal:= TRUE; isomorphisms:= 1; RETURN;
	//M2: 		  <* ELSE *> checkIsomorphism(isomorphisms, foundOriginal);
	//M2: 		  <* END *>
	//M2: 		END;
	//M2: 		valuePtr:= valuePtr^.nextValue
	//M2: 	END; (* WHILE *)
	//M2: 	found:= FALSE;
	while(valuePtr != NULL){
		if(domain.get(valuePtr->value) ){
			*(o_mstate.valueAt[lastVar]) = valuePtr->value;
			found = true;
			checkIsomorphism();
		}
		valuePtr = valuePtr->nextValue;
	}
	found = false;
#ifdef FS_CALLS
	std::cout<<"<o_fosearch_t::lastInstatiation\n";
#endif
};



//M2: PROCEDURE reduce(i, v: CARDINAL; VAR dD: domainArrayType; VAR allowed: domainType; VAR consistent: BOOLEAN);
void o_fosearch_t::reduce(
		cardinal_t	i,
		cardinal_t	v,
		domain_t	*dD,
		domain_t&	allowed
		){
#ifdef FS_CALLS
	std::cout<<">>o_fosearch_t::reduce("<<i<<","<<v<<")\n";
#endif
	//M2: 	VAR
	//M2: 	  futPtr: futPtrType;
	//M2: 	  pastPtr, iPastPtr: adjVarPtrType;
	//M2: 	  jD: domainType;
	fut_rec_t *futPtr;
	adjVar_rec_t *pastPtr, *iPastPtr;
	domain_t jD;

	domain_t *pM, *ipM;


	//M2: 	BEGIN
	//M2: 	  futPtr:= after[i];
	futPtr = after[i];
	//M2: 	  WHILE futPtr# NIL DO
	//M2: 	    iPastPtr:=  futPtr^.pastListPtr; pastPtr:= iPastPtr^.next;
	while(futPtr != NULL){
		iPastPtr = futPtr->pastListPtr;
		pastPtr = iPastPtr->next;
	//M2: 	    WITH pastPtr^ DO
	//M2: 	      jD:= iPastPtr^.jMiPtr^[v] * jMiPtr^[valuePtr^] * allowed; pastPtr:= next         --ra
	//M2: 	    END;

//#ifdef FS_INS
//
//#endif


//		if(pastPtr->direction == ETYPE_N){
//			*pM = *(pastPtr->jMiPtr[*(pastPtr->valuePtr)]) & *(pastPtr->iMjPtr[*(pastPtr->valuePtr)]);
//		}
//		else if(pastPtr->direction == ETYPE_F){
//			pM = pastPtr->jMiPtr[*(pastPtr->valuePtr)];
//		}
//		else{
//			pM = pastPtr->iMjPtr[*(pastPtr->valuePtr)];
//		}
//
//		if(iPastPtr->direction == ETYPE_N){
//			*ipM = *(iPastPtr->jMiPtr[v]) & *(iPastPtr->iMjPtr[v]);
//		}
//		else if(iPastPtr->direction == ETYPE_F){
//			ipM = iPastPtr->jMiPtr[v];
//		}
//		else{
//			ipM = iPastPtr->iMjPtr[v];
//		}
//
//		jD = *pM & *ipM & allowed;

		//jD = *(iPastPtr->jMiPtr[v]) & *((pastPtr->jMiPtr)[*(pastPtr->valuePtr)]) & allowed;
		jD = *(iPastPtr->jMiPtr[v]);
		jD &= *((pastPtr->jMiPtr)[*(pastPtr->valuePtr)]);
		jD &= allowed;



#ifdef FS_IINS
		std::cout<<"after[i]: jD = *(iPastPtr->jMiPtr[v]) & *((pastPtr->jMiPtr)[*(pastPtr->valuePtr)]) & allowed;//TODO verify\n";
#endif


		pastPtr = pastPtr->next;
	//M2: 	    WHILE pastPtr # NIL DO  (* intersect bit vectors for all past variables *)
	//M2: 	      WITH pastPtr^ DO jD:= jD * jMiPtr^[valuePtr^]; pastPtr:= next END;
	//M2: 	    END;
		while(pastPtr != NULL){

//			if(pastPtr->direction == ETYPE_N){
//				*pM = *(pastPtr->jMiPtr[*(pastPtr->valuePtr)]) & *(pastPtr->iMjPtr[*(pastPtr->valuePtr)]);
//			}
//			else if(pastPtr->direction == ETYPE_F){
//				pM = pastPtr->jMiPtr[*(pastPtr->valuePtr)];
//			}
//			else{
//				std::cout<<"@\n";
//				pM = pastPtr->iMjPtr[*(pastPtr->valuePtr)];
//			}
//
//			jD &= *pM;

			jD &= *((pastPtr->jMiPtr)[*(pastPtr->valuePtr)]);

#ifdef FS_IINS
		std::cout<<"after[i]: jD &= *((pastPtr->jMiPtr)[*(pastPtr->valuePtr)]);\n";
#endif

			pastPtr = pastPtr->next;
		}
	//M2: 	    IF jD = domainType{} THEN consistent:= FALSE; RETURN END;                          --rb
	//M2: 	    futPtr:= futPtr^.nextFuture
		if(jD.is_empty()){
			consistent = false;
			return;
		}
		futPtr = futPtr->nextFuture;
	}
	//M2: 	  END;
	//M2: 	  futPtr:= setsAt[i];
	futPtr = setsAt[i];
	//M2: 	  WHILE futPtr# NIL DO  (* for each focus set do *)
	//M2: 	    pastPtr:= futPtr^.pastListPtr;
	//M2: 	    WITH pastPtr^ DO jD:= jMiPtr^[valuePtr^] * allowed; pastPtr:= next;  END;         --rc
	//M2: 	    WHILE pastPtr # NIL DO  (* intersect bit vectors for all past variables *)
	//M2: 	       WITH pastPtr^ DO jD:= jD * jMiPtr^[valuePtr^]; pastPtr:= next END;
	//M2: 	    END;
	//M2: 	    IF jD = domainType{} THEN consistent:= FALSE; RETURN END;                         --rd
	//M2: 	    dD[futPtr^.iFuture]:= jD;  futPtr:= futPtr^.nextFuture
	//M2: 	  END;
	while(futPtr != NULL){
		pastPtr = futPtr->pastListPtr;

//		if(pastPtr->direction == ETYPE_N){
//			*pM = *(pastPtr->jMiPtr[*(pastfree_valueAtPtr->valuePtr)]) & *(pastPtr->iMjPtr[*(pastPtr->valuePtr)]);
//		}
//		else if(pastPtr->direction == ETYPE_F){
//			pM = pastPtr->jMiPtr[*(pastPtr->valuePtr)];
//		}
//		else{
//			std::cout<<"@\n";
//			pM = pastPtr->iMjPtr[*(pastPtr->valuePtr)];
//		}
//
//		jD = *pM & allowed;

		//jD = *(pastPtr->jMiPtr[*(pastPtr->valuePtr)]) & allowed;
		jD = *(pastPtr->jMiPtr[*(pastPtr->valuePtr)]);
		jD &= allowed;


#ifdef FS_IINS
		std::cout<<"setsAs: jD = *(pastPtr->jMiPtr[*(pastPtr->valuePtr)]) & allowed;\n";
		std::cout<<"\tsetsAs: jD = *(pastPtr->jMiPtr["<<*(pastPtr->valuePtr)<<"]) & allowed;\n";
		//std::cout<<"\tpastPtr->node= "<<pastPtr->node<<"\n";
		std::cout<<"\tpastPtr->adjVar= "<<pastPtr->adjVar<<"\n";
		std::cout<<"\td[";(pastPtr->jMiPtr[*(pastPtr->valuePtr)])->print_numbers();std::cout<<"]\n";
#endif


		pastPtr = pastPtr->next;
		while(pastPtr != NULL){

//			if(pastPtr->direction == ETYPE_N){
//				*pM = *(pastPtr->jMiPtr[*(pastPtr->valuePtr)]) & *(pastPtr->iMjPtr[*(pastPtr->valuePtr)]);
//			}
//			else if(pastPtr->direction == ETYPE_F){
//				pM = pastPtr->jMiPtr[*(pastPtr->valuePtr)];
//			}
//			else{
//				pM = pastPtr->iMjPtr[*(pastPtr->valuePtr)];
//			}
//
//			jD &= *pM;


			jD &= *(pastPtr->jMiPtr[*(pastPtr->valuePtr)]);
#ifdef FS_IINS
		std::cout<<"setsAs: jD &= *(pastPtr->jMiPtr[*(pastPtr->valuePtr)]);\n";
		std::cout<<"\tsetsAs: jD &= *(pastPtr->jMiPtr["<<*(pastPtr->valuePtr)<<"]);\n";
		//std::cout<<"\tpastPtr->node= "<<pastPtr->node<<"\n";
		std::cout<<"\tpastPtr->adjVar= "<<pastPtr->adjVar<<"\n";
#endif


			pastPtr = pastPtr->next;
		}
		if(jD.is_empty()){
#ifdef FS_CALLS
	std::cout<<"<o_fosearch_t::reduce:false\n";
#endif
			consistent = false;
			return;
		}

#ifdef FS_IINS
		std::cout<<"dD["<<futPtr->iFuture<<"] = [";jD.print_numbers();std::cout<<"]\n";
#endif
		dD[futPtr->iFuture] = jD;
		futPtr= futPtr->nextFuture;
	}
	//M2: 	  consistent:= TRUE;
	consistent = true;
	//M2: 	END reduce;
#ifdef FS_CALLS
	std::cout<<"<o_fosearch_t::reduce:true\n";
#endif
};


void o_fosearch_t::fosearch(){
#ifdef FS_CALLS
	std::cout<<">>o_fosearch_t::fosearch\n";

	std::cout<< ((cardinal_t)(INT_MAX)) <<"\n";
	std::cout<< ((u_size_t)(INT_MAX)) <<"\n";
#endif
	//M2:	successor, predecessor: cardPatternType;
	//M2:	dD: domainArrayType;
	//M2:	domain: domainType;
	//M2:	setsAt: ARRAY[0..alphaUB] OF futPtrType;
	//M2:	next: ARRAY[0..alphaUB] OF DlistPtrType;
	//M2:	before: ARRAY[0..alphaUB] OF adjVarPtrType;
	//M2:	after:  ARRAY[0..alphaUB] OF futPtrType;
	//M2:	valuePtr, initialDlist: DlistPtrType;
	//M2:	i, firstVar, lastVar: CARDINAL;
	//M2:	consistent, found: BOOLEAN;
	//M2:	v: CARDINAL;

	//card_pattern_t successor, predecessor;
	//domain_array_t dD = new domain_t[pattern.nof_nodes];
	domain_t domain;
	Dlist_rec_t **next = new Dlist_rec_t*[pattern.nof_nodes];
	Dlist_rec_t *valuePtr, *initialDlist;
	//cardinal_t	i, firstVar, lastVar;
	//bool consistent, found;
	//cardinal_t v;
	cardinal_t i, v;

	//M2:	initialise; dD:= Dsets; nodesVisited:= 0; isomorphisms:= 0;

	//??????????????????????????????????????????

	dD = new domain_t[pattern.nof_nodes];

	initialise();

#ifdef FS_DOMAINS
	o_domains.print_domains(pattern.nof_nodes);
	print();
#endif


#ifdef FS_IINS
	std::cout<<"firstVar= "<<firstVar<<"\n";
	std::cout<<"lastVar= "<<lastVar<<"\n";
#endif
	dD = o_domains.Dsets;
	o_mstate.isomorphisms = 0;
	//M2:	i:= firstVar; initialDlist:= Dlists[i]; next[i]:= initialDlist;  Dlists[i]:= NIL;
	i = firstVar;
	initialDlist = o_domains.Dlists[i];
	next[i] = initialDlist;
	o_domains.Dlists[i] = NULL;
	//M2:	domain:= dD[i]; valuePtr:= initialDlist;                                             --fa
	domain = dD[i];
	valuePtr = initialDlist;
	//M2:	IF i = lastVar THEN
	//M2:	lastInstantiation(valuePtr, domain, found); conclude; RETURN
	//M2:	END;
	if(i == lastVar){
#ifdef FS_IINS
		std::cout<<"init_last_inst\n";
#endif
		lastInstantiation( valuePtr, domain);
		free_initd(initialDlist);
		delete [] next;

		return;
	}


	//o_domains.print_domains(pattern.nof_nodes);

	//M2:	LOOP
	while(true){
	//M2:	IF valuePtr # NIL THEN
	//M2:		IF valuePtr^.value IN domain THEN

#ifdef FS_INS
		std::cout<<"while\n";
		std::cout<<"domain[";domain.print_numbers();std::cout<<"]\n";
		std::cout<<"allowed[";o_mstate.allowed.print_numbers();std::cout<<"]\n";
#endif

		if(valuePtr != NULL){

			valuesVisited++;

			if(domain.get(valuePtr->value)){
	//M2:			v:= valuePtr^.value;  valueAt[i]^:= v; EXCL(allowed, v); INC(nodesVisited);    --fb
	//M2:			reduce(i, v, dD, allowed, consistent);
				v = valuePtr->value;

#ifdef FS_IINS
	std::cout<<"(i,v)("<<i<<","<<v<<")\n";
#endif

#ifdef FS_IINS
				std::cout<<"(i,v)("<<i<<","<<v<<")\n";
				//o_domains.print_domains(pattern.nof_nodes);
				std::cout<<"domain[";domain.print_numbers();std::cout<<"]\n";
				std::cout<<"allowed[";o_mstate.allowed.print_numbers();std::cout<<"]\n";
#endif

				*(o_mstate.valueAt[i]) = v;
				o_mstate.allowed.set(v, false);

				reduce(i, v, dD, o_mstate.allowed);

#ifdef FS_IINS
				std::cout<<"domain[";domain.print_numbers();std::cout<<"]\n";
				std::cout<<"allowed[";o_mstate.allowed.print_numbers();std::cout<<"]\n";
#endif

				//consistent = true;

				nodeVisited++;
	//M2:			IF consistent AND (dD[successor[i]] * allowed # domainType {}) THEN            --fc

				//if(consistent && !((dD[successor[i]] & o_mstate.allowed).is_empty()) ){
				if(consistent && !(dD[successor[i]].emptyAND(o_mstate.allowed)) ){
	//M2:	  			next[i]:= valuePtr^.nextValue;
	//M2:	  			i:= successor[i]; domain:= dD[i] * allowed;                                  --fd
	//M2:	  			valuePtr:= seLists[i, selector[i]^];

#ifdef FS_IINS
		std::cout<<"chosen::(i,v)("<<i<<","<<v<<")\n";
#endif
					next[i] = valuePtr->nextValue;
					i = successor[i];
					//domain = dD[i] & o_mstate.allowed;
					domain = dD[i];
					domain &= o_mstate.allowed;


					valuePtr = o_glists.seLists[i][*(o_mstate.selector[i])];
	//M2:	  			IF i = lastVar THEN
	//M2:					lastInstantiation(valuePtr, domain, found);
	//M2:					<* IF SINGLE THEN *> IF found THEN conclude; RETURN END; <* END *>
	//M2:					i:= predecessor[i]; INCL(allowed, v);                                      --fe
	//M2:					domain:= dD[i] * allowed; valuePtr:= next[i];                              --ff
	//M2:	  			END;
					if(i == lastVar){
#ifdef FS_IINS
		std::cout<<"cicle_last_inst\n";
#endif
						lastInstantiation( valuePtr, domain);
						//TODO if single return, delete next, free_initd(initialDlist);

						i = predecessor[i];
						o_mstate.allowed.set(v, true);
						//domain = dD[i] & o_mstate.allowed;
						domain = dD[i];
						domain &= o_mstate.allowed;

						valuePtr = next[i];
					}
	//M2:			ELSE
	//M2:	  			valuePtr:= valuePtr^.nextValue; INCL(allowed, v);                            --fg
	//M2:			END
				}else{
					valuePtr = valuePtr->nextValue;
					o_mstate.allowed.set(v, true);
					nodeFails++;
				}
	//M2:		ELSE valuePtr:= valuePtr^.nextValue
	//M2:		END;
			}else{
				valuePtr = valuePtr->nextValue;
				valuesFails++;
			}
	//M2:	ELSE
	//M2:	  i:= predecessor[i]; (* no values remain so backtrack *)
	//M2:	  IF i = nil THEN EXIT
	//M2:	  ELSE  INCL(allowed, valueAt[i]^);  domain:= dD[i] * allowed; valuePtr:= next[i]; --fh
	//M2:	  END;
	//M2:	END; (* IF valuePtr *)
		}else{
			i = predecessor[i];
			if(i == MAXCARD) break;
			else{
				o_mstate.allowed.set(*(o_mstate.valueAt[i]), true);
				//domain = dD[i] & o_mstate.allowed;
				domain = dD[i];
				domain &= o_mstate.allowed;


				valuePtr = next[i];
			}
		}
	//M2:	END;
	}


	free_initd(initialDlist);
	delete [] next;

	//M2:	conclude;
#ifdef FS_CALLS
	std::cout<<"<o_fosearch_t::fosearch\n";
#endif
};


