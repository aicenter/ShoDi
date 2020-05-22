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
  * `cmake --build . --target shortestPathsPreprocessor`
3. Then invoke the `shortestPaths` target of the created `Makefile`:
  * `cmake --build . --target shortestPaths`

After this, you should acquire a runnable application `shortestPathsPreprocessor` and a shared library `libshortestPaths.so` in Linux or `shortestPaths.dll` in Windows. This result can also be achieved by simply loading this project into your favourite IDE and then using the tools provided by it. Note that your library must have the exact same architecture as the JVM you will be using. In some older JDKs, the JVM is 32-bit only. In such cases, it is necessary to compile a 32-bit version of the library (64-bit version will not work with the JVM).

Creating the data structure required for the query algorithm
------------------------------------------------------------

Let us now assume that we want to use Transit Node Routing with Arc Flags in Amodsim, that is the `TNRAFTravelTimeProvider`. If we wanted to use Contraction Hierarchies or Transit Node Routing, the process would be fairly similar.

To precompute the data structure, we first need an input graph. Assume we have our desired road network in the form of two `GeoJSON` files `nodes.geojson` and `edges.geojson`. We can use the provided Python script to transform those two files into an input graph for the `preprocessor`.

1. Go into the Python subdirectory of this project
2. Run the `transformGeoJSONtoXenGraph.py` Python script present in that subdirectory.
   * The script provides a very simple command line interface. It expects 4 arguments: `N E P O`. `N` is the path to the `nodes.geojson` file, `E` is the path to the `edges.geojson` file, `P` is your desired precision and `O` is the desired output path.
   * The precision (the `P` argument) must be a positive number. Precision `1` means that the edge weighs in the obtained graph will correspond to seconds. Precision `1000` means they will correspond to milliseconds. For large graphs you can also use precisions smaller than `1`. For example using `0.1` the weights of the edges of the obtained graph will be in tens of milliseconds.
   * The script will output two files. The first file both stored in a location specified by the `O` argument. The first file will have a `.xeng` suffix and represents the actual graph. The second file will have a `.xeni` suffix and represents the mapping.
   * An example of a correct call of the script: `python transformGeoJSONtoXenGraph.py nodes.geojson edges.geojson 1000 output_graph`. This will create two files `output_graph.xeng` and `output_graph.xeni` in the current working directory.

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

Now we need to make sure that the Java VM will be able to load the library. The JVM loads libraries from the locations contained in the `java.library.path` system property. This property is initialized from `LD_LIBRARY_PATH` in Linux and from `PATH` in Windows. The easiest way to make sure the JVM will be able to load the library is therefore to edit those system variables so that they contain the path to the library.

### In Linux

Assume the library (the `libshortestPaths.so` file) is located in the `/home/user/splib` directory. Then all we have to do is add this directory to the `LD_LIBRARY_PATH` system variable:

- `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/user/splib`

This command will add the path to the library to your existing library path. After this command, only the current session is affected, so we would need to rerun this command for example every time when we reboot the computer. To make the change permanent, we need to add this command into the `.bashrc` file:

- `echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/user/splib' >> ~/.bashrc`

This appends the export command to your `.bashrc` so that it will be executed each time when a new session is initialized. You can also edit your `.bashrc` manually using your text editor of choice (such as Vim or Gedit).

### In Windows

Assume the library (the `shortestPaths.dll` file) is located in the `C:\Users\user\splib` directory. We need to add this directory to the `PATH` environment variable:

1. Right-click on the Start Button.
2. Select `System` from the context menu.
3. Click `Advanced system settings`.
4. Go to the `Advanced` tab and click `Environment Variables...`
5. Select the variable called `Path` and click `Edit...` (There might be two `Path` variables one in the `User variables` section and one in the `System variables` section. We recommend changing the one in the `System variables` section if possible.)
6. In the pop-up window click `New` and add the paste the path to the directory containing the library, in our case `C:\Users\user\splib`.
7. Save everything.

It might also happen that there is no existing variable called `Path` in your list of environment variables. In that case you can create a new variable called `Path` containing only the path to the library.

### Other options

If you do not want to change the system variables, you can supply the path to library to the JVM by using the `-Djava.library.path=/path/to/directory` option. Note that in this case the `java.library.path` from the system variable is replaced by the path in the argument, so you might also need to add paths to other libraries such as `Gurobi`, so you might need to for example use `-Djava.library.path=/path/to/directory;/path/to/Gurobi`. 

Using `-Djava.library.path` proved to be kinda tricky when using `mvn exec` though, since you need to get this `-Djava.library.path` argument to the JVM that will be actually executing the code. For this you might need something like `-Dexec.args=" -Djava.library.path=/path/to/directory"` so we recommend setting the system variables instead if possible, as it looks like the easier solution to us.

### What remains?

After we have ensured that the JVM will be able to find the library by ensuring the `java.library.path` property will contain the path to it (using any of the given options), we are basically ready. Additionally, we should check that the `TravelTimeProvider` is bound to `TNRAFTravelTimeProvider` in the `MainModule`. If it is, we can run the `OnDemandVehiclesSimulation` and it should now be using Transit Node Routing With Arc Flags to compute the travel times.

The simulation can be started by the following comand:

- `mvn exec:exec '-Dexec.executable="java"' '-Dexec.args="-classpath %classpath cz.cvut.fel.aic.amodsim.OnDemandVehiclesSimulation path/to/local/config.cfg"' '-Dfile.encoding=UTF-8'`

For larger graphs or some of the more demanding providers, the default maximum memory allocation pool for the JVM might not be enough. In such cases, you might need to add `-Xmx20g` to your `-Dexec.args` to allow the JVM to use more memory (substitute `20g` by your desired amount).

