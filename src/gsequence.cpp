/*
 * gsequence.cpp
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

#include "gsequence.h"


void o_gsequence_t::mcRecords(_iternal_s& _i, cardinal_t	*branches){
#ifdef GSEQ_CALLS
	std::cout<<">>o_gsequence_t::mcRecords\n";
#endif
	//M2:
	//M2: VAR
	//M2: 	  adjVarPtr: adjVarPtrType;
	//M2: 	  ptr, headPtr: vPtrType;
	//M2: 	  i, score: CARDINAL;
	adjVar_rec_t *adjVarPtr;
	vRec_t *ptr, *headPtr;
	cardinal_t i, score;
	//M2: 	BEGIN
	//M2: 	  FOR i:= 0 TO maxRandomOverlap DO (* make dummy records *)
	//M2: 	    NEW(ptr); WITH ptr^ DO next:= ptr; previous:= ptr; varNo:= dummy END;
	//M2: 	    overlapLists[i]:= ptr;
	//M2: 	  END;
	for(cardinal_t i=0; i<maxRandomOverlap; i++){
		ptr = new vRec_t();
			ptr->next = ptr;
			ptr->previuos = ptr;
			ptr->varNo = DUMMY;
		_i.overlapLists[i] = ptr;
#ifdef DL_INSTRCS
		std::cout<<"g_seq: declared  _i.overlapList["<<i<<"]\n";
#endif
	}
	//M2: 	  headPtr:= overlapLists[0];
	headPtr = _i.overlapLists[0];
	//M2: 	  FOR i:= 0 TO alphaUB DO (* create one record for each variable *)
	for(u_size_t i=0; i<pattern.nof_nodes; i++){
	//M2: 	    IF Dcards[i] > 1 THEN
	//M2: 	      NEW(ptr);
	//M2: 	      WITH ptr^ DO
	//M2: 	        adjacentListPtr:= adjVarInfo[i]; varNo:= i; nbAdjacentCovered:= 0; covered:= FALSE;
	//M2: 	      END;
	//M2: 	      ptr^.next:= headPtr^.next; (* Link this record into overlapLists[0] *)
	//M2: 	      headPtr^.next:= ptr; ptr^.previous:= headPtr;
	//M2: 	      ptr^.next^.previous:= ptr;
	//M2: 	      vIndex[i]:= ptr;
		if(o_domains.Dcards[i] > 1){
			ptr = new vRec_t();
				ptr->adjacentListPtr = o_glists.adjVarInfo[i];
				ptr->varNo = i;
				ptr->nbAdjacentCovered = 0;
				ptr->covered = false;
			ptr->next = headPtr->next;
			headPtr->next = ptr;
			ptr->previuos = headPtr;
			ptr->next->previuos = ptr;
			_i.vIndex[i] = ptr;
#ifdef DL_INSTRCS
		std::cout<<"g_seq: declared  _i.vindex["<<i<<"]\n";
#endif
		}
	//M2: 	    ELSE vIndex[i]:= NIL
	//M2: 	    END;
		else{
			_i.vIndex[i] = NULL;
		}
	//M2: 	    adjVarPtr:= adjVarInfo[i]; score:= 0;
	//M2: 	    WHILE adjVarPtr # NIL DO
	//M2: 	      score:= score + degree[adjVarPtr^.adjVar]; adjVarPtr:= adjVarPtr^.next
	//M2: 	    END;
	//M2: 	    branches[i]:= score
		adjVarPtr = o_glists.adjVarInfo[i];
		score = 0;
		while(adjVarPtr != NULL){
			score += o_glists.degree[adjVarPtr->adjVar];
			adjVarPtr = adjVarPtr->next;
		}
		branches[i] = score;
	//M2: 	  END;
	}
	//M2: 	  FOR i:= 0 TO alphaUB DO  (* initialise dbranches *)
	//M2: 	    IF Dcards[i] > 1 THEN
	//M2: 	      adjVarPtr:= adjVarInfo[i]; score:= 0;
	//M2: 	      WHILE adjVarPtr # NIL DO
	//M2: 	        score:= score + branches[adjVarPtr^.adjVar]; adjVarPtr:= adjVarPtr^.next
	//M2: 	      END;
	//M2: 	      vIndex[i]^.dbranches:= score;
	//M2: 	    END;
	//M2: 	  END;
	for(cardinal_t i=0; i<pattern.nof_nodes; i++){
		if(o_domains.Dcards[i] > 1){
			adjVarPtr = o_glists.adjVarInfo[i];
			score = 0;
			while(adjVarPtr != NULL){
				score += branches[adjVarPtr->adjVar];
				adjVarPtr = adjVarPtr->next;
			}
			_i.vIndex[i]->dbranches = score;
		}
	}
	//M2: 	END mcRecords;
#ifdef GSEQ_CALLS
	std::cout<<"<o_gsequence_t::mcRecords\n";
#endif
};

void o_gsequence_t::updateCover(_iternal_s& _i){
#ifdef GSEQ_CALLS
	std::cout<<">>o_gsequence_t::updateCover\n";
#endif
	//M2:	VAR
	//M2:	   adjVarPtr: adjVarPtrType;
	//M2:	   jPtr, headPtr: vPtrType;
	//M2:	   j, nbCovered: CARDINAL;
	//M2:	BEGIN
	//M2:	   adjVarPtr:= adjVarInfo[iChosen];
#ifdef DL_INSTRCS
	std::cout<<"gsequence::updatecover::trying to access o_glists.adjVarInfo["<<_i.iChosen<<"]\n";
#endif
	adjVar_rec_t *adjVarPtr = o_glists.adjVarInfo[_i.iChosen];
#ifdef DL_INSTRCS
	std::cout<<"gsequence::updatecover::i1.1\n";
#endif
	vRec_t *jPtr, *headPtr;
	cardinal_t j, nbCovered;

	//vIndex_t vIndex = new v_ptr_t[pattern.nof_nodes];

	//M2:	   WHILE adjVarPtr # NIL DO
	//M2:	     j:= adjVarPtr^.adjVar; jPtr:= vIndex[j];
	//M2:	     IF (jPtr # NIL) AND (NOT jPtr^.covered) THEN
	//M2:	       nbCovered:= jPtr^.nbAdjacentCovered + 1; jPtr^.nbAdjacentCovered:= nbCovered;
	//M2:	       IF nbCovered > maxOverlap THEN maxOverlap:= nbCovered END;
	//M2:	       (* now unlink from list *)
	//M2:	       jPtr^.previous^.next:=jPtr^.next; jPtr^.next^.previous:= jPtr^.previous;
	//M2:	       (* now link into  overlapLists[nbCovered] *)
	//M2:	       headPtr:= overlapLists[nbCovered]; (* points to dummy record *)
	//M2:	       jPtr^.next:= headPtr^.next;
	//M2:	       headPtr^.next:= jPtr; jPtr^.previous:= headPtr; jPtr^.next^.previous:= jPtr;
	//M2:	     END;
	//M2:	     adjVarPtr:= adjVarPtr^.next (* for next variable adjacent to iChosen *)
	//M2:	   END;
	while(adjVarPtr != NULL){

		j = adjVarPtr->adjVar;
		jPtr = _i.vIndex[j];


		if(jPtr != NULL  &&  (!( jPtr->covered ))){

			nbCovered = jPtr->nbAdjacentCovered + 1;
			jPtr->nbAdjacentCovered = nbCovered;
			if(nbCovered > _i.maxOverlap){
				_i.maxOverlap = nbCovered;
			}
			jPtr->previuos->next = jPtr->next;
			jPtr->next->previuos = jPtr->previuos;
			headPtr = _i.overlapLists[nbCovered];

//			if(headPtr!=NULL && jPtr!=NULL){
				jPtr->next = headPtr->next;
				headPtr->next = jPtr;
				jPtr->previuos = headPtr;
				jPtr->next->previuos = jPtr;
//			}
//			else{
//				jPtr->next = NULL;
//			}

		}

		adjVarPtr = adjVarPtr->next;

	}
	//M2:	END updateCover;
#ifdef GSEQ_CALLS
	std::cout<<"<o_gsequence_t::updateCover\n";
#endif
};

void o_gsequence_t::selectNextVar(_iternal_s& _i){
#ifdef GSEQ_CALLS
	std::cout<<">>o_gsequence_t::selectNextVar\n";
#endif
	//M2: 	VAR
	//M2: 	  vPtr, stopPtr: vPtrType;
	//M2: 	  nbCovered,  bestScore: CARDINAL;
	//M2: 	BEGIN
	//M2: 	  nbCovered:= maxOverlap; (* Find biggest overlap for which the list is non-empty *)
	vRec_t *vPtr, *stopPtr;
	cardinal_t nbCovered = _i.maxOverlap;
	cardinal_t bestScore;
	//M2: 	  WHILE overlapLists[nbCovered]^.next = overlapLists[nbCovered] DO DEC(nbCovered) END;
	//M2: 	  maxOverlap:= nbCovered;maxRandomOverlap
	//M2: 	  (* now select iChosen from overlapLists[nbCovered] *)
	//M2: 	  stopPtr:= overlapLists[nbCovered]; (* points to dummy record *)
	//M2: 	  vPtr:= stopPtr^.next;  bestScore:= 0;
	//while(_i.overlapLists[nbCovered]->next == _i.overlapLists[nbCovered]){
#ifdef GSEQ_CALLS
	std::cout<<">>o_gsequence_t::selectNextVar- ---\n";
	std::cout<<"nbCovered = "<<nbCovered<<"\n";
	std::cout<<"maxOverlap = "<<_i.maxOverlap<<"\n";
	std::cout<<"maxRandomOverlap = "<<maxRandomOverlap<<"\n";

#endif
	while((_i.overlapLists[nbCovered])->next == _i.overlapLists[nbCovered]){
		if(nbCovered == 0)
			break;
		nbCovered--;
	}

#ifdef GSEQ_CALLS
	std::cout<<">>o_gsequence_t::selectNextVar- ---\n";
#endif

//	while(true){
//#ifdef GSEQ_CALLS
//		std::cout<<".seg.while\n";
//		std::cout<<"nbCovered = "<<nbCovered<<"\n";
//		std::cout<<"maxOverlap = "<<_i.maxOverlap<<"\n";
//		std::cout<<"maxRandomOverlap = "<<maxRandomOverlap<<"\n";
//		if(_i.overlapLists[nbCovered] != NULL)  std::cout<<"\t _i.overlapLists[nbCovered] != NULL\n";
//		else  std::cout<<"\t _i.overlapLists[nbCovered] == NULL\n";
//#endif
//		if(_i.overlapLists[nbCovered] != NULL){
//#ifdef GSEQ_CALLS
//		if(_i.overlapLists[nbCovered]->next != NULL)  std::cout<<"\t _i.overlapLists[nbCovered]->next != NULL\n";
//		else  std::cout<<"\t _i.overlapLists[nbCovered]->next == NULL\n";
//#endif
//			if(_i.overlapLists[nbCovered]->next != NULL){
//				if((_i.overlapLists[nbCovered])->next == _i.overlapLists[nbCovered]){
//					nbCovered--;
//				}
//				else
//					break;
//			}
//			else
//				break;
//		}
//		else
//			break;
//	}

#ifdef GSEQ_CALLS
	std::cout<<">>o_gsequence_t::selectNextVar-- ---\n";
	std::cout<<"nbCovered = "<<nbCovered<<"\n";
		std::cout<<"maxOverlap = "<<_i.maxOverlap<<"\n";
		std::cout<<"maxRandomOverlap = "<<maxRandomOverlap<<"\n";
#endif
	_i.maxOverlap = nbCovered;
	stopPtr = _i.overlapLists[nbCovered];
	vPtr = stopPtr->next;
	bestScore = 0;


#ifdef GSEQ_CALLS
	std::cout<<">>o_gsequence_t::selectNextVar-- ---\n";
	if(vPtr == NULL) std::cout<<"vPtr == NULL\n";
	else std::cout<<"vPtr!=NULL\n";
#endif
	//M2: 	  WHILE vPtr # stopPtr DO
	//M2: 	    WITH vPtr^ DO
	//M2: 	      IF (dbranches > bestScore) THEN
	//M2: 	        bestScore:= dbranches; iChosen:= varNo
	//M2: 	      END;
	//M2: 	      vPtr:= vPtr^.next
	//M2: 	    END;
	//M2: 	  END;
	//M2: 	  (* now unlink iChosen from the list that included it *)
	//M2: 	  vPtr:= vIndex[iChosen];  vPtr^.covered:= TRUE;
	//M2: 	  vPtr^.previous^.next:=vPtr^.next; vPtr^.next^.previous:= vPtr^.previous;
	while(vPtr != stopPtr){
#ifdef GSEQ_CALLS
	std::cout<<".o_gsequence_t.while..\n";
	if(vPtr == NULL) std::cout<<"vPtr == NULL\n";
	else std::cout<<"vPtr!=NULL\n";
#endif
//		adjVar_ptr_t adjacentListPtr;
//		cardinal_t varNo, degree, dbranches, nbAdjacentCovered;
//		vRec_t *previuos, *next;
//		bool covered;
		if(vPtr->dbranches > bestScore){
			bestScore = vPtr->dbranches;
			_i.iChosen = vPtr->varNo;
		}
		vPtr= vPtr->next;
	}

#ifdef GSEQ_CALLS
	std::cout<<">>o_gsequence_t::selectNextVar-- ---\n";
	if(vPtr == NULL) std::cout<<"vPtr == NULL\n";
	else{
		std::cout<<"vPtr!=NULL\n";
		std::cout<<"covered= "<<vPtr->covered<<"\n";
	}
	if(vPtr->previuos == NULL) std::cout<<"vPtr->prev == NULL\n";
	else std::cout<<"vPtr->prev!=NULL\n";
	if(vPtr->next == NULL) std::cout<<"vPtr->next == NULL\n";
	else std::cout<<"vPtr->next!=NULL\n";
	std::cout<<"iChosen = "<<_i.iChosen<<"\n";
#endif
	vPtr = _i.vIndex[_i.iChosen];
	if(vPtr!=NULL){
		vPtr->covered = true;
		vPtr->previuos->next = vPtr->next;
		vPtr->next->previuos = vPtr->previuos;
	}
	//M2: 	END selectNextVar;
#ifdef GSEQ_CALLS
	std::cout<<"<o_gsequence_t::selectNextVar\n";
#endif
};

void o_gsequence_t::initialise(_iternal_s& _i){
#ifdef GSEQ_CALLS
	std::cout<<">>o_gsequence_t::initialise\n";
#endif

	//	varAtOrd = new cardinal_t[pattern.nof_nodes];
	//	ordOfVar = new cardinal_t[pattern.nof_nodes];

	//	vIndex_t vIndex = new v_ptr_t[pattern.nof_nodes];
	//	v_ptr_t* overlapLists = new v_ptr_t[maxRandomOverlap(pattern)];
	cardinal_t *branches = new cardinal_t[pattern.nof_nodes];


	//M2: VAR
	//M2: 	  i, bestScore, singletonCount: CARDINAL;
	//M2: 	BEGIN
	//M2: 	  mcRecords;
	//mcRecords(pattern, vIndex, overlapLists, adjVarInfo, degree, branches, Dcards);

	mcRecords(_i, branches);


#ifdef GSEQ_I_INS
		for(cardinal_t i=0; i<maxRandomOverlap; i++){
			std::cout<<"_i.overlapLists["<<i<<"]= "<<_i.overlapLists[i]<<"\n";
		}
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			std::cout<<"_i.vIndex["<<i<<"]= "<<_i.vIndex[i]<<"\n";
		}
#endif
	//M2:	  bestScore:= 0;  nextOrd:= 0; singletonCount:= 0;
	cardinal_t mcRecords = 0, bestScore = 0, singletonCount = 0;
	_i.nextOrd = 0;
	//M2: 	  FOR i:= 0 TO alphaUB DO
	//M2: 	    IF Dcards[i] = 1 THEN
	//M2: 			varAtOrd[nextOrd]:= i; ordOfVar[i]:= nextOrd; updateCover(i); INC(nextOrd);
	//M2: 			INC(singletonCount);
	//M2: 	    END;
	//M2: 	  END;
	for(u_size_t i=0; i<pattern.nof_nodes; i++){
#ifdef GSEQ_I_INS
		std::cout<<"gseq: Dcards["<<i<<"]= "<<o_domains.Dcards[i]<<"\n";
#endif
		if(o_domains.Dcards[i] == 1){
			o_varsorder.varAtOrd[_i.nextOrd] = i;
			o_varsorder.ordOfVar[i] = _i.nextOrd;
			//updateCover(i, adjVarInfo, pattern, vIndex, maxOverlap, overlapLists);
			updateCover(_i);
			(_i.nextOrd)++;
			singletonCount++;
		}
	}
	//M2: 	  IF singletonCount = nAlpha THEN terminal:= TRUE; RETURN ELSE terminal:= FALSE END;
	//M2: 	  selectNextVar(iChosen);
	//M2: 	  varAtOrd[nextOrd]:= iChosen; ordOfVar[iChosen]:= nextOrd;  INC(nextOrd);
	if(singletonCount == pattern.nof_nodes){
		o_mstate.terminal = true;
		return;
	}
	else{
		o_mstate.terminal = false;
	}
	selectNextVar(_i);

	o_varsorder.varAtOrd[_i.nextOrd] = _i.iChosen;
	o_varsorder.ordOfVar[_i.iChosen] = _i.nextOrd;
	(_i.nextOrd)++;
#ifdef GSEQ_CALLS
	std::cout<<"<o_gsequence_t::initialise\n";
#endif
};

