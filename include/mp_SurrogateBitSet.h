/*
 * SurrogateBitSet.h
 *
 *  Surrogate Bit Set
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


//#include "defdebug.h"

#ifndef SURROGATEBITSET_H_
#define SURROGATEBITSET_H_


#include <iostream>
#include <fstream>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "size_t.h"


//#define SBS_CALLS
//#define SBS_CONSTRUCTORS
//#define SBS_DECONSTRUCTORS
//#define SBS_PRINTINFO


/**nbIn
 * Class SurrogateBitSet : Surrogate Bit Set
 *
 * ============================================================
 *
 * Structure:
 *
 * | empty working blocks | working blocks |
 * | all zeros			  | at least one 1 |
 * | non instantiated	  | instantiated   |
 * |					  |^			  ^|
 * 						fworkindex		lworkindex
 *
 */
class SurrogateBitSet{
	/* ============================================================
	 * Types Declarations
	 * ============================================================
	 */
	//typedef unsigned char sbitset_block_t;
	typedef uint32_t sbitset_block_t;
	static const u_size_t block_size = sizeof(sbitset_block_t)*8;

	/* ============================================================
	 * Fields
	 * ============================================================
	 */
public:
	u_size_t _nwblocks;		//number of working blocks
	u_size_t _neblocks;		//number of starting empty blocks, not instantiated
	sbitset_block_t* _bits;	//data, working blocks
	u_size_t _fworkindex;		//first working index, it is a multiple of block_size + 1
	u_size_t _lworkindex;		//last working index, it is a multiple of block_size

public:
	/* ============================================================
	 * Constructors
	 * ============================================================
	 */

	SurrogateBitSet(){
		_nwblocks = 0;
		_neblocks = 0;
		_fworkindex = 0;
		_lworkindex = 0;
		_bits = NULL;
	}

	//SurrogateBitSet(const SurrogateBitSet& c){
	SurrogateBitSet(SurrogateBitSet& c){
		_nwblocks = c._nwblocks;
		_neblocks = c._neblocks;
		_fworkindex = c._fworkindex;
		_lworkindex = c._lworkindex;
		if(c._bits == NULL){
			_bits = NULL;
		}
		else{
			_bits = (sbitset_block_t*)calloc(_nwblocks, block_size);
			for(u_size_t i=0;i<_nwblocks;i++){
				_bits[i] = c._bits[i];
			}
		}
#ifdef SBS_CALLS
		std::cout<<">>SBS.(SBS)\n";
		std::cout<<"\t [";print_numbers();std::cout<<"]\n";
		std::cout<<"\t =[";c.print_numbers();std::cout<<"]\n";
#endif
	}

	/**
	 * Warning : inconsistent surrogate property, since all bits are set to zero.
	 */
	SurrogateBitSet(u_size_t first_index, u_size_t last_index){
		if(first_index > last_index){
			u_size_t temp = first_index;
			first_index = last_index;
			last_index = first_index;
		}

		u_size_t first_iblock = iblock_of(first_index);
		u_size_t last_iblock = iblock_of(last_index);

		_neblocks = first_iblock;
		_nwblocks = last_iblock - first_iblock +1;
		_fworkindex = first_iblock * block_size;
		_lworkindex = ((last_iblock+1) * block_size) -1;
		_bits = (sbitset_block_t*)calloc(_nwblocks, block_size);
	}

	~SurrogateBitSet(){
#ifdef SBS_DECONSTRUCTORS
	std::cout<<"~SurrogateBitSet\n";
#endif
		if(_bits != NULL){
			delete [] _bits;
		}
#ifdef SBS_DECONSTRUCTORS
	std::cout<<"~SurrogateBitSet<\n";
#endif
	}

	bool is_empty(){
		//return _nwblocks == 0;
		/*
		 * Now it works also with not properly surrogate set
		 */
		if(_nwblocks == 0)	return true;
		else{
			bool empty = true;
			for(u_size_t i=0; i<_nwblocks && empty; i++){
				if(_bits[i] != 0)
					empty = false;
			}
			return empty;
		}

	}

	bool at_least_one(){
		iterator IT = first_ones();
		return IT!=end();
	}
	/* ============================================================
	 * Support
	 * ============================================================
	 */

private:
	u_size_t iblock_of(u_size_t index){
		return (u_size_t)((index / block_size));
	}

