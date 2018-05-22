/*
 * sbitset.h
 *
 *  Created on: Nov 5, 2010
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


#ifndef OSBITSET_H_
#define OSBITSET_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "size_t.h"

//#define BS_CALLS

typedef u_size_t sbitset_block;

/*
 * At least one empty block ever
 */


class sbitset{
static const size_t bytes_per_block = sizeof(sbitset_block);
static const size_t bits_per_block = sizeof(sbitset_block)*8;

public:
	sbitset_block* _bits;
	size_t _nblocks;

public:
	sbitset(){
#ifdef BS_CALLS
		std::cout<<">sbitset():"<<this<<"\n";
#endif
		_bits = (sbitset_block*)calloc(1, bytes_per_block);
		_nblocks = 1;
	}

	sbitset(const sbitset& c){
#ifdef BS_CALLS
		std::cout<<">sbitset(const c):"<<this<<"\n";
#endif
		_bits = (sbitset_block*)malloc(c._nblocks * bytes_per_block);
		memcpy(_bits, c._bits, c._nblocks * bytes_per_block);
		_nblocks = c._nblocks;
	}
	sbitset(sbitset& c){
#ifdef BS_CALLS
		std::cout<<">sbitset(c):"<<this<<"\n";
#endif
		_bits = (sbitset_block*)malloc(c._nblocks * bytes_per_block);
		memcpy(_bits, c._bits, c._nblocks * bytes_per_block);
		_nblocks = c._nblocks;
	}

	~sbitset(){
#ifdef BS_CALLS
		std::cout<<"~sbitset():"<<this<<"\n";
#endif
		delete [] _bits;
	}


	void printi(std::ostream& os){
		for(size_t i=0;i<_nblocks * bits_per_block;i++){
			os<<"("<<i<<":"<<get(i)<<")";
		}
	}

	void print_numbers(){
		std::cout<<"[";
		for(size_t i=0;i<_nblocks * bits_per_block;i++){
			if(get(i))
				std::cout<<i<<" ";
		}
		std::cout<<"]";
	}
	void print_info(){
	}


	bool is_empty(){
#ifdef BS_CALLS
		std::cout<<"sbitset.is_empty()\n";
		std::cout<<"\t";print_numbers();std::cout<<"\n";
#endif
		for(size_t i = 0; i<_nblocks; i++){
			if(_bits[i]!=0){
#ifdef BS_CALLS
		std::cout<<"sbitset.is_empty():false\n";
#endif
				return false;
			}
		}
#ifdef BS_CALLS
		std::cout<<"<sbitset.is_empty():true\n";
#endif
		return true;
	}

	bool get(size_t pos){
		if(pos>=0 && pos<(_nblocks * bits_per_block)){
			return _bits[pos/bits_per_block]&(1<<(bits_per_block-(pos%bits_per_block)-1));
		}
		return 0;
	}

	void resize(size_t new_size){
		//TODO not used but nedded for future version
//		if(new_size < _nblocks * bitsof_block){
//			//size_t n_nblocks = (size_t)(ceil(new_size / bitsof_block));
//			size_t n_nblocks = (size_t)(ceil(new_size / bitsof_block) +1);
//			//if(n_nblocks == 0)n_nblocks = 1;
//			sbitset_block* n_bits = (sbitset_block*)calloc(n_nblocks, bytesof_block);
//			memcpy(n_bits, _bits, n_nblocks * bytesof_block);
//			_nblocks = n_nblocks;
//			_bits = n_bits;
//		}
//		else
		if(new_size > _nblocks * bits_per_block){
			size_t n_nblocks = (size_t)(ceil(new_size / bits_per_block) +1);
			//size_t n_nblocks = (size_t)(ceil(new_size / bitsof_block));
			//if(n_nblocks == 0)n_nblocks = 1;
			sbitset_block* n_bits = (sbitset_block*)calloc(n_nblocks, bytes_per_block);
			memcpy(n_bits, _bits, _nblocks * bytes_per_block);
			_nblocks = n_nblocks;
			delete [] _bits;
			_bits = n_bits;
		}
	}

	void resizeAsis(size_t new_size){
		//TODO not used but nedded for future version
//		if(new_size < _nblocks * bitsof_block){
//		}
//		else
		if(new_size > _nblocks * bits_per_block){
			size_t n_nblocks = (size_t)(ceil(new_size / bits_per_block) +1);
			delete [] _bits;
			_bits = (sbitset_block*)malloc(n_nblocks * bytes_per_block);
			_nblocks = n_nblocks;
		}
	}



	void set(size_t pos, bool value){
		if(pos>=0){
			if(pos >= _nblocks * bits_per_block){
				resize(pos+1);
			}
			if(value!=get(pos))
				this->_bits[pos/bits_per_block] ^= 1<<(bits_per_block-(pos%bits_per_block)-1);
		}
	}


	sbitset& operator&=(sbitset& bs){
#ifdef BS_CALLS
		std::cout<<"sbitset&=\n";
		std::cout<<"\t";print_numbers();std::cout<<"\n";
		std::cout<<"\t&";bs.print_numbers();std::cout<<"\n";
#endif
		size_t wblokcs = _nblocks <= bs._nblocks ? _nblocks : bs._nblocks;
		for(size_t i = 0; i<wblokcs; i++)
			_bits[i] &= bs._bits[i];
		for(size_t i = wblokcs; i< _nblocks; i++)
			_bits[i] = 0;

#ifdef BS_CALLS
		std::cout<<"\t=";print_numbers();std::cout<<"\n";
		std::cout<<"<sbitset&=\n";
#endif
		return *this;
	}

