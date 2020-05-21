Formats used in this library
============================

In this file, all the file formats used by this project are described.

Input formats
-------------

This part described all the file formats that can be used as input for the `preprocessor` (the mapping file can also be used as input for the `library`).

### XenGraph input format

One of the two input formats that can be used to describe the input graph for the preprocessing. This format can also be used when benchmarking Dijkstra's Algorithm performance.

The format is a plain text file that looks as follows:

* It begins with a line `XGI n e` where `XGI` is a fixed string which serves as a magic constant. `n` is then a positive integer denoting the number of nodes, while `e` is a positive integer denoting the number of edges.
* After that follows exactly `e` lines in the format `s t w f` each representing one edge. In this case, `s` is the source node of the edge, `t` is the target node of the edge, both must be in the range from 0 to `n`-1 (nodes are indexed from 0). `w` is the weight of the edge, in our case a positive integer. `f` is a flag denoting whether the edge is a one way edge. It must be either 1 or 0. If `f` is one, the edge is a one way edge and only an edge from `s` to `t` is added to the graph. If `f` is 0, we treat the edge as a bidirectional edge and therefore two edges are added into the graph, an edge from `s` to `t` and an edge from `t` to `s`, both with the weight `w`. 

The expected suffix for XenGraph files is `.xeng` although it is not enforced.

### DIMACS input format

One of the two input formats that can be used to describe the input graph for the preprocessing.

This input format was used during the 9th DIMACS Implementation Challenge on shortest paths. Example graphs in this format can therefore be downloaded from the website of the challenge: http://users.diag.uniroma1.it/challenge9/download.shtml . The graph file is a plain text file and it looks as follows:

* Everywhere in the file lines beginning with the character `c` can occur. Those lines are comment lines and are skipped during loading.
* First line of the file not starting with the character `c` must be in the format `p sp n e`. Here `p sp` is a fixed string which serves as a magic constant. `n` is a positive integer denoting the number of nodes, `e` is another positive integer denoting the number of edges.
* Then there must be exactly `e` lines of the format `a s t w` each representing one edge. In this case, `a` is a fixed characted constant indicating that the line describes one edge (so it is not a comment line). `s` is the source node of the edge and `t` is the target node of the edge. In this format, nodes are indexed from 1, so both `s` and `t` must be in the range from 1 to `n` (Internally, nodes are indexed from 0, so during loading, each node ID is automatically decreased by one). In this format, each edge is considered to be a one way edge, so if we want a bidirectional edge between `s` to `t`, we must provide two lines, one for an edge from `s` to `t` and one for an edge from `t` to `s`, both with the same weight.

The expected suffix for DIMACS graph files is `.gr` although it is not enforced.

### Query set input format

For easier benchmarking, the user can provide a set of queries which will be used for the benchmark in a very simple plain text format:

* The file begins with a line that contains only a single integer `cnt` denoting the number of queries.
* The first line is followed by exactly `cnt` lines in the format `s g` each denoting one query. `s` and `g` are both positive integers. When benchmarking without mapping, `s` and `g` must be in the range from 0 to `n`-1 where `n` is the number of nodes in the graph. When benchmarking with mapping, `s` and `g` can be arbitrary positive integers as long as they fit in the long long unsigned int datatype (usually 64 bit).

### Mapping file input format

If your application internally for some reason uses some node IDs that are not in the range from 0 to `n`-1 (where `n` is the number of nodes in the graph), you can use a mapping file that will allow the application to transform node IDs from your application to IDs in the range from 0 to `n`-1. Using those mapping files, you can let the library do all the mapping work, so will not need to change the IDs in your application. The mapping file is a plain text file in the following format:

* The file begins with a line `XID n` where `XID` is a fixed string which serves as a magic constant and `n` is the amount of nodes in the graph. 
* The first line is followed by exactly `n` lines each only containing one integer `i`. The `j`-th line represents the original ID of the `(j-2)`-th node. So the second line contains the original ID of the node with the ID 0 in our application. The third line contains the original ID of the node with the ID 1, and so on up to the line `n+1` contains the original ID for the node with the ID `n-1` in our application.

The expected suffix for the mapping files is `.xeni` although it is not enforced.


Data structures formats
-----------------------

### Contraction Hierarchies format

All the data needed for the Contraction Hierarchies shortest distance query algorithm is stored in a binary file that is organised as follows:

* "CH\r\n" (0x43 0x48 0x0d 0x0a)
*    unsigned int: version (currently "1", shold be == compared)
*    unsigned int: number of nodes (= n)
*    unsigned int: number of original edges (= m1)
*    unsigned int: number of shortcut edges (= m2)
*    n times, for each node 0..(n-1):
    *        unsigned int: rank
*    m1 times, original edges:
    * unsigned int: source node
    *        unsigned int: target node
    *        unsigned int: weight
    *        unsigned int: flags
*    m2 times, shortcut edges:
    *        unsigned int: source node
    *        unsigned int: target node
    *        unsigned int: weight
    *        unsigned int: flags
    *        unsigned int: shortcut middle node
*    unsigned int: 0x12345678 as terminator
* possible (bit) flags are:
    *    1 = forward edge
    *    2 = backward edge
    *    4 = shortcut edge
    *    the actual flag is obtained as sum of those, so for example a forward shortcut edge will have the flag set to 5 (1+4)