	void set_windexes(){
		if(_nwblocks == 0){
			_fworkindex = 0;
			_lworkindex = 0;
		}
		else{
			_fworkindex = _neblocks * block_size;
			_lworkindex = (( _neblocks + _nwblocks ) * block_size) -1;
		}
	}

	void delete_blanks(sbitset_block_t **bits, u_size_t* neblocks, u_size_t* nwblocks){
#ifdef SBS_CALLS
		std::cout<<">SBS.delete_blanks("<<(*neblocks)<<","<<(*nwblocks)<<" )\n";
		//std::cout<<(*bits)[0]<<"\n";
#endif

#ifdef SBS_PRINTINFO
		//std::cout<<"\t [";print_numbers();std::cout<<"]\n";
		std::cout<<(*bits)[0]<<"\n";
#endif

		if(bits != NULL){
			u_size_t i=0;

			u_size_t istart = *neblocks;
			while(i<(*nwblocks) && (*bits)[i]==0){
				istart++;
				i++;
			}

			if(istart >= (*neblocks)+(*nwblocks)){
				//allzeros
				*neblocks = 0;
				*nwblocks = 0;
				delete [] (*bits);
				(*bits) = NULL;
#ifdef SBS_CALLS
//		if((*bits) == NULL) std::cout<<"set bits NULL\n";
//		else std::cout<<"error on set bits NULL\n";
		std::cout<<"<SBS.delete_blanks-empty\n";
#endif
				return;
			}

			u_size_t isize = *nwblocks;
			i = isize -1;
			while(i>=0 && (*bits)[i]==0){
				isize--;
				i--;
			}

#ifdef SBS_PRINTINFO
			std::cout<<"istart= "<<istart<<"\n";
			std::cout<<"isize= "<<isize<<"\n";
#endif

			if(istart != (*neblocks)  ||  isize!=(*nwblocks)){
				//trim - left and/or right

				sbitset_block_t* n_bits = (sbitset_block_t*)calloc(isize - (istart - (*neblocks)), block_size);

#ifdef SBS_PRINTINFO
				std::cout<<"new size= "<<isize - (istart - (*neblocks))<<"\n";
				std::cout<< &(bits[istart - (*neblocks)])<<"\n";
				std::cout<< (void*)(bits[istart - (*neblocks)])<<"\n";
				std::cout<<&(n_bits[0])<<"\n";
				std::cout<<n_bits<<"\n";
#endif

				//memcpy(&(n_bits[0]), &(bits[istart - (*neblocks)]), isize - (istart - (*neblocks)));
				//memcpy(&(n_bits[0]), &(bits[istart - (*neblocks)]), (isize - (istart - (*neblocks)))*block_size);
				memcpy(n_bits, (&(*bits)[istart - (*neblocks)]), (isize - (istart - (*neblocks)))*block_size);

				delete [] (*bits);
				(*bits) = n_bits;

				*neblocks = istart;
				*nwblocks = isize - (istart - (*neblocks));
			}
		}
#ifdef SBS_CALLS
		//std::cout<< (*bits)[0]<<"\n";
		std::cout<<"<SBS.delete_blanks\n";
#endif
	}

public:
	void surrogate(){
		delete_blanks(&_bits, &_neblocks, &_nwblocks);
		set_windexes();
	}

	/* ============================================================
	 * Resize
	 * ============================================================
	 */

public:
	void resize(u_size_t index){
		if(_bits == NULL){
			resize(index, index);
		}
		else{
			if(index < _fworkindex)
				resize(index, _lworkindex);
			if(index > _lworkindex)
				resize(_fworkindex, index);
		}
	}

