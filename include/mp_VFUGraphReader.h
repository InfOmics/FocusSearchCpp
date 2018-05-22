/*
 * mp_VFUGraphReader.h
 *
 *  Created on: Mar 14, 2012
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


#ifndef MP_VFUGRAPHREADER_H_
#define MP_VFUGRAPHREADER_H_




#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "size_t.h"

#include "mp_GTypeDirectives.h"
#include "mp_GraphReader.h"


class VFUGraphReader : public GraphReader{
public:
	std::ifstream& in;
	u_size_t c_id;

	VFUGraphReader(std::ifstream& _in): in(_in){
		c_id = 0;
	}

	~VFUGraphReader(){
	}

	static void open_ifstream(std::ifstream& in, const std::string& input_file){
		in.open(input_file.c_str(), std::ios_base::in | ios_base::binary);
	}

	unsigned short p0_read_word_unlabeled(std::ifstream& in){
		unsigned char b1, b2;
		b1=in.get();
		b2=in.get();
		//std::cout<<(unsigned short)(b1 | b2)<<".";
		//return b1 | b2;
		return b1 | (b2 << 8);
	}


	bool read_graph_unlabeled(std::ifstream& in, DynamicGraph& ed){
		int nodes;
		int edges;
		int target;
		int i, j;

		if(!in.good())return false;
		nodes=p0_read_word_unlabeled(in);

		for(int i=0;i<nodes;i++){
			if(!in.good())return false;
			ed.insert_node(NULL);

			edges=p0_read_word_unlabeled(in);

			for(int j=0;j<edges;j++){
				if(!in.good())return false;
				target=p0_read_word_unlabeled(in);

				ed.insert_edge(i, target, NULL);
			}
		}
		return true;
	};

	virtual DynamicGraph* read(){
		DynamicGraph* ed = new DynamicGraph(c_id++, ordered);
		if(read_graph_unlabeled(in, *ed))
			return ed;
		return NULL;
	}

	virtual bool forward(u_size_t pos){
		//TODO
		return false;
	}
};

#endif /* MP_VFUGRAPHREADER_H_ */
