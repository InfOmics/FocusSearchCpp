/*
 * vars_ordering.h
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


#ifndef VARS_ORDERING_H_
#define VARS_ORDERING_H_

#include "gdex_graph.h"

#include "debug.h"


class o_vars_order_t{
public:
	cardinal_t* varAtOrd;
	cardinal_t* ordOfVar;

//	vars_order_t(u_size_t  size){
//		varAtOrd = new cardinal_t[size];
//		ordOfVar = new cardinal_t[size];
//	}

	o_vars_order_t(focusgraph_t& pattern){
		//varAtOrd = new cardinal_t[pattern.nof_nodes];
		//ordOfVar = new cardinal_t[pattern.nof_nodes];
		varAtOrd = (cardinal_t*)calloc(pattern.nof_nodes, sizeof(cardinal_t));
		ordOfVar = (cardinal_t*)calloc(pattern.nof_nodes, sizeof(cardinal_t));
	}

	~o_vars_order_t(){
#ifdef DEBUG_DELETE
		std::cout<<"~o_vars_order_t>\n";
#endif
		delete [] varAtOrd;
		delete [] ordOfVar;
#ifdef DEBUG_DELETE
		std::cout<<"~o_vars_order_t<\n";
#endif
	}

	void print_varAtOrd(focusgraph_t& pattern){
		std::cout<<"#varAtOrd:";
		if(varAtOrd == NULL)
			std::cout<<"NULL";
		else{
			for(u_size_t i=0; i<pattern.nof_nodes; i++)
				std::cout<<"["<<i<<":"<<varAtOrd[i]<<"]";
		}
		std::cout<<"\n";
	}

	void print_ordOfVar(focusgraph_t& pattern){
		std::cout<<"#ordOfVar:";
		if(ordOfVar == NULL)
			std::cout<<"NULL";
		else{
			for(u_size_t i=0; i<pattern.nof_nodes; i++)
				std::cout<<"["<<i<<":"<<ordOfVar[i]<<"]";
		}
		std::cout<<"\n";
	}


};


#endif /* VARS_ORDERING_H_ */
