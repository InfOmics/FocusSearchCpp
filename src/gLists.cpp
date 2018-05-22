/*
 * gLists.cpp
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


//#include "defdebug.h"

//#define MSL_CALLS
//#define MSL_INSTRS
//#define MSL_CICLES
//#define MSL_INS
//#define MSL_DOMAINS

#include "gLists.h"

void o_gLists_t::mcSeLists(
		o_domains_t& 		o_domains,
		o_match_state_t& 	o_mstate,
		o_vars_order_t& 	o_varsorder
	){

#ifdef MSL_CALLS
	std::cout<<">>gLists::mcSeLists\n";
#endif

	//M2: 	VAR
	//M2: 	  iDomain, row: domainType;
	//M2: 	  matPtr: bitMatrixPointerType;
	//M2: 	  ptr: adjVarPtrType;
	//M2: 	  seListPtr, seList, iPtr, jPtr: DlistPtrType;
	//M2: 	  i, j, u, v, jOrd, firstVar, ordLastSingleton: CARDINAL;
	domain_t iDomain, row;
	bitMatrix_t *matPtr;
	adjVar_rec_t *ptr;
	Dlist_rec_t *seListPtr, *seList, *iPtr, *jPtr;
	cardinal_t i, j, u, v, jOrd, firstVar, ordLastSingleton;


	//M2: 	BEGIN
	//M2: 	  firstVar:= varAtOrd[0]; ordLastSingleton:= 0;                                       --a
	//M2: 	  IF Dcards[firstVar] = 1 THEN                                                        --b
	//M2: 	    valueAt[firstVar]^:= Dlists[firstVar]^.value; firstVar:= varAtOrd[1];
	//M2: 	  END;

#ifdef MSL_DOMAINS
	o_mstate.print_valueAt(pattern);
#endif
	firstVar = o_varsorder.varAtOrd[0];
	ordLastSingleton = 0;
	if(o_domains.Dcards[firstVar] == 1){
		*(o_mstate.valueAt[firstVar]) = o_domains.Dlists[firstVar]->value;
		firstVar = o_varsorder.varAtOrd[1];
	}
#ifdef MSL_DOMAINS
	o_mstate.print_valueAt(pattern);
#endif

	//M2: 	  FOR jOrd:= 1 TO alphaUB DO
	//M2: 	    j:= varAtOrd[jOrd];                                                               --c
	for(jOrd = 1; jOrd < pattern.nof_nodes; jOrd++){
		j = o_varsorder.varAtOrd[jOrd];

#ifdef MSL_CICLES
		std::cout<<".gLists::mcSeLists: j="<<j<<"\n";
#endif
	//M2: 	    IF Dcards[j] > 1 THEN
	//M2: 	      ptr:= adjVarInfo[j];                                                            --d
		if(o_domains.Dcards[j] > 1){

			ptr = adjVarInfo[j];

#ifdef MSL_INS
		std::cout<<"j= "<<j<<"; jOrd= "<<jOrd<<"; ";
		if(ptr == NULL)std::cout<<"adjVarInfo["<<j<<"] = ptr == NUL\n";
		else std::cout<<"adjVarInfo["<<j<<"] = ptr != NUL\n";
#endif
	//M2: 	      LOOP                                                                            --e
	//M2: 	        IF ptr = NIL THEN
	//M2: 		  		meepCard('No SeList for j = ', j, 4); meepCard(', jOrd = ', jOrd, 2);  peepLn; HALT
	//M2: 	        END;
	//M2: 			IF ordOfVar[ptr^.adjVar] < jOrd THEN i:= ptr^.adjVar; EXIT END;
	//M2: 			ptr:= ptr^.next;
	//M2: 	      END;
			while(true){
				if(ptr == NULL){
					std::cout<<"gLists: No SeLits for j="<<j<<"\n";
					exit(1);
				}
#ifdef MSL_INS
	std::cout<<"o_varsorder.ordOfVar["<<ptr->adjVar<<"] < jOrd\n";
	std::cout<<"\t=>"<<o_varsorder.ordOfVar[ptr->adjVar]<<" < "<<jOrd<<"\n";
#endif
				if(o_varsorder.ordOfVar[ptr->adjVar] < jOrd){
					i = ptr->adjVar;
					break;
				}
				ptr = ptr->next;
			}
	//M2: 	      selector[j]:= valueAt[i]; (* i is the selector for j *)
	//M2: 	      iDomain:= Dsets[i];
	//M2: 	      iPtr:= Dlists[i]; matPtr:= ptr^.jMiPtr;                                         --f
	//M2:
			o_mstate.selector[j] = o_mstate.valueAt[i];
			iDomain = o_domains.Dsets[i];
			iPtr = o_domains.Dlists[i];
			matPtr = (ptr->jMiPtr);
	//M2:		  WHILE iPtr # NIL DO                                                             --g
	//M2: 	        u:= iPtr^.value; row:= matPtr^[u]; jPtr:= Dlists[j]; seList:= NIL;  --h
	//M2: 			WHILE jPtr # NIL DO                                                           --i
	//M2: 		  		v:= jPtr^.value;
	//M2: 		  		IF v IN row THEN
	//M2: 		    		NEW(seListPtr);
	//M2: 		    		WITH seListPtr^ DO value:= v; nextValue:= seList END;
	//M2: 		    		seList:= seListPtr;
	//M2: 		  		END;
	//M2:			jPtr:= jPtr^.nextValue;
	//M2: 		  	END;
	//M2: 		  	seLists[j,u]:= seList; iPtr:= iPtr^.nextValue;
	//M2: 	      END;
			while(iPtr != NULL){
#ifdef MSL_INS
				std::cout<<".while.iPtr\n";
#endif
				u = iPtr->value;
				row = *((matPtr)[u]);
				jPtr = o_domains.Dlists[j];
				seList = NULL;
				while(jPtr != NULL){
#ifdef MSL_INS
				std::cout<<".while.jPtr\n";
#endif
					v = jPtr->value;
					if(row.get(v)){
#ifdef MSL_INS
				std::cout<<".row.get(v) == true\n";
#endif
						seListPtr = new Dlist_rec_t();
							seListPtr->value = v;
							seListPtr->nextValue = seList;
						seList = seListPtr;
					}
#ifdef MSL_INS
				std::cout<<"<.while.jPtr\n";
#endif
					jPtr = jPtr->nextValue;
				}

#ifdef MSL_INS
				std::cout<<"END.while.jPtr\n";
#endif

				seLists[j][u] = seList;
				iPtr = iPtr->nextValue;
			}
	//M2: 	    ELSE ordLastSingleton:= jOrd; valueAt[j]^:= Dlists[j]^.value;                     --j
	//M2: 	    END; (* IF Dcards > *)
		}else{
			ordLastSingleton = jOrd;
			*(o_mstate.valueAt[j]) = o_domains.Dlists[j]->value;
#ifdef MSL_DOMAINS
	o_mstate.print_valueAt(pattern);
#endif
		}
	//M2: 	  END; (* FOR j *)
	}


#ifdef MSL_INS
				std::cout<<"<.while\n";
#endif

#ifdef DL_INSTRCS
	std::cout<<"-gLists::mcSeLists:--\n";
#endif
	//M2: 	  IF ordLastSingleton > 0 THEN
	//M2: 	    firstVar:= varAtOrd[ordLastSingleton + 1];                                        --k
	//M2: 	    j:= varAtOrd[0]; valueAt[j]^:= Dlists[j]^.value;                                  --l
	//M2: 	  END;
	if(ordLastSingleton > 0){
		firstVar = o_varsorder.varAtOrd[ordLastSingleton + 1];
		j = o_varsorder.varAtOrd[0];
		*(o_mstate.valueAt[j]) = o_domains.Dlists[j]->value;
	}
	//M2: 	END mcSeLists;

#ifdef DL_CALLS
	std::cout<<"<gLists::mcSeLists\n";
#endif
};






void o_gLists_t::mcAdjVarInfo(o_predicate_t& o_predicates, bool isordered){
#ifdef DL_CALLS
	std::cout<<">>o_gLists_t::mcAdjVarInfo()\n";
#endif

	//M2:	VAR
	//M2:	  ptr: adjVarPtrType;
	//M2:	  g, i, k, first, second, otherVar: CARDINAL;
	//M2:	BEGIN
	//M2:	  degree:= cardPatternType{0 BY nAlpha};
	//M2:	  FOR g:= 1 TO eAlpha DO
	//M2:		FOR k:= 0 TO 1 DO
	//M2:		  first:= alphaEdges[g, 0];
	//M2:		  second:= alphaEdges[g, 1];
	//M2:		  NEW(ptr); ptr^.lineNo:= g;
	//M2:		  IF k = 0 THEN
	//M2:			i:= first;
	//M2:			ptr^.adjVar:= second;
	//M2:			ptr^.iMjPtr:= Rb1[g];
	//M2:			ptr^.jMiPtr:= Rb2[g];
	//M2:			INC(degree[first]); INC(degree[second]);
	//M2:		  ELSE
	//M2:			i:= second;
	//M2:			ptr^.adjVar:= first;
	//M2:			ptr^.iMjPtr:= Rb2[g];
	//M2:			ptr^.jMiPtr:= Rb1[g]
	//M2:		  END;
	//M2:		  ptr^.next:= adjVarInfo[i];
	//M2:		  adjVarInfo[i]:= ptr;
	//M2:		END;
	//M2:	  END;
	//M2:	END mcAdjVarInfo;

	//adjVarInfo = new adjVar_rec_t*[pattern.nof_nodes];//see gLists.h
//	init_degree(pattern);
//	init_adjVarInfo(pattern);
//
//
//	for(u_size_t i = 0; i<pattern.nof_nodes; i++)
//		adjVarInfo[i] = NULL;

	adjVar_rec_t *ptr;
	cardinal_t first, second, otherVar;

	for(u_size_t g = 0; g<pattern.nof_edges; g++){
		first = pattern.edges[g].source;
		second = pattern.edges[g].target;

		adjVar_rec_t *ptr = new adjVar_rec_t();
			ptr->lineNo = g;
			ptr->adjVar = second;
			ptr->iMjPtr = o_predicates.Rb1[g];
			ptr->jMiPtr = o_predicates.Rb2[g];

//			ptr->e = g;
//			ptr->iMjPtr_size = reference.nof_nodes;
//			ptr->node = first;

//			if(isordered){	ptr->direction = ETYPE_F;}
//			else{ ptr->direction = ETYPE_N;}

		ptr->next = adjVarInfo[first];
		adjVarInfo[first] = ptr;
//
		degree[first]++;
		degree[second]++;
//
		adjVar_rec_t *ptr2 = new adjVar_rec_t();
			ptr2->lineNo = g;
			ptr2->adjVar = first;
			ptr2->iMjPtr = o_predicates.Rb2[g];
			ptr2->jMiPtr = o_predicates.Rb1[g];

//			ptr2->e = g;
//			ptr2->iMjPtr_size = reference.nof_nodes;
//			ptr2->node = second;
//
//			if(isordered){	ptr->direction = ETYPE_B;}
//			else{ ptr->direction = ETYPE_N;}

		ptr2->next = adjVarInfo[second];
		adjVarInfo[second] = ptr2;
	}
#ifdef DL_CALLS
	std::cout<<"<o_gLists_t::mcAdjVarInfo()\n";
#endif
};


