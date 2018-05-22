/*
 * AttributeComparator.h
 *
 *  Created on: Dec 14, 2011
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


#ifndef ATTRIBUTECOMPARATOR_H_
#define ATTRIBUTECOMPARATOR_H_

#include <string>

#include "debug.h"

using namespace std;

class AttributeComparator{
public:
	virtual bool compare(void* attr1, void* attr2)=0;
	virtual int compareint(void* attr1, void* attr2)=0;

	virtual bool lessThan(void* attr1, void* attr2)=0;
	virtual bool greaterThan(void* attr1, void* attr2)=0;
};

class StringAttrComparator: public AttributeComparator{
public:
	StringAttrComparator(){};

	virtual bool compare(void* attr1, void* attr2){
		if(attr1 == NULL && attr2 == NULL)
			return true;
		if(attr1 == NULL || attr2 == NULL)
			return false;
		return (((string*)attr1)->compare(*((string*)attr2))) == 0;
	};

	virtual int compareint(void* attr1, void* attr2){
		if(attr1 == NULL && attr2 == NULL)
			return 0;
		if(attr1 == NULL)
			return (new string(""))->compare(*((string*)attr2));
		if(attr2 == NULL)
			return (((string*)attr1)->compare(*(new string(""))));
		return (((string*)attr1)->compare(*((string*)attr2)));
		//return 0;
	};

	virtual bool lessThan(void* attr1, void* attr2){
		return (*((string*)attr1)) < (*((string*)attr2));
	};

	virtual bool greaterThan(void* attr1, void* attr2){
		if(attr1 == NULL && attr2 == NULL)
			return false;
		if(attr1 == NULL)
			return false;
		if(attr2 == NULL)
			return true;
		//return (*((string*)attr1)) > (*((string*)attr2));
		return (((string*)attr1)->compare(*((string*)attr2))) > 0;
	}
};

#endif /* ATTRIBUTECOMPARATOR_H_ */