	sbitset& operator|=(sbitset& bs){
#ifdef BS_CALLS
		std::cout<<"sbitset|=\n";
		std::cout<<"\t";print_numbers();std::cout<<"\n";
		std::cout<<_nblocks<<"\n";
		std::cout<<"\t|";bs.print_numbers();std::cout<<"\n";
		std::cout<<bs._nblocks<<"\n";
#endif
		size_t wblokcs = _nblocks <= bs._nblocks ? _nblocks : bs._nblocks;


		for(size_t i = 0; i<wblokcs; i++)
			_bits[i] |= bs._bits[i];
		if(bs._nblocks > _nblocks){
			resize(bs._nblocks * bits_per_block);
			for(size_t i = wblokcs; i<_nblocks; i++)
				_bits[i] = bs._bits[i];
		}
#ifdef BS_CALLS
		std::cout<<"\t=";print_numbers();std::cout<<"\n";
		std::cout<<_nblocks<<"\n";
		std::cout<<"<sbitset|=\n";
#endif
		return *this;
	}

//	sbitset operator&(sbitset& bs){
//#ifdef BS_CALLS
//		std::cout<<"sbitset&\n";
//		std::cout<<"\t";print_numbers();std::cout<<"\n";
//		std::cout<<"\t&";bs.print_numbers();std::cout<<"\n";
//#endif
////		sbitset* r = new sbitset(*this);
////		(*r) &= bs;
//		sbitset r(*this);
//		r &= bs;
//		return r;
//#ifdef BS_CALLS
//		std::cout<<"\t=";print_numbers();std::cout<<"\n";
//		std::cout<<"<sbitset&\n";
//#endif
////		return *r;
//	}

//	sbitset operator|(sbitset& bs){
//#ifdef BS_CALLS
//		std::cout<<"sbitset|\n";
//		std::cout<<"\t";print_numbers();std::cout<<"\n";
//		std::cout<<"\t|";bs.print_numbers();std::cout<<"\n";
//#endif
////		sbitset* r = new sbitset(*this);
////		(*r) |= bs;
//		sbitset r(*this);
//#ifdef BS_CALLS
//		std::cout<<"\t=";print_numbers();std::cout<<"\n";
//		std::cout<<"<sbitset|\n";
//#endif
//		//return *r;
//		return r;
//	}

	sbitset& operator =(sbitset& b){
#ifdef BS_CALLS
		std::cout<<"sbitset=\n";
		std::cout<<"\t ";b.print_numbers();std::cout<<"\n";
#endif
		delete [] _bits;
		_nblocks = b._nblocks;
		_bits = (sbitset_block*)malloc(_nblocks * bytes_per_block);
		memcpy(_bits, b._bits, _nblocks * bytes_per_block);
		return *this;
#ifdef BS_CALLS
		std::cout<<"\t=";print_numbers();std::cout<<"\n";
		std::cout<<"<sbitset=\n";
#endif
	}

	bool operator!=(sbitset& b){
#ifdef BS_CALLS
		std::cout<<"sbitset!=\n";
		std::cout<<"\t";print_numbers();std::cout<<"\n";
		std::cout<<"\t&";b.print_numbers();std::cout<<"\n";
#endif
		size_t wblokcs = _nblocks <= b._nblocks ? _nblocks : b._nblocks;
		for(size_t i = 0; i<wblokcs; i++){
			if(_bits[i] != b._bits[i]){
#ifdef BS_CALLS
		std::cout<<"<sbitset!=true\n";
#endif
				return true;
			}
		}
		if(wblokcs > _nblocks){
			for(size_t i = wblokcs; i<b._nblocks; i++){
				if(b._bits[i] != 0){
#ifdef BS_CALLS
		std::cout<<"<sbitset!=true\n";
#endif
					return true;
				}
			}
		}
		else{
			for(size_t i = wblokcs; i<_nblocks; i++){
				if(_bits[i] != 0){
#ifdef BS_CALLS
		std::cout<<"<sbitset!=true\n";
#endif
					return true;
				}
			}
		}
#ifdef BS_CALLS
		std::cout<<"<sbitset!=false\n";
#endif
		return false;
	}


	bool emptyAND(sbitset& b){
		size_t wblokcs = _nblocks <= b._nblocks ? _nblocks : b._nblocks;
		for(size_t i = 0; i<wblokcs; i++){
			if((_bits[i] & b._bits[i]) != 0){
				return false;
			}
		}
		return true;
	}


	void setAll(size_t size, bool value){
		resizeAsis(size);
		sbitset_block svalue = 0x0;
		sbitset_block mask = value;
//		if(value) 	mask = 0x1;
//		else		mask = 0x0;
		for(size_t i=0; i<bits_per_block; i++){
			svalue |= mask;
			mask = mask << 1;
		}
		for(size_t i=0; i<_nblocks; i++){
			_bits[i] = svalue;
		}
	}
};

#endif /* SBITSET_H_ */
