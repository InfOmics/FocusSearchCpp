/*
 * FGFFGraphReader.h
 *
 *  Created on: Mar 15, 2012
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


#ifndef FGFFGRAPHREADER_H_
#define FGFFGRAPHREADER_H_


#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "size_t.h"

#include "mp_GTypeDirectives.h"
#include "mp_GraphReader.h"


class FGFFGraphReader : public GraphReader{
private:

	std::ifstream& in;
	u_size_t c_id;

	enum State {
		AttendGraph,
		LoadNumNodes,
		LoadNodes,
		LoadNumEdges,
		LoadEdges,
		End,
		Error
	};
	enum EState{
		FirstEdge,
		SecondEdge
	};


public:
	FGFFGraphReader(std::ifstream& _in): in(_in){
		c_id = 0;
	}

	~FGFFGraphReader(){
	}

	static void open_ifstream(std::ifstream& in, const std::string& input_file){
		in.open(input_file.c_str(), std::ios_base::in);
	}

	/**
	 * Read next graph into container
	 */
	virtual DynamicGraph* read(){
		DynamicGraph* ed = new DynamicGraph(c_id++, ordered);

		State state = AttendGraph;
		EState estate = FirstEdge;
		std::string s;
		int itemcount=0, i=0;
		int firste, seconde;

		while(!in.eof() && state!=End && state!=Error){
			in>>s;
			if(s==""){
				state = Error;
				break;
				//return false;
			}
			switch (state){
				case AttendGraph:{
					if(s[0]=='#'){
						//_graph.name=s;
						state=LoadNumNodes;
					}
					else{
						state = Error;
						break;
						//return false;
					}
					break;
				}
				case LoadNumNodes:{
					//itemcount=stringto<int>(s);
					itemcount=atoi(s.c_str());
					if(itemcount==0){
						state = Error;
						break;
						//return false;
					}
					i=0;
					state=LoadNodes;
					break;
				}
				case LoadNodes:{
					ed->insert_node(NULL);
					i++;
					if(i>=itemcount)
						state=LoadNumEdges;

					break;
				}
				case LoadNumEdges:{
					//itemcount=stringto<int>(s);
					itemcount=atoi(s.c_str());
					i=0;
					state=LoadEdges;
					break;
				}
				case LoadEdges:{
					if(i>=itemcount){
						state=End;
					}
					else{
						if(estate==FirstEdge){
							firste=atoi(s.c_str());
							estate=SecondEdge;
						}
						else{
							seconde=atoi(s.c_str());
							ed->insert_edge(firste, seconde, NULL);
							//ed->insert_edge(seconde, firste, NULL);
							estate=FirstEdge;
							i++;
						}
						if(i>=itemcount){
							state=End;
						}
					}
					break;
				}
			}
		};




		if(state!=End){
			delete ed;
			return NULL;
		}
		else{
			return ed;
		}
		//return true;
	}

	/**
	 * Go forward to graph number pos
	 */
	virtual bool forward(u_size_t pos){
		//TODO
		return false;
	}
};

#endif /* FGFFGRAPHREADER_H_ */
