/*
 * Tree.h
 *
 *  Created on: Dec 16, 2011
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


#ifndef TREE_H_
#define TREE_H_

//#define T_CALLS

template<class T>
class TreeNode{
public:
	TreeNode* next;
	TreeNode* child;
	T value;
	TreeNode(T _value){
		value = _value;
		next = NULL;
		child = NULL;
	}
	TreeNode(T _value, TreeNode* _next){
		value = _value;
		next = _next;
		child = NULL;
	}
	~TreeNode(){
#ifdef T_CALLS
		std::cout<<"~TreeNode\n";
#endif
		if(next != NULL)
			delete next;
		if(child != NULL)
			delete child;
#ifdef T_CALLS
		std::cout<<"<~TreeNode\n";
#endif
	}
};


/**
 * Tree with multiple roots
 */
template<class T>
class Tree{
public:
	TreeNode<T>* head;
	Tree(){
		head = NULL;
	}
	~Tree(){
#ifdef T_CALLS
		std::cout<<"~Tree\n";
#endif
		if(head != NULL)
			delete head;
#ifdef T_CALLS
		std::cout<<"<~Tree\n";
#endif
	}

	TreeNode<T>* insert(TreeNode<T>* parent, T value,  bool allow_duplicate){
		TreeNode<T>* r;

		TreeNode<T>* e;
		if(parent == NULL)	e = head;
		else				e = parent->child;

		TreeNode<T>* p = NULL;
		while(e!=NULL && value > e->value){
			 p = e;
			 e = e->next;
		}
		if(e == NULL){
			if(p == NULL){
				if(parent == NULL){
					head = new TreeNode<T>(value);
					r = head;
				}
				else{
					parent->child = new TreeNode<T>(value);
					r = parent->child;
				}
			}
			else{
				p->next = new TreeNode<T>(value, e);
				r = p->next;
			}
		}
		else{
			if(value == e->value){
				if(allow_duplicate){
					e->next = new TreeNode<T>(value, e->next);
					r = e->next;
				}
				else
					r = e;
			}
			else{
				if(p == NULL){
					if(parent == NULL){
						head = new TreeNode<T>(value, e);
						r = head;
					}
					else{
						parent->child = new TreeNode<T>(value, e);
						r = parent->child;
					}
				}
				else{
					p->next = new TreeNode<T>(value, e);
					r = p->next;
				}
			}
		}

		return r;
	}

	TreeNode<T>* insert_rev(TreeNode<T>* parent, T value,  bool allow_duplicate){
		TreeNode<T>* r;

		TreeNode<T>* e;
		if(parent == NULL)	e = head;
		else				e = parent->child;

		TreeNode<T>* p = NULL;
		while(e!=NULL && value <= e->value){
			 p = e;
			 e = e->next;
		}
		if(e == NULL){
			if(p == NULL){
				if(parent == NULL){
					head = new TreeNode<T>(value);
					r = head;
				}
				else{
					parent->child = new TreeNode<T>(value);
					r = parent->child;
				}
			}
			else{
				p->next = new TreeNode<T>(value, e);
				r = p->next;
			}
		}
		else{
			if(value == e->value){
				if(allow_duplicate){
					e->next = new TreeNode<T>(value, e->next);
					r = e->next;
				}
				else
					r = e;
			}
			else{
				if(p == NULL){
					if(parent == NULL){
						head = new TreeNode<T>(value, e);
						r = head;
					}
					else{
						parent->child = new TreeNode<T>(value, e);
						r = parent->child;
					}
				}
				else{
					p->next = new TreeNode<T>(value, e);
					r = p->next;
				}
			}
		}

		return r;
	}

	void print_dfs(){
		std::cout<<"Tree: ";
		print_dfs(head);
		std::cout<<"__\n";
	}
	void print_dfs(TreeNode<T>* n){
		if(n!=NULL){
			std::cout<<"[("<<n->value<<")";
			TreeNode<T>* c = n->child;
			while(c!=NULL){
				print_dfs(c);
				c = c->next;
			}
			std::cout<<"]";
		}
	}
};

#endif /* TREE_H_ */
