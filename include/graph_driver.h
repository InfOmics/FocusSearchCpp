/*
 * graph_driver.h
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


#ifndef GRAPH_DRIVER_H_
#define GRAPH_DRIVER_H_

#include "mp_GraphReader.h"
#include "mp_GFUGraphReader.h"
#include "mp_GFDGraphReader.h"
#include "mp_EGFUGraphReader.h"
#include "mp_EGFDGraphReader.h"
#include "mp_VFUGraphReader.h"

#include "mp_FGFFGraphReader.h"


enum GRAPH_FILE_TYPE {GFT_GFU, GFT_GFD,  GFT_EGFU, GFT_EGFD,   GFT_FGFF, GFT_GFF, GFT_VFU, GFT_VFL, GFT_FSG, GFT_FSPEEP};


void open_ifstream(std::ifstream& in, const std::string& input_file, GRAPH_FILE_TYPE _graphtype){
	switch(_graphtype){
	case GFT_FGFF:
	case GFT_GFF:
	case GFT_GFU:
//		GFUGraphReader::open_ifstream(in, input_file);
//		break;
	case GFT_GFD:
	case GFT_EGFD:
	case GFT_EGFU:
		GFDGraphReader::open_ifstream(in, input_file);
		break;
	case GFT_FSG:
		break;
	case GFT_FSPEEP:
		//in.open(input_file.c_str(), ios_base::in);
		break;
	case GFT_VFU:
		VFUGraphReader::open_ifstream(in, input_file);
		break;
	case GFT_VFL:
		//in.open(input_file.c_str(), ios_base::in | ios_base::binary );
		break;
	}
};


GraphReader* get_graph_reader(std::ifstream& in, GRAPH_FILE_TYPE _graphtype){
	switch(_graphtype){
		case GFT_FGFF:
			return new FGFFGraphReader(in);
			break;
		case GFT_GFF:
			//readed=read_graph_p0_gff(in, *ed);
			break;
		case GFT_GFU:
			return new GFUGraphReader(in);
			break;
		case GFT_GFD:
			return new GFDGraphReader(in);
			break;
		case GFT_EGFU:
			return new EGFUGraphReader(in);
			break;
		case GFT_EGFD:
			return new EGFDGraphReader(in);
			break;
		case GFT_VFU:
			//readed=read_graph_p0_unlabeled(in, *ed);
			return new VFUGraphReader(in);
			break;
		case GFT_VFL:
			//readed=read_graph_p0_labeled(in, *ed);
			break;
		case GFT_FSG:
			//readed=read_graph_p0_fg(in, *ed);
			break;
		case GFT_FSPEEP:
			//readed=read_graph_fspeep(in, *ed);
			break;
		}

	return NULL;
};



#endif /* GRAPH_DRIVER_H_ */
