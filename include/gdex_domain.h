/*
 * gdex_domains.h
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


#ifndef GDEX_DOMAIN_H_
#define GDEX_DOMAIN_H_

#include "mp_sbitset.h"
#include "mp_SurrogateBitSet.h"
#include "gdex_graph.h"

#include "debug.h"

//M2: domainType  				= PACKEDSET OF [0..betaUB];
//typedef SurrogateBitSet 		domain_t;
typedef sbitset 	domain_t;


//M2:	DlistRecType 				= RECORD
//M2:										value: CARDINAL;
//M2:										nextValue: DlistPtrType
//M2:								  END;
class 								Dlist_rec_t{
public:
	cardinal_t value;
	Dlist_rec_t* nextValue;

	Dlist_rec_t(){
		value  = 0;
		nextValue = NULL;
	}

	~Dlist_rec_t(){
	}
};

//M2:	DlistsType  				= ARRAY[0..alphaUB] OF DlistPtrType;
typedef Dlist_rec_t** 				DLists_t;

class o_domains_t{
public:
	domain_t	*Dsets;
	cardinal_t	*Dcards;
	DLists_t	Dlists;

	focusgraph_t& _pattern;


	o_domains_t(focusgraph_t& pattern) : _pattern(pattern){
		Dsets = new domain_t[pattern.nof_nodes];
		//Dcards = new cardinal_t[pattern.nof_nodes];
		Dcards = (cardinal_t*)calloc(pattern.nof_nodes, sizeof(cardinal_t));

		Dlists = new Dlist_rec_t*[pattern.nof_nodes];
		for(u_size_t i=0; i<pattern.nof_nodes; i++){
			Dlists[i] = NULL;
		}
	}


	void insertDlistRecord(u_size_t j, cardinal_t value){
		if(Dlists[j] == NULL){
			Dlists[j] = new Dlist_rec_t();
			Dlists[j]->value = value;
			Dlists[j]->nextValue = NULL;
		}
		else{
			Dlist_rec_t* d = new Dlist_rec_t();
			d->value = value;
			d->nextValue = Dlists[j];
			Dlists[j] = d;
		}
	}

	~o_domains_t(){
#ifdef DEBUG_DELETE
		std::cout<<"~o_domains_t>\n";
#endif
		delete [] Dsets;
		delete [] Dcards;
		if(Dlists != NULL){
			Dlist_rec_t *c_d, *p_d;
			for(u_size_t i=0; i<_pattern.nof_nodes; i++){
				if(Dlists[i] != NULL){
					p_d = NULL;
					c_d = Dlists[i];
					while(c_d != NULL){
						if(p_d != NULL) delete p_d;
						p_d = c_d;
						c_d = c_d->nextValue;
					}
					if(p_d != NULL) delete p_d;
					Dlists[i] = NULL;
				}
			}
			delete [] Dlists;
		}
#ifdef DEBUG_DELETE
		std::cout<<"~o_domains_t<\n";
#endif
	}


	void print_domains(u_size_t s){
		for(u_size_t i = 0; i<s; i++){
			std::cout<<"Domain for node id["<<i<<"]: card["<<Dcards[i]<<"]: ";
			Dsets[i].print_numbers();std::cout<<"\n";
		}
	};

	void print_Dlists(){
		std::cout<<"##DLists::\n";
		for(int i=0; i<_pattern.nof_nodes; i++){
			std::cout<<"node["<<i<<"]: ";
			if(Dlists[i]== NULL){
				std::cout<<"no Dlists";
			}else{
				Dlist_rec_t* d = Dlists[i];
				std::cout<<"[";
				while(d!=NULL){
					std::cout<<d->value<<" ";
					d=d->nextValue;
				}
				std::cout<<"]";
			}
			std::cout<<"\n";
		}
	}
};





#endif /* GDEX_DOMAINS_H_ */
