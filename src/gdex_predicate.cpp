/*
 * gdex_predicate.cpp
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


#include "gdex_predicate.h"

#include <set>

bool o_predicate_t::mcMatrices(focusgraph_t& pattern, focusgraph_t& reference){
#ifdef DL_CALLS
	std::cout<<">>o_predicate_t::mcMatrices\n";
#endif
	if(pattern._directives != reference._directives)
		exit(1);

	/*
	 * Rb1 viene fatta gli edge del pattern
	 * Qbx viene costruita in base alle label degli edge del target, ma solo per quelle che si hanno pure nel pattern
	 *
	 * 1 = ij ;  2 = ji
	 */

	/*
	 * For general applications, we don't know if the labels' set is contiguous
	 */
	std::set<cardinal_t> t_elabels;
	for(u_size_t i =0; i<reference.nof_edges; i++){
		t_elabels.insert(reference.edges[i].attr);
	}
	std::set<cardinal_t> p_elabels;
	for(u_size_t i =0; i<pattern.nof_edges; i++){
		if(t_elabels.find(pattern.edges[i].attr) == t_elabels.end()){
			return false;
		}
		p_elabels.insert(pattern.edges[i].attr);
	}


	_l1 = pattern.nof_edges;
	_l2 = reference.nof_nodes;

	if(p_elabels.size() == 0)
		return true;

	predicate_Mij_t Qb1;
	predicate_Mij_t Qb2;
	init_predicate_Mij_t(Qb1, *(p_elabels.rbegin()) +1, reference.nof_nodes);
	init_predicate_Mij_t(Qb2, *(p_elabels.rbegin()) +1, reference.nof_nodes);

	cardinal_t attr;
	cardinal_t i;
	cardinal_t j;

	bool nisord = !reference.is(ordered);

	for(u_size_t k = 0; k<reference.nof_edges; k++){

		attr = reference.edges[k].attr;
		i = reference.edges[k].source;
		j = reference.edges[k].target;

		Qb1[attr][i]->set(j, true);
		Qb2[attr][j]->set(i, true);

		//if(! reference.is(ordered)){
		if(nisord){
			Qb1[attr][j]->set(i, true);
			Qb2[attr][i]->set(j, true);
		}

	}

//	Rb1 = new bitMatrix_t*[ *(p_elabels.rbegin()) ];
//	Rb2 = new bitMatrix_t*[ *(p_elabels.rbegin()) ];

	Rb1 = new bitMatrix_t*[ pattern.nof_edges ];
	Rb2 = new bitMatrix_t*[ pattern.nof_edges ];

	for(u_size_t k = 0; k<pattern.nof_edges; k++){
		attr = pattern.edges[k].attr;
		Rb1[k] = Qb1[attr];
		Rb2[k] = Qb2[attr];
	}

	_l1 = pattern.nof_edges;
	_l2 = reference.nof_nodes;

	delete [] Qb1;
	delete [] Qb2;

	return true;
};