Contraction Hierarchies data structure files are automatically generated with the `.ch` suffix. This suffix is not enforced when loading the data structure.

### Transit Node Routing format

All the data needed for the Transit Node Routing shortest distance query algorithm is stored in a binary file that is organised as follows:

* "TNRG" (0x54 0x4e 0x52 0x47)
*    unsigned int: number of nodes (= n)
*    unsigned int: number of edges (= e, both original edges and shortcut edges are contained)
*    unsigned int: number of transit nodes (= t)
*    e times, for each edge 0..(e-1):
    *        unsigned int: source node
    *        unsigned int: target node
    *        unsigned int: weight
    *        bool: forward flag (determines whether the edge can be used in the forward direction)
    *        bool: backward flag (determines whether the edge can be used in the backward direction)
*    n times, for each node 0..(n-1):
    *        unsigned int: rank
*    t times:
    *        unsigned int: a transit node ID (indices of all the nodes in the transit node set)
*    t*t times, transit node-set distance table values:
    *        unsigned int: weight (shortest distance from the i-th transit node to the j-th transit node)
*    n times: access nodes
    *   unsigned int: the number of forward access nodes for node i (k)
    * k times: forward access nodes for node i
        * unsigned int: forward access node ID
        * unsigned int: forward access node weight
    *   unsigned int: the number of backward access nodes for node i (l)
    * l times: backward access nodes for node i
        * unsigned int: backward access node ID
        * unsigned int: backward access node weight
*   n times: search spaces
    *     unsigned int: forward search space size (k)
    *     k times: forward search space for node i
        * unsigned int: a node ID of a node in the forward search space of node i
    *     unsigned int: backward search space size (l)
    *     l times: backward search space for node i
        * unsigned int: a node ID of a node in the backward search space of node i

Transit Node Routing data structure files are automatically generated with the `.tnrg` suffix. This suffix is not enforced when loading the data structure.


### Transit Node Routing with Arc Flags format

All the data needed for the Transit Node Routing with Arc Flags shortest distance query algorithm is stored in a binary file that is organised as follows:

* "TGAF" (0x54 0x47 0x41 0x46)
*    unsigned int: number of nodes (= n)
*    unsigned int: number of edges (= e, both original edges and shortcut edges are contained)
*    unsigned int: number of transit nodes (= t)
*    unsigned int: number of regions for Arc Flags (= k)
*    e times, for each edge 0..(e-1):
    *        unsigned int: source node
    *        unsigned int: target node
    *        unsigned int: weight
    *        bool: forward flag (determines whether the edge can be used in the forward direction)
    *        bool: backward flag (determines whether the edge can be used in the backward direction)
*    n times, for each node 0..(n-1):
    *        unsigned int: rank
*    n times, for each node 0..(n-1):
    *        unsigned int: region (for Arc Flags)
*    t times:
    *        unsigned int: a transit node ID (indices of all the nodes in the transit node set)
*    t*t times, transit node-set distance table values:
    *        unsigned int: weight (shortest distance from the i-th transit node to the j-th transit node)
*    n times: access nodes
    *   unsigned int: the number of forward access nodes for node i (x)
    * x times: forward access nodes for node i
        * unsigned int: forward access node ID
        * unsigned int: forward access node weight
        * uint32_t: arc flags for the forward access node encoded into a 32 bit unsigned integer
    *   unsigned int: the number of backward access nodes for node i (y)
    * y times: backward access nodes for node i
        * unsigned int: backward access node ID
        * unsigned int: backward access node weight
        * uint32_t: arc flags for the backward access node encoded into a 32 bit unsigned integer
*   n times: search spaces
    *     unsigned int: forward search space size (x)
    *     x times: forward search space for node i
        * unsigned int: a node ID of a node in the forward search space of node i
    *     unsigned int: backward search space size (y)
    *     y times: backward search space for node i
        * unsigned int: a node ID of a node in the backward search space of node i

Transit Node Routing with Arc Flags data structure files are automatically generated with the `.tgaf` suffix. This suffix is not enforced when loading the data structure.

### Distance Matrix format

The implementation can compute Distance Matrices and answer queries using those matrices. This can not be used directly from the `preprocessor` or the `library`, but the code is present in the implementation. This was used in the thesis during benchmarks (examples can be found in [src/thesisTestScenarios.cpp](./src/thesisTestScenarios.cpp)). The Distance Matrix is stored in a binary file that is organized as follows:


* "XDM" (0x58 0x44 0x4d)
*    unsigned int: number of nodes (= n)
* n*n times, the values for the matrix:
    * unsigned int: value for the field[i][j] in the distance matrix

The Distance Matrices obtained from our implementation automatically have the `.xdm` suffix. This suffix is not enforced when loading the Distance Matrix.



Output format when benchmarking
-------------------------------

When benchmarking, the user can specify an optional file into which the values for all the queries computed by the chosen method will be stored. Those values can be used to verify the correctness of the algorithms. The output format is a very simple plain text file that looks as follows:

* A string containing the name of the query set file (for example `Prague10000randomQueries.txt`).
* After that follows exactly `q` lines where `q` is the amount of queries in the query set. Each line contains exactly one number corresponding to the answer to the query. So the second line contains the answer to the first query, the third line contains the answer to the second query and so on.

