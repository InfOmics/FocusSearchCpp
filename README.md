# FocusSearchCpp
***Version: 0.3***
> C++ implementation of FocusSearch (originally implemented in Modula2 by Ullmann)


Before use the FocusSearch-C++ command line interface of all distributions, please rebuilt it by make -B
The tool usage is:

```
./fsearch ISO_TYPE INPUT_FORMAT target_graph pattern_graph
```

|ISO_TYPE|specify isomorphism|
|---------------|------------|
|mono|monomorphism, the classical subgraph matching|

|INPUT_FORMAT|	specify input file format|
|---------------|------------|
|gfd	|directed graphs with attributes only on nodes.|
|gfu	|undirected graphs with attributes only on nodes.|
|vfu| Sansone et al. file format for labeled directed graphs with attributes only on nodes.|


##### Default graph file format

The RI project provides two graph file format gfu and gfd, respectively for undirected and directed graphs with attributes only on nodes.

Graphs are stored in text files containing one or more items.
The current input format allows the description of undirect graphs with labels on nodes.
> #[graph_name] <br>
[number of nodes] <br>
[label_of_first_node] <br>
[label_of_second_node] <br>
... <br>
[number of edges] <br>
[node id] [node id] <br>
[node id] [node id] <br>
... <br>

Node ids are assigned following the order in which they are written in the input file, starting from 0.
***[graph_name] and labels can not contain blank characters (spaces).
Labels are case sensitive.***

An example of input file is the following:

> #my_graph <br>
4 <br>
A <br>
B <br>
C <br>
Br <br>
5 <br>
0 1 <br>
2 1 <br>
2 3 <br>
0 3 <br>
0 2 <br>


Indeed, an example of input file in geu format (undirected graph with labels both on nodes and attibutes) is:
> #my_graph <br>
4 <br>
A <br>
B <br>
C <br>
Br <br>
5 <br>
0 1 a <br>
2 1 n<br>
2 3 m<br>
0 3 k<br>
0 2 a<br>

<hr />
