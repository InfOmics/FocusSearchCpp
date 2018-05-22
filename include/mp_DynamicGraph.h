/*
 * DynamicGraph.h
 *
 *  Created on: Dec 10, 2011
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


#ifndef DYNAMICGRAPH_H_
#define DYNAMICGRAPH_H_

//#define DG_CALLS

#include "size_t.h"
#include "mp_GTypeDirectives.h"
#include "debug.h"


/* ===================================================================================================================
 * DynamicGraphEdge
 * ===================================================================================================================
 */
class DynamicGraphEdge{
public:
	u_size_t source;
	u_size_t target;
	void* attribute;

	DynamicGraphEdge(u_size_t _source, u_size_t _target, void* _attr){
		source=_source;
		target=_target;
		attribute=_attr;
	};
	~DynamicGraphEdge(void){
		//delete attribute;
	};
};

/* ===================================================================================================================
 * DynamicGraphNode
 * ===================================================================================================================
 */
class DynamicGraphNode{
public:
	u_size_t id;
	void* attribute;

	DynamicGraphNode(){
		id=-1;
		attribute=NULL;
	};
	DynamicGraphNode(u_size_t _id, void* _attr){
		id=_id;
		attribute=_attr;
	};
	~DynamicGraphNode(void){
		//delete attribute;
	};

};

/* ===================================================================================================================
 * TempListNode
 * ===================================================================================================================
 */
class TempListNode{
public:
	TempListNode* next;
	DynamicGraphNode* 	content;
	TempListNode(DynamicGraphNode* _content){
		next=NULL;
		content=_content;
	};
	~TempListNode(void){
		//delete next;
		delete content;
	};
};

/* ===================================================================================================================
 * TempListEdge
 * ===================================================================================================================
 */
class TempListEdge{
public:
	TempListEdge* next;
	DynamicGraphEdge* 	content;
	TempListEdge(DynamicGraphEdge* _content){
		next=NULL;
		content=_content;
	};
	~TempListEdge(void){
		//delete next;
		delete content;
	};
};

/* ===================================================================================================================
 * DynamicGraph
 * ===================================================================================================================
 */
class DynamicGraph{
public:
	u_size_t id;
	u_size_t next_node_id;

	TempListNode* nodes;
	TempListNode* nodestail;

	TempListEdge* edges;
	TempListEdge* edgestail;

	DGDirectives directives;

	DynamicGraph(){
		id=-1;
		nodes=NULL;
		edges=NULL;
		next_node_id=0;
		directives = none;
	};
	DynamicGraph(DGDirectives _directives)
			: directives(_directives){
		id=-1;
		nodes=NULL;
		edges=NULL;
		next_node_id=0;
	}
	DynamicGraph(u_size_t _id){
		id=_id;
		nodes=NULL;
		edges=NULL;
		next_node_id=0;
		directives = none;
	};
	DynamicGraph(u_size_t _id, DGDirectives _directives)
			: directives(_directives){
		id=_id;
		nodes=NULL;
		edges=NULL;
		next_node_id=0;
	}

	~DynamicGraph(void){
#ifdef DG_CALLS
		std::cout<<">~DynamicGraph\n";
#endif
		TempListNode* tln=nodes;
		TempListNode* ptln=NULL;
		while(tln!=NULL){
			if(ptln!=NULL)	delete ptln;
			ptln=tln;
			tln=tln->next;
		}
		if(ptln!=NULL)	delete ptln;
		TempListEdge* tle=edges;
		TempListEdge* ptle=NULL;
		while(tle!=NULL){
			if(ptle!=NULL)	delete ptle;
			ptle=tle;
			tle=tle->next;
		}
		if(ptle!=NULL)	delete ptle;
#ifdef DG_CALLS
		std::cout<<"<~DynamicGraph\n";
#endif
	};

	void freeContent(){
#ifdef DEBUG_DELETE
		std::cout<<">DynamicGraph.freeContent()\n";
#endif
		TempListNode* n = nodes;
		while(n!=NULL){
			if(n->content!= NULL && n->content->attribute != NULL)
				delete n->content->attribute;
			n = n->next;
		}
		TempListEdge* e = edges;
		while(e!=NULL){
			if(e->content!= NULL && e->content->attribute != NULL)
				delete e->content->attribute;
			e = e->next;
		}
#ifdef DEBUG_DELETE
		std::cout<<"<DynamicGraph.freeContent()\n";
#endif
	}

	void insert_node(void* _attr){
		if(nodes==NULL){
			nodes=new TempListNode(new DynamicGraphNode(next_node_id, _attr));
			nodestail=nodes;
		}
		else{
			TempListNode* newnode=new TempListNode(new DynamicGraphNode(next_node_id, _attr));
			//newnode->next=nodes;
			//nodes=newnode;
			nodestail->next=newnode;
			nodestail=newnode;
		}
		next_node_id++;
	};

	void insert_edge(u_size_t _node1, u_size_t _node2, void* _attr){
		if(edges==NULL){
			edges=new TempListEdge(new DynamicGraphEdge(_node1, _node2, _attr));
			edgestail = edges;
		}
		else{
			TempListEdge* newedge=new TempListEdge(new DynamicGraphEdge(_node1, _node2, _attr));
			newedge->next=edges;
			edges=newedge;
		}
	};

	u_size_t count_edges(u_size_t _nodeid, u_size_t& _out, u_size_t& _in){
		u_size_t count=0;
		DynamicGraphEdge* e;
		TempListEdge* c=edges;
		while(c!=NULL){
			e=(DynamicGraphEdge*)c->content;
			if((e->source)==_nodeid){
				_out++;
				count++;
			}
			if((e->target)==_nodeid){
				_in++;
				count++;
			}
			c=c->next;
		}
		return count;
	};

	void count_edges(u_size_t* _outs, u_size_t* _ins){
		DynamicGraphEdge* e;
		TempListEdge* c=edges;
		while(c!=NULL){
			e=(DynamicGraphEdge*)c->content;
			_outs[e->source]++;
			_ins[e->target]++;
			c=c->next;
		}
	};


	bool is(DGDirectives _directives){
		return (directives & _directives) > 0;
	}
};


#endif /* DYNAMICGRAPH_H_ */
