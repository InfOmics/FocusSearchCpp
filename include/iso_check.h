/*
 * iso_check.h
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


#ifndef ISO_CHECK_H_
#define ISO_CHECK_H_

#include "match_state.h"
#include "gLists.h"
#include "gdex_graph.h"

void checkIsomorphism(focusgraph_t& pattern, o_domains_t& o_domains, o_match_state_t& o_mstate);
//
//	//M2: PROCEDURE checkIsomorphism;
//	//M2: VAR
//	//M2:   isomorphism: cardPatternType;
//	//M2:   i: CARDINAL;
//	//M2: BEGIN
//	//M2:   FOR i:= 0 TO alphaUB DO isomorphism[i]:= Dlists[i]^.value END;
//	//M2:   IF matchedOriginal(isomorphism) THEN RETURN END;
//	//M2:   meepLn('FAIL: prepro found the following isomorphism which did NOT match the original: ');
//	//M2:   FOR i:= 0 TO alphaUB DO
//	//M2: 	peepCard(i, 3); peepStr(':  '); seeChar(isomorphism[i]); peepLn;
//	//M2:   END;
//	//M2:   WrStr(' Failed because prepro found iso that did not match the original'); WrLn;
//	//M2:   HALT;
//	//M2: END checkIsomorphism;
//};

//M2: 	PROCEDURE matchedOriginal(VAR pattern: cardPatternType): BOOLEAN;
//M2: 	VAR
//M2: 	  i: CARDINAL;
//M2: 	BEGIN
//M2: 	  FOR i:= 0 TO alphaUB DO
//M2: 	    IF betaForAlpha[i] # VAL(CARDINAL, pattern[i]) THEN RETURN FALSE END;
//M2: 	  END;
//M2: 	  RETURN TRUE;
//M2: 	END matchedOriginal;

#endif /* ISO_CHECK_H_ */
