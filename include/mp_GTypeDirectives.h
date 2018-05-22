/*
 * GTypeDirectives.h
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


#ifndef GTYPEDIRECTIVES_H_
#define GTYPEDIRECTIVES_H_


enum DGDirectives{
	none = 0,
	ordered = 1L << 1,
	node_labeled = 1L << 2,
	edge_labeled = 1L << 3
};

inline DGDirectives
operator&(DGDirectives __a, DGDirectives __b)
{ return DGDirectives(static_cast<int>(__a) & static_cast<int>(__b)); }

inline DGDirectives
operator|(DGDirectives __a, DGDirectives __b)
{ return DGDirectives(static_cast<int>(__a) | static_cast<int>(__b)); }

inline DGDirectives
operator^(DGDirectives __a, DGDirectives __b)
{ return DGDirectives(static_cast<int>(__a) ^ static_cast<int>(__b)); }

inline DGDirectives&
operator|=(DGDirectives& __a, DGDirectives __b)
{ return __a = __a | __b; }

inline DGDirectives&
operator&=(DGDirectives& __a, DGDirectives __b)
{ return __a = __a & __b; }

inline DGDirectives&
operator^=(DGDirectives& __a, DGDirectives __b)
{ return __a = __a ^ __b; }

inline DGDirectives
operator~(DGDirectives __a)
{ return DGDirectives(~static_cast<int>(__a)); }

#endif /* GTYPEDIRECTIVES_H_ */
