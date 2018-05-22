/*
 * gdex_graph.h
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


#include <iostream>

#ifndef GDEX_GRAPH_H_
#define GDEX_GRAPH_H_

#include "size_t.h"
#include "mp_DynamicGraph.h"
#include "mp_IDer.h"

#include <stdlib.h>

#include "debug.h"

//typedef short ETYPE_t;
//#define ETYPE_N 0x0;
//#define ETYPE_F 0x1;
//#define ETYPE_B 0x2;

enum ETYPE_t{ETYPE_N, ETYPE_F, ETYPE_B};


class agraph_t{
public:
	DGDirectives _directives;
	bool is(DGDirectives d){
		return (_directives & d) > 0;
	}
};



class focusgraph_edge_t{
public:
	cardinal_t source;
	cardinal_t target;
	cardinal_t attr;

	focusgraph_edge_t(){
		source =target = attr = 0;
	}
	~focusgraph_edge_t(){
	}
};

class focusgraph_t : public agraph_t{
public:

	u_size_t nof_nodes;
	u_size_t nof_edges;

	cardinal_t* nodes_attrs;	//indexed by node id, from 0 to n -1
	focusgraph_edge_t* edges;

//	u_size_t nof_dist_nodes_lables;
//	u_size_t nof_dist_edges_lables;


	focusgraph_t(DynamicGraph& dg, IDer& ider){
		_directives = dg.directives;

		u_size_t nofn = 0;
		u_size_t nofe = 0;

		TempListNode* tn = dg.nodes;
		while(tn != NULL){nofn++;	tn = tn->next;}

		TempListEdge* te = dg.edges;
		while(te != NULL){nofe++;	te = te->next;}

		nof_nodes = nofn;
		nof_edges = nofe;

//		nodes_attrs = (cardinal_t*)(calloc(nof_nodes, sizeof(cardinal_t)));
//		edges = (focusgraph_edge_t*)(malloc(nof_edges * sizeof(focusgraph_edge_t)));

		//nodes_attrs = new cardinal_t[nof_nodes];
		nodes_attrs = (cardinal_t*)calloc(nof_nodes, sizeof(cardinal_t));
		edges = new focusgraph_edge_t[nof_edges];

		tn = dg.nodes;
		while(tn != NULL){
			//nodes_attrs[tn->content->id] = ider.idFor(tn->content->attribute);
//			std::cout<<*((std::string*)(tn->content->attribute))<<"\n";
//			std::cout<<ider.idFor(tn->content->attribute)<<"\n";
			nodes_attrs[tn->content->id] = ider.idFor(tn->content->attribute);
			//nodes_attrs[tn->content->id] = 0;
			tn = tn->next;
		}

		int i=0;
		te = dg.edges;
		while(te != NULL){
			edges[i].source = te->content->source;
			edges[i].target = te->content->target;
			edges[i].attr = ider.idFor(te->content->attribute);
			i++;
			te = te->next;
		}

	};

	~focusgraph_t(){
		delete [] nodes_attrs;
		delete [] edges;
	}


	void print(){
		std::cout<<"##graph\n";
		if(this->is(ordered))std::cout<<"ordered\n";
		std::cout<<"\tnodes: "<<nof_nodes<<":\n";
		for(u_size_t i = 0; i< nof_nodes; i++){
			std::cout<<"\t\t["<<nodes_attrs[i]<<"]\n";
		}
		std::cout<<"\tedges: "<<nof_edges<<":\n";
		for(u_size_t i = 0; i< nof_edges; i++){
			std::cout<<"\t\t("<<edges[i].source<<", "<<edges[i].target<<")["<<edges[i].attr<<"]\n";
		}
	}
};


#endif /* GDEX_GRAPH_H_ */
