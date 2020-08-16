Shortest Distances computation library in C++
=============================================

This project implements multiple methods for shortest distance computation in directed weighted graphs that leverage 
preprocessing of the graph to provide fast shortest distance queries.
The implemented methods are Contraction Hierarchies, Transit Node Routing (based on Contraction Hierarchies),
Transit Node Routing with Arc Flags (extension of Transit Node Routing) and Distance Matrix computation.

The project is split into two major components. 
One component is the **preprocessor** (an executable called `shortestPathsPreprocessor`) which takes an arbitrary 
graph in a supported format (described later) and prepares the graph, creating structures for fast distance computation.
Additionally, the preprocessor allows running a set of queries and benchmark the time required to answer them. 
This way, the user can easily evaluate whether the performance is sufficient for their use case. 

The second component is the **library** (a shared library called `libshortestPaths.so` in Linux 
or `shortestPaths.dll` in Windows), which can load the structures prepared by the preprocessor to answer shortest 
distance queries. 
The library can be used in a `C++` application, but it can also be integrated into a `Java` application. 
A simple example application written in `Java` which uses the library can be found in the `javatests` subdirectory. 
This application also serves as a testing tool to check whether the project is working correctly on a given machine. 

Integration of this library into an application written in languages other than `C++` and `Java` is also possible,
but it will require you to generate new 'glue code' for the desired language using a tool called `SWIG`. 
You will also need to change the `CMakeLists.txt` file in order to compile the library for such usage. 
The steps required for the integration with a different language are briefly described [here](./src/API/README.md).

Table of Contents
=================

