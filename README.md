Shortest Paths computation library in C++
=========================================

This project implements multiple complex preprocessing methods for shortest distance computation in directed weighted graphs. The implemented methods are Contraction Hierarchies, Transit Node Routing (based on Contraction Hierarchies) and Transit Node Routing with Arc Flags (extension of Transit Node Routing).

The project is split into two major components. One component is the **preprocessor** (an executable called `shortestPathsPreprocessor`) which takes an arbitrary graph in a supported format (described later) and prepares the structures required for the query algorithms of one of the three methods. Additionally, the preprocessor allows the user to run a set of queries using some method and benchmark the time required to answer them. This way, the user can easily evaluate whether the performance is sufficient for his use case. The second component is the **library** (a shared library called `libshortestPaths.so` in Linux), which can load the structures prepared by the preprocessor to answer arbitrary queries using the query algorithms of the previously mentioned methods. The library can be used in some bigger `C++` application, but it can be also used from some other language. A simple example application written in `Java` which uses the library can be found in the `javatests` subdirectory.

Links
=====

If you want to make this library running with Amod-to-agentpolis as fast as possible, you can check the [Amod readme](./AMOD_README.md) for step-by-step instructions on how to accomplish this.

If you need to get some information about the formats for the files used in this library (for example the formats used for the Transit Node Routing data structure files), you can check [this](./FORMATS.md) text file which describes all the formats used by this library.


Preprocessor
============

Compiling
---------

To use the preprocessor, we first need to compile it. Compilation should be fairly easy. First, run `CMake` in the root of this project. This should create a `Makefile` with a target called `shortestPathsPreprocessor` that will build the preprocessor application.

In Linux, the process can be done as follows:
* `mkdir cmakedata`
* `cd cmakedata`
* `cmake ..`
* `make shortestPathsPreprocessor`

Usage
-----

The preprocessor has a simple **command line interface** (CLI) that should make the process of preprocessing graphs quite simple.

Let us now assume that you have a directed weighted graph in one of the given formats. The formats are described under this part. There is also a `Python` script prepared for transformation of `GeoJSON` data to one of the supported formats.

The command line interface requires you to use specific arguments in the correct order. The first argument determines whether you want to preprocess a graph file or benchmark a preprocessed graph using some method. The following arguments are specific to each of those two.

### Graph preprocessing

To preprocess a graph using Contraction Hierarchies, simply call the preprocessor with the following arguments:
`./shortestPathsPreprocessor create ch [xengraph/dimacs] [input_file] [output_file]`.
Here the third argument must be exactly `xengraph` or `dimacs` and it determines which input format is used. The fourth argument is your input file in the chosen format, and the last argument is the output file. A suffix `.ch` will be automatically added to the path. An example of a correct call is: `./shortestPathsPreprocessor create ch xengraph my_graph.xeng my_graph`.

To preprocess a graph for Transit Node Routing, the arguments are as follows:
`./shortestPathsPreprocessor create tnr [xengraph/dimacs] [fast/slow/dm] [tnodes_cnt] [input_file] [output_file]`. The third argument again determines which input format is used. The fourth argument determines which preprocessing mode should be used. The `fast` mode will precompute the structures in the smallest time out of the three, but the performance of the resulting data structure will be significantly lower than for the other two modes. Modes `slow` and `dm` will produce exactly the same result, but the `dm` mode uses distance matrix to speed up the precomputation. This means that the `dm` mode requires a lot of memory, but the precomputation will be fairly quick. The `slow` mode needs significantly less memory, but takes more time. The fifth argument determines the size of the transit nodes set. Therefore it must be an integer between 1 and the number of nodes in the graph. Less transit nodes usually mean lower memory requirements, but also worse query times. By choosing the appropriate size of the transit node set, you can find a great balance between memory requirements and performance. The sixth argument is your input file in the chosen format, and the last argument is the output file. A suffix `.tnrg` will be automatically added to the output path. An example of a correct call: `./shortestPathsPreprocessor create tnr xengraph dm 1000 my_graph.xeng my_graph`.

To preprocess a graph for Transit Node Routing with Arc Flags, the arguments are as follows: `./shortestPathsPreprocessor create tnraf [xengraph/dimacs] [slow/dm] [tnodes_cnt] [input_file] [output_file]`. The third argument determines which input format is used. The fourth argument again switches between preprocessing modes. Both modes return the exact same result, but the `dm` mode is faster while requiring more memory. The fifth argument again determines the size of the transit node set (it must be an integer between 1 and the number of nodes in the graph). The sixth argument is your input file in the chosen format, the last argument is the output file. A suffix `.tgaf` will be automatically added to the output path. An example of a correct call: `./shortestPathsPreprocessor create tnraf xengraph dm 1000 my_graph.xeng my_graph`.

