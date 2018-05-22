/*
 * temp_prematch.h
 *
 *  Created on: Mar 19, 2012
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


#ifndef TEMP_PREMATCH_H_
#define TEMP_PREMATCH_H_


#include "gdex_domain.h"
#include "mp_Tree.h"
#include "debug.h"
//#define PM_CALLS
//#define PM_INS

#define COMPLEX_PREMATCH

namespace PREMATCH{

class degrees_t{
public:
	u_size_t	*_inDegrees;
	u_size_t	*_outDegrees;

	degrees_t(){
		_inDegrees = NULL;
		_outDegrees = NULL;
	}

	~degrees_t(){
		if(_outDegrees != NULL)
			delete [] _outDegrees;
		if(_inDegrees != NULL)
			delete [] _inDegrees;
	}
};

inline bool
degree_check_oo(degrees_t& pattern, u_size_t j, degrees_t& target, u_size_t i){
	return 	pattern._outDegrees[j] <= target._outDegrees[i]  &&
			pattern._inDegrees[j] <= target._inDegrees[i];
}
inline bool
degree_check_uu(degrees_t& pattern, u_size_t j, degrees_t& target, u_size_t i){
	return 	pattern._outDegrees[j] <= target._outDegrees[i];
}


class o_prematch_t{

private:
//TODO...done
	o_domains_t& o_domains;

	focusgraph_t& pattern;
	focusgraph_t& reference;

	bool		takeOrder;
	bool 		takeEdgeLabels;
	bool 		takeNodeLabels;

	degrees_t p_degrees;
	degrees_t r_degrees;

#ifdef COMPLEX_PREMATCH
	Tree<u_size_t>			**p_outTrees;
	Tree<u_size_t>			**p_inTrees;
	Tree<u_size_t>			**r_outTrees;
	Tree<u_size_t>			**r_inTrees;

	int treesLevels;
#endif

public:
	o_prematch_t(	o_domains_t&  _o_domains,
					focusgraph_t& _pattern,
					focusgraph_t& _reference)
			:	o_domains(_o_domains), pattern(_pattern), reference(_reference)
	{
		//TODO...done
		takeOrder 			= pattern.is(ordered);
		takeEdgeLabels 		= pattern.is(edge_labeled);
		takeNodeLabels		= pattern.is(node_labeled);

		init_degrees(pattern, p_degrees);
		init_degrees(reference, r_degrees);

#ifdef COMPLEX_PREMATCH
		treesLevels = 0;
		if(takeEdgeLabels)
			treesLevels++;
		if(takeNodeLabels)
			treesLevels++;

		//p_outTrees = p_inTrees = r_outTrees = r_inTrees = NULL;

		init_trees();
#endif
	}


	~o_prematch_t(){
#ifdef COMPLEX_PREMATCH
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			if(p_outTrees[i] != NULL){ delete p_outTrees[i];}
			if(p_inTrees[i] != NULL) delete p_inTrees[i];
		}
		delete [] p_outTrees;
		delete [] p_inTrees;

		for(u_size_t i=0; i<reference.nof_nodes; i++){
			if(r_outTrees[i] != NULL) delete r_outTrees[i];
			if(r_inTrees[i] != NULL) delete r_inTrees[i];
		}
		delete [] r_outTrees;
		delete [] r_inTrees;
#endif
	}

private:

	void init_degrees(focusgraph_t& g, degrees_t& degrees){
		degrees._outDegrees = (u_size_t*)calloc(g.nof_nodes, sizeof(u_size_t));
		if(takeOrder){
			degrees._inDegrees = (u_size_t*)calloc(g.nof_nodes, sizeof(u_size_t));
		}
		for(u_size_t i = 0; i<g.nof_edges; i++){
			degrees._outDegrees[ g.edges[i].source ] ++;
			if(takeOrder)
				degrees._inDegrees[ g.edges[i].target ] ++;
		}
	}

	u_size_t degreeOf(degrees_t& degrees, u_size_t nodeid){
		if(takeOrder){
			return  degrees._outDegrees[nodeid] + degrees._inDegrees[nodeid];
		}
		return degrees._outDegrees[nodeid];
	}

#ifdef COMPLEX_PREMATCH
	void build_prematch_trees(
			focusgraph_t& 			_g,
			degrees_t& 				degrees,
			Tree<u_size_t>			**_outTrees,//return
			Tree<u_size_t>			**_inTrees,  //return
			domain_t&				needed
			){
#ifdef PM_CALLS
		std::cout<<">build_prematch_trees\n";
#endif
		for(u_size_t i=0;i<_g.nof_nodes; i++){
			_outTrees[i] = new Tree<u_size_t>();
			if(takeOrder)
				_inTrees[i] = new Tree<u_size_t>();
			else
				_inTrees[i] = NULL;
		}

#ifdef PM_INS
		std::cout<<"-\n";
#endif

		TreeNode<u_size_t>* tn = NULL;
		for(u_size_t i = 0; i<_g.nof_edges; i++){
			if(needed.get( _g.edges[i].source )){
				tn = NULL;
				if(takeEdgeLabels)
					tn = _outTrees[ _g.edges[i].source ]->insert(NULL, _g.edges[ i ].attr, false);
				if(takeNodeLabels)
					tn = _outTrees[ _g.edges[i].source ]->insert(tn, _g.nodes_attrs[_g.edges[i].target], false);
				 _outTrees[ _g.edges[i].source ]->insert_rev(tn, degreeOf(degrees, _g.edges[i].target ), true);
			}
			if(takeOrder){
				if(needed.get( _g.edges[i].target )){
					tn = NULL;
					if(takeEdgeLabels)
						tn = _inTrees[ _g.edges[i].target ]->insert(NULL, _g.edges[ i ].attr, false);
					if(takeNodeLabels)
						tn = _inTrees[ _g.edges[i].target ]->insert(tn, _g.nodes_attrs[ _g.edges[i].source ], false);
					 _inTrees[ _g.edges[i].target ]->insert_rev(tn, degreeOf(degrees, _g.edges[i].source ), true);
				}
			}
		}


#ifdef DEBUG_COMPLEX_PREMATCH
		for(u_size_t i=0;i<_g.nof_nodes; i++){
			std::cout<<"Tree["<<i<<"]\n";
			_outTrees[i]->print_dfs();
		}
#endif
#ifdef PM_CALLS
		std::cout<<"<build_prematch_trees\n";
#endif
	}

	void init_trees(){
#ifdef PM_CALLS
		std::cout<<">init_trees\n";
#endif
		p_outTrees = new Tree<u_size_t>*[pattern.nof_nodes];
		p_inTrees = new Tree<u_size_t>*[pattern.nof_nodes];


		domain_t pneed; pneed.setAll(pattern.nof_nodes, true);
		build_prematch_trees(pattern, p_degrees, p_outTrees, p_inTrees, pneed);

		r_outTrees = new Tree<u_size_t>*[reference.nof_nodes];
		r_inTrees = new Tree<u_size_t>*[reference.nof_nodes];

		for(u_size_t i=0; i<reference.nof_nodes; i++){
			r_outTrees[i] = NULL;
			r_inTrees[i] = NULL;
		}
#ifdef PM_CALLS
		std::cout<<"<init_trees\n";
#endif
	}



	bool _internal_tree_match(
			TreeNode<u_size_t> *p_n,
			TreeNode<u_size_t> *t_n,
			int level){

		if(level == 0){
			//node degree level
			while(p_n != NULL){
				if(t_n == NULL){
#ifdef PM_INS
					std::cout<<"complex-discard on degrees\n";
#endif
					return false;
				}
				if(p_n->value > t_n->value){
#ifdef PM_INS
					std::cout<<"complex-discard on degrees\n";
#endif
					return false;
				}
				p_n = p_n->next;
				t_n = t_n->next;
			}
		}
		else{
			while(p_n != NULL){//(node/edge) label level
				while(t_n!=NULL && (t_n->value < p_n->value)){
					t_n = t_n->next;
				}
				if(!(t_n != NULL && (p_n->value == t_n->value))){
					return false;
				}

				_internal_tree_match(p_n->child, t_n->child, level-1);

				p_n = p_n->next;
			}
		}
		return true;
	}





	bool
	complex_check_oo(u_size_t p_node, u_size_t r_node){
		return _internal_tree_match(p_outTrees[p_node]->head, r_outTrees[r_node]->head, treesLevels) &&
				 _internal_tree_match(p_inTrees[p_node]->head, r_inTrees[r_node]->head, treesLevels);
	};
	bool
	complex_check_uu(u_size_t p_node, u_size_t r_node){
		return _internal_tree_match(p_outTrees[p_node]->head, r_outTrees[r_node]->head, treesLevels);
	};
#endif

public:
	void run(){
		domain_t rneed;
		rneed.resize(reference.nof_nodes);

		bool(* degree_check)(degrees_t& pattern, u_size_t j, degrees_t& target, u_size_t i);
		if(takeOrder){
			degree_check = degree_check_oo;
		}
		else{
			degree_check = degree_check_uu;
		}


		if(takeNodeLabels){
			for(u_size_t i = 0; i<reference.nof_nodes; i++){
				for(u_size_t j=0; j<pattern.nof_nodes; j++){
					if( 	(pattern.nodes_attrs[j] == reference.nodes_attrs[i]) &&
							degree_check(p_degrees, j, r_degrees, i)){
						o_domains.Dsets[j].set(i, true);
						o_domains.Dcards[j]++;
						o_domains.insertDlistRecord(j, i);
						rneed.set(i, true);
					}
				}
			}
		}else{
			for(u_size_t i = 0; i<reference.nof_nodes; i++){
				for(u_size_t j=0; j<pattern.nof_nodes; j++){
					if( 	degree_check(p_degrees, j, r_degrees, i)){
						o_domains.Dsets[j].set(i, true);
						o_domains.Dcards[j]++;
						o_domains.insertDlistRecord(j, i);
						rneed.set(i, true);
					}
				}
			}
		}


#ifdef COMPLEX_PREMATCH
		//init_trees();
		build_prematch_trees(reference, r_degrees, r_outTrees, r_inTrees, rneed);


		Dlist_rec_t *p_d, *c_d, *t_c;
		bool compatible = false;
		for(u_size_t j=0; j<pattern.nof_nodes; j++){
			p_d = NULL;
			c_d = o_domains.Dlists[j];
			while(c_d != NULL){
				if(takeOrder){
					compatible = complex_check_oo(j, c_d->value);
				}
				else{
					compatible = complex_check_uu(j, c_d->value);
				}

				if(compatible){
					p_d = c_d;
					c_d = c_d->nextValue;
				}
				else{
					o_domains.Dsets[j].set(c_d->value, false);
					o_domains.Dcards[j]--;
					if(p_d==NULL){
						o_domains.Dlists[j] = c_d->nextValue;
						delete c_d;
						c_d = o_domains.Dlists[j];
					}else{
						p_d->nextValue = c_d->nextValue;
						delete c_d;
						c_d = p_d->nextValue;
					}

				}
			}
		}
#endif

	}


};

}


#endif /* TEMP_PREMATCH_H_ */