	/**
	 * first_index and last_index are not  block indexes
	 * but they are absolute positions
	 */
	void resize(u_size_t first_index, u_size_t last_index){
		if(first_index > last_index){
			u_size_t temp = first_index;
			first_index = last_index;
			last_index = first_index;
		}

		u_size_t first_iblock = iblock_of(first_index);
		u_size_t last_iblock = iblock_of(last_index);

		u_size_t o_first_iblock = _neblocks;
		u_size_t o_last_iblock = _neblocks + _nwblocks -1;
		u_size_t o_neblocks = _neblocks;
		u_size_t o_nwblocks = _nwblocks;

		_neblocks = first_iblock;
		_nwblocks = last_iblock - first_iblock +1;

		//sbitset_block_t* n_bits = (sbitset_block_t*)calloc(_nwblocks, block_size);
		sbitset_block_t* n_bits = (sbitset_block_t*)malloc(_nwblocks * block_size);

		if(_bits != NULL){

			if( (first_iblock <= o_first_iblock && o_first_iblock <= last_iblock) ||
				(first_iblock <= o_last_iblock && o_last_iblock <= last_iblock)	||
				(first_iblock >= o_first_iblock && o_last_iblock >= last_iblock )
				){
				//intersection
				/*n_btis[*/	u_size_t a =	(first_iblock >= o_first_iblock) ? 0 : o_first_iblock - _neblocks;
				/*_bits[*/	u_size_t b =	(first_iblock <= o_first_iblock) ? 0 : first_iblock - o_neblocks;
				/*num*/		u_size_t c =	(last_iblock <= o_last_iblock) ?
											(last_iblock - (b + o_neblocks) + 1)*block_size :
											(o_last_iblock - (b + o_neblocks) + 1)*block_size;


				memcpy( &(n_bits[a]), &(_bits[b]), c);  //TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			}

			delete [] _bits;
		}

		_bits = n_bits;

		set_windexes();
//		_fworkindex = first_iblock * block_size;
//		_lworkindex = ((last_iblock+1) * block_size) -1;
	}


	/* ============================================================
	 * Get and Set
	 * ============================================================
	 */

	void set(u_size_t index, bool value){
#ifdef SBS_CALLS
		std::cout<<">>SBS.set("<<index<<",";
		if(value)std::cout<<"true)\n";
		else std::cout<<"false)\n";
#endif

		resize(index);

		if(value != get(index)){
			index = index - _fworkindex;
			_bits[index/block_size] ^= 1<<(block_size-(index%block_size)-1);
		}
	}

	bool get(u_size_t index){
		if(index >= _fworkindex && index <= _lworkindex){
			index = index - _fworkindex;
			return _bits[index/block_size]&(1<<(block_size-(index%block_size)-1));
		}
		return 0;
	}


	/* ============================================================
	 * Operators
	 * ============================================================
	 */

	//SurrogateBitSet& operator&=(const SurrogateBitSet& bs){
	SurrogateBitSet& operator&=(SurrogateBitSet& bs){
#ifdef SBS_CALLS
		std::cout<<">>SBS.&=\n";
		std::cout<<"\t [";print_numbers();std::cout<<"]\n";
		std::cout<<"\t [";bs.print_numbers();std::cout<<"]\n";
#endif
		if(_bits == NULL || bs._bits == NULL){
			_nwblocks = 0;
			_neblocks = 0;
//			_fworkindex = 0;
//			_lworkindex = 0;
			delete [] _bits;
			_bits = NULL;
		}
		else{
			u_size_t t_fiblock = iblock_of(_fworkindex);
			u_size_t t_liblock = iblock_of(_lworkindex);
			u_size_t bs_fiblock = iblock_of(bs._fworkindex);
			u_size_t bs_liblock = iblock_of(bs._lworkindex);

			if( (bs_fiblock <= t_fiblock && t_fiblock <= bs_liblock) ||
					//	this		|f		|
					//	bs		|f		l|
				(bs_fiblock <= t_liblock && t_liblock <= bs_liblock) ||
					//	this		|		l|
					//	bs				|f		l|
				(t_fiblock <= bs_fiblock && bs_liblock <= t_liblock)
					//	this	|f				l|
					//	bs			|f		l|
				){
				//intersection
				u_size_t n_neblocks = (t_fiblock > bs_fiblock) ? t_fiblock : bs_fiblock;
				u_size_t n_nwblocks = (t_liblock < bs_liblock) ? t_liblock - n_neblocks + 1 : bs_liblock - n_neblocks + 1;


//std::cout<<"new size = "<<n_nwblocks<<"\n";

				//sbitset_block_t* n_bits = (sbitset_block_t*)calloc(n_nwblocks, block_size);
				sbitset_block_t* n_bits = (sbitset_block_t*)malloc(n_nwblocks * block_size);

				for(int i=n_neblocks; i<n_neblocks + n_nwblocks; i++){
					if((t_fiblock <= i && i <= t_liblock) && (bs_fiblock <= i && i <= bs_liblock)){
//std::cout<<"copy bits["<<i-_neblocks<<"]\n";
						n_bits[i-n_neblocks] = _bits[i-_neblocks] & bs._bits[i-bs._neblocks];
					}

				}

//std::cout<<"n_bits[0]= "<<n_bits[0]<<"\n";

				delete [] _bits;  //TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

				_nwblocks = n_nwblocks;
				_neblocks = n_neblocks;

				delete_blanks(&n_bits, &_neblocks, &_nwblocks);

//				if(n_bits == NULL) std::cout<<"n_bits == NULL\n";
//				else std::cout<<"n_bits != NULL\n";

				_bits = n_bits;

//				if(_bits == NULL) std::cout<<"_bits == NULL\n";
//				else std::cout<<"_bits != NULL\n";

//				_nwblocks = n_nwblocks;
//				_neblocks = n_neblocks;

//				_fworkindex = _neblocks * block_size;
//				_lworkindex = (( _neblocks + _nwblocks ) * block_size) -1;
			}
			else{
				delete [] _bits;
				_bits = NULL;
				_nwblocks = 0;
				_neblocks = 0;
//				_fworkindex = 0;
//				_lworkindex = 0;
			}
		}

		set_windexes();

#ifdef SBS_CALLS
//		if(_bits == NULL) std::cout<<"\t _bits[0]= NULL\n";
//		else std::cout<<"\t _bits[0]="<<_bits[0]<<"\n";
		std::cout<<"\t =[";print_numbers();std::cout<<"]\n";
#endif
		return *this;
	}

	//SurrogateBitSet& operator|=(const SurrogateBitSet& bs){
	SurrogateBitSet& operator|=(SurrogateBitSet& bs){
#ifdef SBS_CALLS
		std::cout<<">>SBS.|=\n";
		std::cout<<"\t [";print_numbers();std::cout<<"]\n";
		std::cout<<"\t [";bs.print_numbers();std::cout<<"]\n";
#endif
		if(_bits == NULL && bs._bits == NULL){
			_bits = NULL;
			_nwblocks = 0;
			_neblocks = 0;
//			_fworkindex = 0;
//			_lworkindex = 0;
		}
		else if(_bits == NULL){
			_nwblocks = bs._nwblocks;
			_neblocks = bs._neblocks;
//			_fworkindex = bs._fworkindex;
//			_lworkindex = bs._lworkindex;
			_bits = (sbitset_block_t*)calloc(_nwblocks, block_size);
			for(int i=0; i<_nwblocks; i++){
				_bits[i] = bs._bits[i];
			}

		}
		else if(bs._bits == NULL){
		}
		else{
			u_size_t t_fiblock = iblock_of(_fworkindex);
			u_size_t t_liblock = iblock_of(_lworkindex);
			u_size_t bs_fiblock = iblock_of(bs._fworkindex);
			u_size_t bs_liblock = iblock_of(bs._lworkindex);

			u_size_t n_neblocks = (t_fiblock < bs_fiblock) ? t_fiblock : bs_fiblock;
			u_size_t n_nwblocks = (t_liblock > bs_liblock) ? t_liblock - n_neblocks + 1 : bs_liblock - n_neblocks + 1;

			sbitset_block_t* n_bits = (sbitset_block_t*)calloc(n_nwblocks, block_size);


			bool in_this = false, in_bs = false;;
			for(int i=n_neblocks; i<n_neblocks + n_nwblocks; i++){
				in_this = t_fiblock <= i && i <= t_liblock;
				in_bs = bs_fiblock <= i && i <= bs_liblock;

				if(in_this && in_bs){
					n_bits[i-n_neblocks] = _bits[i-_neblocks] | bs._bits[i-bs._neblocks];
				}
				else if(in_this){
					n_bits[i-n_neblocks] = _bits[i-_neblocks];
				}
				else if(in_bs){
					n_bits[i-n_neblocks] = bs._bits[i-bs._neblocks];
				}
				else{}

			}

			delete [] _bits;
			_nwblocks = n_nwblocks;
			_neblocks = n_neblocks;
			delete_blanks(&n_bits, &_neblocks, &_nwblocks);
			_bits = n_bits;

//			_nwblocks = n_nwblocks;
//			_neblocks = n_neblocks;


			//delete_blanks(n_bits, &_neblocks, &_nwblocks);
//			_fworkindex = _neblocks * block_size;
//			_lworkindex = (( _neblocks + _nwblocks ) * block_size) -1;
		}

		set_windexes();

#ifdef SBS_CALLS
		std::cout<<"\t =[";print_numbers();std::cout<<"]\n";
#endif
		return *this;
	}


	//SurrogateBitSet& operator&(const SurrogateBitSet& bs){
	SurrogateBitSet& operator&(SurrogateBitSet& bs){
		SurrogateBitSet* r = new SurrogateBitSet(*this);
#ifdef SBS_CALLS
		std::cout<<">>SBS.&\n";
		std::cout<<"\t [";r->print_numbers();std::cout<<"]\n";
		std::cout<<"\t [";bs.print_numbers();std::cout<<"]\n";
#endif
		(*r) &= bs;
#ifdef SBS_CALLS
		std::cout<<"\t =[";r->print_numbers();std::cout<<"]\n";
#endif
		return *r;
	}

	//SurrogateBitSet& operator|(const SurrogateBitSet& bs){
	SurrogateBitSet& operator|(SurrogateBitSet& bs){
		SurrogateBitSet* r = new SurrogateBitSet(*this);
#ifdef SBS_CALLS
		std::cout<<">>SBS.|\n";
		std::cout<<"\t [";r->print_numbers();std::cout<<"]\n";
		std::cout<<"\t [";bs.print_numbers();std::cout<<"]\n";
#endif
		(*r) |= bs;
#ifdef SBS_CALLS
		std::cout<<"\t =[";r->print_numbers();std::cout<<"]\n";
#endif
		return *r;
	}

	SurrogateBitSet& operator =(SurrogateBitSet& b){
		_nwblocks = b._nwblocks;
		_neblocks = b._neblocks;
		_fworkindex = b._fworkindex;
		_lworkindex = b._lworkindex;
		if(b._bits == NULL){
			_bits = NULL;
		}
		else{
			//_bits = (sbitset_block_t*)calloc(_nwblocks, block_size);
			_bits = (sbitset_block_t*)malloc(_nwblocks * block_size);
//			for(u_size_t i=0;i<_nwblocks;i++){
//				_bits[i] = b._bits[i];
//			}
			memcpy(_bits, b._bits, _nwblocks*block_size);
		}
#ifdef SBS_CALLS
		std::cout<<">>SBS.=\n";
		std::cout<<"\t [";b.print_numbers();std::cout<<"]\n";
		std::cout<<"\t =[";print_numbers();std::cout<<"]\n";
#endif
	}

	//bool operator!=(const SurrogateBitSet& b){
	bool operator!=(SurrogateBitSet& b){
#ifdef SBS_CALLS
		std::cout<<">>SBS.!=\n";
		std::cout<<"\t [";b.print_numbers();std::cout<<"]\n";
		std::cout<<"\t [";print_numbers();std::cout<<"]\n";
#endif
		if(this->_fworkindex == b._fworkindex && this->_lworkindex == b._lworkindex){
			for(u_size_t i=0; i<this->_nwblocks; i++){
				if(this->_bits[i] != b._bits[i]){
#ifdef SBS_CALLS
	std::cout<<"\t =true\n";
#endif
					return true;
				}
			}
#ifdef SBS_CALLS
	std::cout<<"\t =fasle\n";
#endif
			return false;
		}
#ifdef SBS_CALLS
	std::cout<<"\t =true\n";
#endif
		return true;
	}

	/* ============================================================
	 * Iterators
	 * ============================================================
	 */
	class iterator{
		SurrogateBitSet& _sb;
		u_size_t _wblock;
		unsigned short _shift;
	public:
		u_size_t first;	//position
		bool second;	//value
	public:
		iterator(SurrogateBitSet& sb) : _sb(sb){
			_wblock = 0;
			_shift = block_size - 1;
			get_comps();
		}
		iterator(SurrogateBitSet& sb, u_size_t wblock) : _sb(sb){
			_wblock = wblock;
			_shift = block_size - 1;
			get_comps();
		}
		iterator(SurrogateBitSet& sb, u_size_t wblock, unsigned short shift) : _sb(sb){
			_wblock = wblock;
			_shift = shift;
			get_comps();
		}
	private:
		void get_comps(){
			first = (_wblock * block_size) + block_size - _shift - 1 + (_sb._neblocks * block_size);
			if(_sb._bits!=NULL && _wblock<_sb._nwblocks){
				second =  _sb._bits[_wblock] & 1<<(_shift);
			}
			else{
				second = false;
			}
		}
	public:
		void print_i(){
			std::cout<<_wblock<<"::"<<_shift<<"\n";
		}
		iterator& operator++(){
			if(_shift == 0){
				_wblock++;
				_shift = block_size - 1;
			}
			else{
				_shift--;
			}

			get_comps();
			return (*this);
		}
		void operator++ (int){
		   ++(*this);
		}
		void next_ones(){
			do{
				 ++(*this);
			}while(!second && _wblock<_sb._nwblocks);
		}

		iterator& operator--(){
			if(_shift == block_size - 1){
				if(_wblock == 0){
					_shift = block_size;
				}
				else{
					_wblock--;
					_shift = 0;
				}
			}
			else{
				_shift++;
			}
			get_comps();
			return (*this);
		}
		void operator-- (int){
		   --(*this);
		}
		void prev_ones(){
			do{
				 --(*this);
			}while(!second && (_wblock>0 || (_wblock==0 && _shift<block_size)));
		}


		const bool operator==(const iterator& IT){
			return &_sb == &(IT._sb) && _wblock == IT._wblock &&  _shift == IT._shift;
		}
		const bool operator!=(const iterator& IT){
			if( &_sb != &(IT._sb) || _wblock != IT._wblock ||  _shift != IT._shift)
				return true;
			return false;
		}
	};

	iterator& begin(){
		if(is_empty())
			return end();

		iterator IT(*this);
		return IT;
	}
	iterator& first_ones(){
		if(is_empty())
			return end();

		iterator IT(*this);
		if(IT.second != 1)
			IT.next_ones();
		return IT;
	}
	iterator& last_ones(){
		if(is_empty()){
			return rend();
		}

		iterator IT(*this, _nwblocks, block_size - 1);
		iterator ITrend = rend();
		IT--;
		while(IT.second != 1 && IT!=ITrend){
			IT.prev_ones();
		}
		return IT;
	}
	iterator& end(){
		iterator IT(*this, _nwblocks);
		return IT;
	}
	iterator& rend(){
		iterator IT(*this, 0, block_size);
		return IT;
	}


public:
	void print_info(){
		std::cout<<"SurrogateBitSet:\n";
		std::cout<<"\tblock_size: "<<block_size<<"\n";
		std::cout<<"\t_nwblocks: "<<_nwblocks<<"\n";
		std::cout<<"\t_neblocks: "<<_neblocks<<"\n";
		std::cout<<"\t_fworkindex: "<<_fworkindex<<"\n";
		std::cout<<"\t_lworkindex: "<<_lworkindex<<"\n";
		if(_bits != NULL){
			std::cout<<"\t";
			for(int i=0; i<_nwblocks; i++){
				std::cout<<_bits[i]<<"|";
			}
			std::cout<<"\n";
		}
	}

	void print_info(const SurrogateBitSet& bs){
		std::cout<<"SurrogateBitSet:\n";
		std::cout<<"\tblock_size: "<<bs.block_size<<"\n";
		std::cout<<"\t_nwblocks: "<<bs._nwblocks<<"\n";
		std::cout<<"\t_neblocks: "<<bs._neblocks<<"\n";
		std::cout<<"\t_fworkindex: "<<bs._fworkindex<<"\n";
		std::cout<<"\t_lworkindex: "<<bs._lworkindex<<"\n";
		if(bs._bits != NULL){
			std::cout<<"\t";
			for(int i=0; i<bs._nwblocks; i++){
				std::cout<<bs._bits[i]<<"|";
			}
			std::cout<<"\n";
		}
	};

	void print_numbers(){
		for(SurrogateBitSet::iterator IT = first_ones(); IT!=end(); IT.next_ones()){
			std::cout<<IT.first<<" ";
		}
	};
};


#endif /* SURROGATEBITSET_H_ */