<!--ts-->
   * [Shortest Distances computation library in C++  ](#shortest-distances-computation-library-in-c)
   * [Installation](#installation)
      * [Prebuilt Packages](#prebuilt-packages)
      * [From Source](#from-source)
   * [Usage](#usage)
      * [The Preprocessor](#the-preprocessor)
         * [Graph Preprocessing](#graph-preprocessing)
            * [Graph Preprocessing using Contraction Hierarchies](#graph-preprocessing-using-contraction-hierarchies)
               * [Example Usage](#example-usage)
            * [Graph Preprocessing for Transit Node Routing](#graph-preprocessing-for-transit-node-routing)
               * [Preprocessing Mode](#preprocessing-mode)
               * [Example Usage](#example-usage-1)
            * [Graph Preprocessing for Transit Node Routing with Arc Flags](#graph-preprocessing-for-transit-node-routing-with-arc-flags)
               * [Preprocessing Mode](#preprocessing-mode-1)
               * [Example Usage](#example-usage-2)
            * [Generation of Distance Matrix](#generation-of-distance-matrix)
               * [Preprocessing Mode](#preprocessing-mode-2)
               * [Example Usage](#example-usage-3)
         * [Benchmarking](#benchmarking)
            * [Benchmark Without ID Mapping](#benchmark-without-id-mapping)
            * [Benchmark With ID Mapping](#benchmark-with-id-mapping)
            * [Computed Distances Output](#computed-distances-output)
      * [The Library](#the-library)
         * [The Library Interface](#the-library-interface)
         * [Javatests Example and Unit Tests](#javatests-example-and-unit-tests)
      * [Amod-to-agentpolis](#amod-to-agentpolis)
      * [Input/Output File Formats](#inputoutput-file-formats)
         * [XenGraph Input Format](#xengraph-input-format)
         * [DIMACS input format](#dimacs-input-format)
         * [CSV input format](#csv-input-format)
         * [The query set input format](#the-query-set-input-format)
         * [The mapping file format](#the-mapping-file-format)

<!-- Added by: nojmy, at: Pá 31. �?ervence 2020, 11:34:25 CEST -->

<!--te-->

Installation
============

Prebuilt Packages
-----------------

The `./prebuilt` directory contains precompiled `shortestPaths` library and
`shortestPathsPreprocessor` executable for Linux and Windows for the x86-64 CPU
architecture.

From Source
-----------


### Requirements
 - [vcpkg](https://github.com/microsoft/vcpkg) for dependency management.
 - [Java](https://java.com/en/download/): JNI is required for Java bindings.
 - [Doxygen](https://www.doxygen.nl/index.html) for documentation (optional).

### Building the project
1. Install `vcpkg` packages: `vcpkg install boost-config boost-graph p-ranav-csv2`
2. `mkdir build && cd build`
3. `cmake -DCMAKE_TOOLCHAIN_FILE="<vcpkg dir>/scripts/buildsystems/vcpkg.cmake" -DCMAKE_BUILD_TYPE=Release ..`
4. `cmake --build . --target shortestPathsPreprocessor --config Release`

Usage
=====

The Preprocessor
----------------

The preprocessor has a simple **command line interface** that should make the process of preprocessing graphs quite 
simple.

Let us now assume that you have a directed weighted graph in one of the supported formats 
(see [Input/Output File Formats](#inputoutput-file-formats)).
In addition, a `Python` script is available for conversion of `GeoJSON` data to one of the supported formats.

The command line interface requires you to use specific arguments in the correct order. 
The first argument determines whether you want to preprocess a graph file or benchmark a preprocessed graph using 
some method.
The arguments following the first one are specific to each of the usages.

### Graph Preprocessing

General usage:

```
create <method name> <method specific arg 1>  <method specific arg 2>...
```

#### Graph Preprocessing using Contraction Hierarchies

To preprocess a graph using Contraction Hierarchies, call the preprocessor with the following arguments:

```
create ch <input_format> <input_file> <output_file>
```

where:

 * `input_format` is one of `xengraph`, `dimacs`, `csv`
 * `input_file` is path to the input file (including file extension)
 * `output_file` is path to the output file (*excluding* file extension - the `.ch` extension will be added automatically)


##### Example Usage

```
create ch xengraph my_graph.xeng my_graph
```

#### Graph Preprocessing for Transit Node Routing

To preprocess a graph for Transit Node Routing, call the preprocessor with the following arguments:

```
create tnr <input_format> <preprocessing_mode> <tnodes_cnt> <input_file> <output_file>
```

where:

 * `input_format` is one of `xengraph`, `dimacs`, `csv`
 * `preprocessing_mode` is one of `fast`, `slow`, `dm`
 * `tnodes_cnt` is a positive integer that determines the size of the transit nodes (less than or equal to the number of nodes in the graph)
 * `input_file` is path to the input file (including file extension)
 * `output_file` is path to the output file (*excluding* file extension - the `.tnrg` extension will be added automatically)

##### Preprocessing Mode

The `preprocessing_mode` argument determines which of the three available preprocessing modes will be used. 
The `fast` mode will precompute the structures in the smallest time out of the three, but the performance of the 
resulting data structure will be significantly lower. 
The modes `slow` and `dm` will produce exactly the same result, but the `dm` mode uses distance matrix to
speed up the precomputation and therefore, it is faster than the `slow` mode, but it requires a lot of memory. 

##### Transite node count
The fifth argument determines the size of the transit nodes set. 
It must be an integer between 1 and the number of nodes in the graph. 
Less transit nodes usually mean lower memory requirements, but also worse query times. 
By choosing the appropriate size of the transit node set, you can find a great balance between memory
requirements and performance. 

##### Example Usage

```
create tnr xengraph dm 1000 my_graph.xeng my_graph
```

#### Graph Preprocessing for Transit Node Routing with Arc Flags

To preprocess a graph for Transit Node Routing, call the preprocessor with the following arguments:

```
create tnraf <input_format> <preprocessing_mode> <tnodes_cnt> <input_file> <output_file>
```

where:

 * `input_format` is one of `xengraph`, `dimacs`, `csv`
 * `preprocessing_mode` is one of `slow`, `dm` (for more info, see [Preprocessing Mode](#preprocessing-mode))
 * `tnodes_cnt` is a positive integer that determines the size of the transit nodes (less than or equal to the numbr of nodes in the graph)
 * `input_file` is path to the input file (including file extension)
 * `output_file` is path to the output file (*excluding* file extension - the `.tgaf` extension will be added automatically)


##### Example Usage

```
create tnraf xengraph dm 1000 my_graph.xeng my_graph
```

#### Generation of Distance Matrix

To generate the distance matrix of a graph, call the preprocessor with the following arguments:

```
create dm <input_format> <output_format> <preprocessing_mode> <input_file> <output_file>
```

where:

 * `input_format` is one of `xengraph`, `dimacs`, `csv`
 * `output_format` is one of `xdm`, `csv`
 * `preprocessing_mode` is one of `slow`, `fast`
 * `input_file` is path to the input file (including file extension)
 * `output_file` is path to the output file (*excluding* file extension - the appropriate extension based on `output_format` will be added automatically)

##### Preprocessing Mode

The `fast` mode provides a significant computational speed advantage over the `slow` mode, at an expense of much 
larger memory usage.

##### Example Usage

```
create dm csv csv fast my_graph.csv my_graph
```


### Benchmarking
For benchmarking, you will need a set of queries which can either use IDs in the range from 0 to n-1 (where n is 
the number of the nodes in the graph) or you can use arbitrary integer node IDs. 
In the second case, you also need to provide a mapping file (see next section). 
The formats of the query set file and the mapping file are described in the 
[Input/Output File Formats](#inputoutput-file-formats) section below.

During benchmarking, all of your queries are answered using a chosen method.
Afterwards, the total time need to answer all the queries in seconds is printed alongside the average time needed 
to answer one query in milliseconds.
Additionally, you can specify an output file, where the computed distances will be stored. 
Those distances can then be used for verification of the correctness of the more complex methods.

#### Benchmark Without ID Mapping

To benchmark without mapping, call the preprocessor with the following arguments:

```
benchmark <method> nomapping <input_data_structure> <query_set> [output_file]
```

where:

* `method` is one of `dijkstra`, `ch`, `tnr`, `tnraf` - the method being benchmarked
* `input_data_structure` is path to the data structure preprocessed using the preprocessor for the selected `method` (generated using the preprocessor as described in the previous section)
* `query_set` is path to the query set (file format described in the [File Formats](#inputoutput-file-formats) section below)
* `output_file` (optional) is path to the output file for the computed distances

#### Benchmark With ID Mapping

To benchmark with mapping, call the preprocessor with the following arguments:

```bash
./shortestPathsPreprocessor benchmark [method] mapping [input_data_structure] [query_set] [mapping_file] [output_file?]
```

where:

* `method` is one of `dijkstra`, `ch`, `tnr`, `tnraf` - the method being benchmarked
* `input_data_structure` is path to the data structure preprocessed using the preprocessor *for the selected* 
`method`. For dijkstra, use a XenGraph file.
`input_data_structure` argument. 
* `query_set` is path to the query set (file format described in the File Formats section below)
* `mapping_file` is path to the mapping file (file format described in the File Formats section below), which will be used to transform node IDs from the query set to the corresponding node IDs used by the query algorithms
* `output_file` (optional) is path to the output file for the computed distances

#### Computed Distances

If you provide the `output_file` argument, the application will output a plain text file that will contain the 
name of the query set file used for the benchmark on the first line, and then on each following line, the result of one
queries. 
You can use these files to validate that various methods return the same results.


The Library
-----------

The library can be used to load the data structures precomputed by the preprocessor and then answer queries quickly
using those structures. 

### Compilation
Compile the library by:

```
cmake --build . --target shortestPaths --config Release
```

### The Library Interface

The library provides a query manager for each of the three methods (Contraction Hierarchies, Transit Node Routing 
and Transit Node Routing with Arc Flags), these query managers serve as an interface that can be used from other 
applications.. 
There is no query manager for distance matrix as there is no computation involved, the distance matrix can be used as 
it is.
The query managers are called `CHDistanceQueryManagerAPI`, `TNRDistanceQueryManagerAPI` 
and `TNRAFDistanceQueryManagerAPI`.

Each manager provides three functions. 
The first function is called `initializeCH` (or `initializeTNR` or `initializeTNRAF` respectively). 
This function can load the data structure prepared by the preprocessor along with the mapping file and initialize 
the query manager for queries. 
The second function is `distanceQuery` and it's purpose is to answer queries. 
It expects the original IDs of the start and the goal nodes as arguments, then returns the
shortest distance from start to goal. 
The third function is called `clearStructures`. 
It function deallocates all the memory required for the data structure. 
This function should be always explicitly called from your application when you will not need to use the query 
manager anymore. 


### Javatests Example and Unit Tests

A simple `Java` application that uses the library to answer queries can be found in the `javatests` subdirectory. 
This application was used to test that the API is functional and everything works as intended. 
You can however use this application as an example of how to use the library from `Java`. 
You can consult the readme for this simple application [here](./javatests/README.md).

For more information about how to integrate this project with other programming languages, please consult 
the [readme](./src/API/README.md) in the `src/API` subdirectory.

Amod-to-agentpolis
------------------

If you want to use the library with Amod-to-agentpolis, you can consult the [Amod readme](./AMOD_README.md) 
for step-by-step instructions on how to accomplish this.

Input/Output File Formats
-------------------------

In this part, we will describe all the input formats that can be used with the preprocessor application. 
The first three formats are input formats for directed weighted graphs that can be used as input for the creation of
data structures for one of the methods or for the benchmarking of the dijkstra algorithm. 
The other two formats are related only to benchmarking.

If you need to get information about output file formats, please look [here](./FORMATS.md) for a description of all 
file formats used by the library.

### XenGraph Input Format

Probably the simplier format for directed weighted graphs. Graphs are represented as plain text files in this format. 
The graph file looks as follows:

* It begins with a line `XGI n e` where `XGI` is a fixed string which serves as a magic constant. `n` and `e` 
are positive integers denoting the number of nodes and edges, respectively.
* After that, exactly `e` lines follow in the format `s t w f`, where each line represents one edge. Here, `s` 
is the source node of the edge and `t` is the target node of the edge. Both these values must be in the range from 
0 to `n`-1 (nodes are indexed from 0). `w` is the weight of the edge, in our case a positive integer. 
`f` is a flag denoting whether the edge is a one way edge. It must be either 1 or 0. If `f` is equal to 1, 
the edge is a one way edge and only an edge from `s` to `t` is added to the graph. If `f` is 0, we treat the edge as 
a bidirectional edge and therefore two edges are added into the graph, an edge from `s` to `t` and an edge from `t` 
to `s`, both with the weight `w`. 

The expected suffix for XenGraph files is `.xeng` although it is not enforced.

### DIMACS input format

This input format was used during the 9th DIMACS Implementation Challenge on shortest paths. 
Example graphs in this format can therefore be downloaded from the website of the challenge:
http://users.diag.uniroma1.it/challenge9/download.shtml . 
The graph file is a plain text file and it looks as follows:

* Lines beginning with the character `c` can occur anywhere in the file. 
Those lines are comment lines and are skipped during loading.
* First line of the file not starting with the character `c` must be in the format `p sp n e`, where `p sp` is a 
fixed string which serves as a magic constant, `n` is a positive integer denoting the number of nodes and `e` is 
another positive integer denoting the number of edges.
* After that, there must be exactly `e` lines of the format `a s t w`, where each line represents one edge. 
Here, `a` is a fixed character constant indicating that the line describes one edge (as opposed to `c` representing
 a comment line), `s` is the source node of the edge and `t` is the target node of the edge. 
In this format, nodes are indexed from 1, so both `s` and `t` must be in the range from 1 to `n` (Internally, 
nodes are indexed from 0, so during loading, each node ID is automatically decremented). 
In this format, each edge is considered to be a *directional* edge, so if we want a bidirectional edge 
between `s` and `t`, we must provide two lines, one for an edge from `s` to `t` and one for an edge 
from `t` to `s`, both with the same weight.

The expected suffix for DIMACS graph files is `.gr` although it is not enforced.

### CSV input format

One of the three input formats that can be used to describe the input graph for the preprocessing.

A CSV file represents an [adjacency matrix](https://en.wikipedia.org/wiki/Adjacency_matrix) of a graph.

* The file is expected to not have a header.
* The file must have its amount of rows equal to its amount of columns.
* Each value on row `i` and column `j` represents the weight of a *directed* edge from node `i` to node `j`.
* Weights are expected to be valid *positive* integers, or `nan` for where there is no edge joining the nodes.


### The Query Set Input Format

For easier benchmarking, the user can provide a set of queries which will be used for the benchmark in a very simple 
plain text format:

* The file begins with a line that contains only a single integer `cnt` denoting the number of queries.
* The first line is followed by exactly `cnt` lines in the format `s g` each denoting one query. 
`s` and `g` are both positive integers. 
When benchmarking without mapping, `s` and `g` must be in the range from 0 to `n`-1 where `n` is the number of 
nodes in the graph. 
When benchmarking with mapping, `s` and `g` can be arbitrary positive integers as long as they fit in the long long 
unsigned int datatype (usually 64 bit).

### The mapping file format

If your application internally for some reason uses some node IDs that are not in the range from 0 to `n`-1 
(where `n` is the number of nodes in the graph), you can use a mapping file that will allow the application to 
transform node IDs from your application to IDs in the range from 0 to `n`-1. 
Using those mapping files, you can let the C++ application do all the mapping work, so will not need to change the 
IDs in your application. 
The mapping file is a plain text file in the following format:

* The file begins with a line `XID n` where `XID` is a fixed string which serves as a magic constant and `n` is the 
amount of nodes in the graph. 
* The first line is followed by exactly `n` lines each only containing one integer `i`. The `j`-th line represents 
the original ID of the `(j-2)`-th node. So the second line contains the original ID of the node with the ID 0 in our 
application. 
The third line contains the original ID of the node with the ID 1, and so on up to the line `n+1` contains the 
original ID for the node with the ID `n-1` in our application.


