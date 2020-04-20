Step-by-step tutorial for getting this library running with Amod-to-agentpolis
==============================================================================

This document describes the exact steps that need to be performed in order to make this library running in Amod-to-agentpolis (which would allow the usage of the travel time providers based on this library).

Compiling the preprocessor and the library
------------------------------------------

First of all, we will need to compile the `preprocessor` that will be used to compute the structures needed and we will also need to compile the `library` that will be loaded from Java.

Both of those tasks should be fairly easy thanks to the provided `CMakeLists.txt`.

1. Create a `Makefile` by running `CMake` in the root of this project. In Linux, this can be done as follows:
  * `mkdir cmakedata`
  * `cd cmakedata`
  * `cmake ..`
2. Now invoke the `shortestPathsPreprocessor` target of the created `Makefile`:
  * `make shortestPathsPreprocessor`
3. Then invoke the `shortestPaths` target of the created `Makefile`:
  * `make shortestPaths`

After this, you should acquire a runnable application `shortestPathsPreprocessor` and a shared library `libshortestPaths.so` in Linux or their equivalents in other operating systems. This result can also be achieved by simply loading this project into your favourite IDE and then using the tools provided by it.

Creating the data structure required for the query algorithm
------------------------------------------------------------

Let us now assume that we want to use Transit Node Routing with Arc Flags in Amod, that is the `TNRAFTravelTimeProvider`. If we wanted to use Contraction Hierarchies or Transit Node Routing, the process would be fairly similar.

To precompute the data structure, we first need an input graph. Assume we have our desired road network in the form of two `GeoJSON` files `nodes.geojson` and `edges.geojson`. We can use the provided Python script to transform those two files into an input graph for the `preprocessor`.

1. Go into the Python subdirectory of this project
2. Copy your `nodes.geojson` and `edges.geojson` into that subdirectory.
3. Run the `transformGeoJSONtoXenGraph.py` Python script present in that subdirectory.

Alternatively if you do not want to copy `nodes.geojson` and `edges.geojson`, you can edit the script and insert the desired paths. After running the script, you should be left with two files `graph.xeng` and `graph.xeni` in the Python subdirectory.

We will now continue with the file `graph.xeng` but save the other one, we will need it later. Now we can use the `preprocessor` with the `graph.xeng` file to precompute the data structures needed for the query algorithm.

1. Run the `preprocessor` as one of the two: 
  1. `./shortestPathsPreprocessor create tnraf xengraph dm 1000 graph.xeng my_graph`
  2. `./shortestPathsPreprocessor create tnraf xengraph slow 1000 graph.xeng my_graph`

The fourth argument (either `dm` or `slow`) switches between two preprocessing modes. The `dm` mode is faster but needs significantly more memory. The `slow` mode is slower but should work even on machines with less memory or for large graphs. For normal sized graphs we suggest using the `dm` mode if you have at least 16 GB of memory and the `slow` mode otherwise. The fifth argument determines the size of the transit node set. Usually more transit nodes mean better performance but higher memory requirements. For the graph of Prague, 2000 transit nodes are suggested. The sixth argument is the path to your `graph.xeng` from the previous step, so you can exchange it for the path on your machine and the last argument is the output path where the Transit Node Routing with Arc Flags data structure will be output, so you can choose it yourself. A `.tgaf` suffix will be automatically appended to your chosen path. 

Please note that the preprocessing can take from minutes to hours based on the size of your graph, the preprocessing mode, the chosen amount of transit nodes and your machine.

The result of the preprocessing will be a file `my_graph.tgaf` that contains all the data neede for the Transit Node Routing with Arc Flags query algorithm.

Configuration of Amod-to-agentpolis
-----------------------------------

Now we have basically everything ready and we can proceed to what needs to be configured on the Amod-to-agentpolis side.

First we will need to set the correct paths in the config. Amod contains a `config.cfg` file in the resources. Find the `shortestpaths` section of that file and then set those paths:
1. Set the `shortestpaths_data_dir` variable to the directory where you will have your `my_graph.tgaf` and your `graph.xeni` files stored.
2. Set the `tnraf_file_name` variable to the name of file acquired by the `preprocessor`, in our case it should be `my_graph.tgaf`.
3. Set the `mapping_file_name` variable to the name of the second file acquired by running the Python script, in our case it should be `graph.xeni`.

You do not have to set the other variables if you only want to use the Transit Node Routing with Arc Flags method.

Now we need to make sure that the Java VM will be able to load the library. The easiest way to accomplish this is to include the path to the `libshortestPaths.so` shared library acquired at the beginning of this tutorial to the `java.library.path`. This can be done by runnning the Java VM with the option `-Djava.library.path=/something/` where `something` is the directory containing `libshortestPaths.so`.

After this is done, we are basically ready. Additionally, check that the `TravelTimeProvider` is bound to `TNRAFTravelTimeProvider` in the `MainModule`. If it is, run the `OnDemandVehiclesSimulation` and it should now be using Transit Node Routing With Arc Flags to compute the travel times.