### Benchmarking

For benchmarking, you will need a set of queries which can either use IDs in the range from 0 to n-1 (where n is the number of the nodes in the graph) or you can use arbitrary integer node IDs. In the second case, you also need to provide a mapping file. The formats of the query set file and the mapping file are described in the next part. During benchmarking, all of your queries are answered using chosen method, then the total time need to answer all the queries in seconds is printed alongside the average time needed to answer one query in milliseconds.

To benchmark without mapping, your call should look as follows: `./shortestPathsPreprocessor benchmark [ch/tnr/tnraf] nomapping [input_data_structure] [query_set]`. Here the second argument determines which method you will be benchmarking. The fourth argument is the path to the data structure used for the benchmark. You must provide a structure preprocessed for the method you are benchmarking, so for example if you want to benchmark `tnraf`, you must provide `your_structure.tgaf` as your fourth argument. The last argument is the path to your query set that will be used for the benchmark.

If you want to benchmark with mapping, your call should look as follows: `./shortestPathsPreprocessor benchmark [ch/tnr/tnraf] mapping [input_data_structure] [query_set] [mapping_file]`. Here, the first five arguments have the same meaning as in the case without mapping. The additional last argument is the path to the mapping file, which will be used to transform node IDs from your query set to the corresponding node IDs used by the query algorithms.

Example of a Transit Node Routing benchmark call without mapping: `./shortestPathsPreprocessor benchmark tnr nomapping my_graph.tnrg my_query_set.txt`

Example of a Transit Node Routing with Arc Flags benchmark call with mapping: `./shortestPathsPreprocessor benchmark tnraf mapping my_graph.tgaf my_query_set.txt my_mapping.xeni`

Input formats
-------------

In this part, we will describe all the input formats that can be used with the preprocessor application. The first two formats are two input formats for directed weighted graphs that can be used as input for the creation of data structures for one of the methods. The other two formats are related to benchmarking.

### XenGraph input format

Probably the simplier format for directed weighted graphs. Graphs are represented as plain text files in this format. The graph file looks as follows:

* It begins with a line `XGI n e` where `XGI` is a fixed string which serves as a magic constant. `n` is then a positive integer denoting the number of nodes, while `e` is a positive integer denoting the number of edges.
* After that follows exactly `e` lines in the format `s t w f` each representing one edge. In this case, `s` is the source node of the edge, `t` is the target node of the edge, both must be in the range from 0 to `n`-1 (nodes are indexed from 0). `w` is the weight of the edge, in our case a positive integer. `f` is a flag denoting whether the edge is a one way edge. It must be either 1 or 0. If `f` is one, the edge is a one way edge and only an edge from `s` to `t` is added to the graph. If `f` is 0, we treat the edge as a bidirectional edge and therefore two edges are added into the graph, an edge from `s` to `t` and an edge from `t` to `s`, both with the weight `w`. 

The expected suffix for XenGraph files is `.xeng` although it is not enforced.

### DIMACS input format

This input format was used during the 9th DIMACS Implementation Challenge on shortest paths. Example graphs in this format can therefore be downloaded from the website of the challenge: http://users.diag.uniroma1.it/challenge9/download.shtml . The graph file is a plain text file and it looks as follows:

* Everywhere in the file lines beginning with the character `c` can occur. Those lines are comment lines and are skipped during loading.
* First line of the file not starting with the character `c` must be in the format `p sp n e`. Here `p sp` is a fixed string which serves as a magic constant. `n` is a positive integer denoting the number of nodes, `e` is another positive integer denoting the number of edges.
* Then there must be exactly `e` lines of the format `a s t w` each representing one edge. In this case, `a` is a fixed characted constant indicating that the line describes one edge (so it is not a comment line). `s` is the source node of the edge and `t` is the target node of the edge. In this format, nodes are indexed from 1, so both `s` and `t` must be in the range from 1 to `n` (Internally, nodes are indexed from 0, so during loading, each node ID is automatically decreased by one). In this format, each edge is considered to be a one way edge, so if we want a bidirectional edge between `s` to `t`, we must provide two lines, one for an edge from `s` to `t` and one for an edge from `t` to `s`, both with the same weight.

The expected suffix for DIMACS graph files is `.gr` although it is not enforced.

### The query set input format

For easier benchmarking, the user can provide a set of queries which will be used for the benchmark in a very simple plain text format:

* The file begins with a line that contains only a single integer `cnt` denoting the number of queries.
* The first line is followed by exactly `cnt` lines in the format `s g` each denoting one query. `s` and `g` are both positive integers. When benchmarking without mapping, `s` and `g` must be in the range from 0 to `n`-1 where `n` is the number of nodes in the graph. When benchmarking with mapping, `s` and `g` can be arbitrary positive integers as long as they fit in the long long unsigned int datatype (usually 64 bit).

### The mapping file format

If your application internally for some reason uses some node IDs that are not in the range from 0 to `n`-1 (where `n` is the number of nodes in the graph), you can use a mapping file that will allow the application to transform node IDs from your application to IDs in the range from 0 to `n`-1. Using those mapping files, you can let the C++ application do all the mapping work, so will not need to change the IDs in your application. The mapping file is a plain text file in the following format:

* The file begins with a line `XID n` where `XID` is a fixed string which serves as a magic constant and `n` is the amount of nodes in the graph. 
* The first line is followed by exactly `n` lines each only containing one integer `i`. The `j`-th line represents the original ID of the `(j-2)`-th node. So the second line contains the original ID of the node with the ID 0 in our application. The third line contains the original ID of the node with the ID 1, and so on up to the line `n+1` contains the original ID for the node with the ID `n-1` in our application.


Python script for transformation of GeoJSON to input graphs
-----------------------------------------------------------

In our case, we want to preprocess real road networks which are represented in `GeoJSON`. Therefore the `Python` subdirectory contains two simple `Python` scripts that can transform the GeoJSON input files into graph files in either the XenGraph or the DIMACS format that can be further processed by the preprocessor.

We recommend the `transformGeoJSONtoXenGraph.py` which takes a pair of files `edges.geojson` and `nodes.geojson` as input and creates two files `graph.xeng` and `graph.xeni`. The first of the created files `graph.xeng` is the road network represented in the XenGraph format, so this file can be immediately used by the preprocessor to precompute structures for one of the methods. The second file `graph.xeni` contains the mapping from the IDs present in the GeoJSON files to the IDs assigned for our application. You will need this file if you want to answer queries using the original IDs.

If you want to change the names of the input or output files, you can easily change the Python script, just change its `__main__`. Additionally, you can change the precision of the edge weights. Edge weights are computed as travel times, meaning we divide the lenght of the edge by the maximum speed along that edge to obtain the weight of the edge. Since the weights are integers internally, we can increase or decrease the precision by first multiplying or dividing the travel time by some power of ten before transforming it to an integer. You can do this by changing the precision variable. Precision 1000 means the travel times will be in millisecons while precision 1 means the travel times will be in seconds. We recommend using bigger precision for smaller road networks to obtain the best possible results, for large networks smaller precision might be necessary as the longest distance between two nodes should still fit into a 32 bit unsigned integer and this might not be the case for large networks with large precision.


Library
=======

Compiling
---------

Let us now focus on the shared library. The library can be used to load the data structures precomputed by the preprocessor and then answer queries quickly using those structures. In order to use the library, we first need to compile it for our architecture. This can be again achieved easily using `CMake`. First, run `CMake` in the root of this project. This should create a `Makefile` with a target called `shortestPaths` that will build the shared library. 

In Linux, the process can be done as follows:
* `mkdir cmakedata`
* `cd cmakedata`
* `cmake ..`
* `make shortestPaths`

The output of this should be a `.so` file on Linux or a `.dll` file on Windows.

The interface of the library
----------------------------

The library currently provides a query manager for each of the three methods (Contraction Hierarchies, Transit Node Routing and Transit Node Routing with Arc Flags). Those query managers serve as an interface that can be used from other applications. The managers are called `CHDistanceQueryManagerAPI`, `TNRDistanceQueryManagerAPI` and `TNRAFDistanceQueryManagerAPI`.

Each manager provides three functions. The first function is called `initializeCH` (or `initializeTNR` or `initializeTNRAF` respectively). This function can load the data structure prepared by the preprocessor along with the mapping file and initialize the query manager for queries. The second function is `distanceQuery`. This function answers queries. It expects the original IDs of the start and the goal nodes as arguments, then returns the shortest distance from start to goal. The third function is called `clearStructures`. This function deallocates all the memory required for the data structure. This function should be always explicitly called from your application when you will not need to use the query manager anymore. This is necessary because for example when using the library from `Java`, the Java garbage collector is not able to clear the memory used by the library. Therefore you must free the memory explicitly by calling this function.

Javatests example
-----------------
A simple `Java` application that uses the library to answer queries can be found in the `javatests` subdirectory. This application was used to test that the API is functional and everything works as intended. You can however use this application as an example of how to use the library from `Java`. 

For more information about how to integrate this project with other programming languages, please check the `README.md` in the `src/API` subdirectory.

