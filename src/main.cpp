//
// Author: Xenty (Michal Cvach)
// Created on: 01.08.19
//

/*
 * This simple main provides a way to create Contraction Hierarchies for given input files (graphs).
 */


#include <fstream>
#include <iomanip>
#include "GraphBuilding/Loaders/DIMACSLoader.h"
#include "GraphBuilding/Loaders/TNRGLoader.h"
#include "GraphBuilding/Loaders/FloatingPointXenGraphLoader.h"
#include "GraphBuilding/Loaders/IntegerXenGraphLoader.h"
#include "Benchmarking/FloatingPoint/FPointCorrectnessValidator.h"
#include "Timer/Timer.h"
#include "CH/Integer/IntegerCHPreprocessor.h"
#include "CH/FloatingPoint/FPointCHPreprocessor.h"
#include "TNR/TNRPreprocessor.h"
#include "TNRAF/TNRAFPreprocessor.h"
#include "GraphBuilding/Loaders/TripsLoader.h"
#include "GraphBuilding/Loaders/DDSGLoader.h"
#include "Benchmarking/Integer/IntegerCHBenchmark.h"
#include "Benchmarking/Integer/IntegerDijkstraBenchmark.h"
#include "Benchmarking/Integer/IntegerCorrectnessValidator.h"
#include "Benchmarking/Integer/TNRBenchmark.h"
#include "Benchmarking/Integer/DistanceCorrectnessValidator.h"
#include "Benchmarking/Integer/PathCorrectnessValidator.h"
#include "CH/Integer/IntegerCHPathQueryManager.h"
#include "Dijkstra/IntegerDijkstra/BasicIntegerDijkstra.h"
#include "TNR/TNRPathQueryManager.h"
#include "DistanceMatrix/IntegerDistanceMatrixComputor.h"
#include "GraphBuilding/Loaders/IntegerDistanceMatrixLoader.h"
#include "Benchmarking/Integer/DistanceMatrixBenchmark.h"


using namespace std;

// Prints info about how to use the application to the user.
//______________________________________________________________________________________________________________________
void printUsageInfo(char * appName) {
    printf("This application allows the user to create a Contraction Hierarchy for a given map file.\n"
           "The correct usage is as follows:\n"
           "%s [command] [precision] [input format] [input file path] [output file path]\n"
           "  - Command: currently only 'c' (create) is a valid command.\n"
           "             This means that you want to create a new Contraction Hierarchy.\n"
           "             Other commands might be added in the future.\n"
           "  - Precision: allows you to set whether the hierarchy should work with integers or doubles\n"
           "               internally. 'f' for doubles (floating point), 'i' for integers.\n"
           "  - Input format: currently the application allows two input formats. DIMACS, which is a format\n"
           "                  introduced in the 9th DIMACS Implementation Challenge, and also XenGraph,\n"
           "                  which is a simple text format for graphs. 'D' for DIMACS, 'X' for XenGraph.\n"
           "                  Note that the DIMACS format can only be used with integer precision.\n"
           "  - Input file path: file path to the graph file (in the chosen format) for which you want\n"
           "                     to generate a hierarchy. The path can be in a relative form from the\n"
           "                     current working directory or an absolute path.\n"
           "  - Output file path: determines where the .ch or .chf file containing the Contraction Hierarchy\n"
           "                      will be saved. The .ch or .chf suffix is added automatically so you do not\n"
           "                      to include it in the path. The .ch suffix is for integer hierarchies while\n"
           "                      the .chf suffix is for the floating point hierarchies.\n"
           "\n"
           "A simple example: let's say I have a file 'Prague_map.xeng' which is in a XenGraph format\n"
           "and want to create a integer Contraction Hierarchy for this graph and save it into\n"
           "the current directory with the name 'Prague_map.ch'. I can do this with the following command:\n"
           " '%s c i X Prague_map.xeng Prague_map'\n", appName, appName);
}

// Creates an integer Contraction Hierarchy (.ch) for an input file in a XenGraph format.
//______________________________________________________________________________________________________________________
void createIntegerXenGraphHierarchy(char * inputFilePath, char * outputFilePath) {
    Timer timer("Whole CH construction timer");
    timer.begin();

    IntegerXenGraphLoader graphLoader = IntegerXenGraphLoader(inputFilePath);
    //DIMACSLoader graphLoader = DIMACSLoader("../input/graph.gr");
    IntegerUpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    IntegerCHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);
    graph->flushInDdsgFormat(outputFilePath);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Creates an integer Contraction Hierarchy (.ch) for an input file in a DIMACS format.
//______________________________________________________________________________________________________________________
void createIntegerDIMACSHierarchy(char * inputFilePath, char * outputFilePath) {
    Timer timer("Whole CH construction timer");
    timer.begin();

    DIMACSLoader graphLoader = DIMACSLoader(inputFilePath);
    IntegerUpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    IntegerCHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);
    graph->flushInDdsgFormat(outputFilePath);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Creates a floating point Contraction Hierarchy (.chf) for an input file in a XenGraph format.
//______________________________________________________________________________________________________________________
void createFloatingPointXenGraphHierarchy(char * inputFilePath, char * outputFilePath) {
    Timer timer("Whole CH construction timer");
    timer.begin();

    FloatingPointXenGraphLoader graphLoader = FloatingPointXenGraphLoader(inputFilePath);
    FPointUpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    FPointCHPreprocessor::preprocessForDDSGF(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);
    graph->flushInDdsgfFormat(outputFilePath);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Creates a TNR structure based on the given XenGraph file.
//______________________________________________________________________________________________________________________
void createTNR() {
    Timer timer("Whole TNR construction timer");
    timer.begin();

    IntegerXenGraphLoader graphLoader = IntegerXenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    IntegerUpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    IntegerCHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessUsingCH(*graph, "../input/Prague_n500_unpack", 500);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Creates a TNR structure based on the given XenGraph file.
//______________________________________________________________________________________________________________________
void createTNRAF() {
    Timer timer("Whole TNR AF construction timer");
    timer.begin();

    IntegerXenGraphLoader graphLoader = IntegerXenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    IntegerUpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    IntegerGraph * originalGraph = graph->createCopy();
    IntegerCHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRAFPreprocessor::preprocessUsingCH(*graph, *originalGraph, "../input/Prague_n500_unpack", 500);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Creates a Contraction Hierarchy based on the given XenGraph file.
//______________________________________________________________________________________________________________________
void createCH() {
    Timer timer("Whole CH construction timer");
    timer.begin();

    IntegerXenGraphLoader graphLoader = IntegerXenGraphLoader("../input/Debug_graph.xeng");
    IntegerUpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    IntegerCHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);
    graph->flushInDdsgFormat("../input/Debug_graph");

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Creates a Distance Matrix based on the given file containing a graph.
//______________________________________________________________________________________________________________________
void createDM() {
    Timer timer("Whole Distance Matrix computation timer");
    timer.begin();

    IntegerXenGraphLoader dijkstraGraphLoader = IntegerXenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    IntegerGraph * graph = dijkstraGraphLoader.loadGraph();

    IntegerDistanceMatrixComputor dmComputor;
    dmComputor.computeDistanceMatrix(*graph);
    dmComputor.outputDistanceMatrixToFile("../input/Prague_int_1000prec");

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Compares the running times of Dijkstra, Contraction Hierarchies and Transit Node Routing on a given set of trips
// (queries). Speed-up of CH in comparison with Dijkstra and of TNR in comparison with Dijkstra and CH is calculated
// and the results of CH and TNR are compared with the results of Dijkstra to validate that those method give correct
// results.
//______________________________________________________________________________________________________________________
void compareMethods() {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    IntegerFlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<long long unsigned int> chDistances(trips.size());
    double chTime = IntegerCHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, chDistances);

    delete ch;

    //DIMACSLoader dijkstraLoader = DIMACSLoader("../input/graph.gr");
    IntegerXenGraphLoader dijkstraLoader = IntegerXenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    IntegerGraph * dijkstraGraph = dijkstraLoader.loadGraph();

    vector<long long unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = IntegerDijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistances);

    IntegerCorrectnessValidator::validateVerbose(chDistances, dijkstraDistances);
    printf("Contraction Hierarchies were %lf times faster than Dijkstra!\n", dijkstraTime/chTime);

    delete dijkstraGraph;

    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_map_1000_sept.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<long long unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnrGraph, tnrDistances);

    IntegerCorrectnessValidator::validateVerbose(tnrDistances, dijkstraDistances);
    printf("TNR was %lf times faster than Dijkstra!\n", dijkstraTime/tnrTime);
    printf("TNR was %lf times faster than CH!\n", chTime/tnrTime);

    delete tnrGraph;

}

// Compares the running times of Dijkstra, Contraction Hierarchies, Transit Node Routing and Distance Matrix on a given
// set of trips (queries).
//______________________________________________________________________________________________________________________
void compareFourMethods() {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_50000_randomTrips.txt");
    //TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    IntegerFlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<long long unsigned int> chDistances(trips.size());
    double chTime = IntegerCHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, chDistances);

    delete ch;

    //DIMACSLoader dijkstraLoader = DIMACSLoader("../input/graph.gr");
    IntegerXenGraphLoader dijkstraLoader = IntegerXenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    IntegerGraph * dijkstraGraph = dijkstraLoader.loadGraph();

    vector<long long unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = IntegerDijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistances);

    IntegerCorrectnessValidator::validateVerbose(chDistances, dijkstraDistances);
    printf("Contraction Hierarchies were %lf times faster than Dijkstra!\n", dijkstraTime/chTime);

    delete dijkstraGraph;

    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_map_1000_newFilter.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<long long unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnrGraph, tnrDistances);

    IntegerCorrectnessValidator::validateVerbose(tnrDistances, dijkstraDistances);
    printf("TNR was %lf times faster than Dijkstra!\n", dijkstraTime/tnrTime);
    printf("TNR was %lf times faster than CH!\n", chTime/tnrTime);

    delete tnrGraph;

    IntegerDistanceMatrixLoader distanceMatrixLoader = IntegerDistanceMatrixLoader("../input/Prague_int_1000prec.xdm");
    IntegerDistanceMatrix * distanceMatrix = distanceMatrixLoader.loadDistanceMatrix();

    vector<long long unsigned int> dmDistances(trips.size());
    double dmTime = DistanceMatrixBenchmark::runAndMeasureOutputAndRetval(trips, *distanceMatrix, dmDistances);

    IntegerCorrectnessValidator::validateVerbose(dmDistances, dijkstraDistances);
    printf("Distance Matrix was %lf times faster than Dijkstra!\n", dijkstraTime/dmTime);
    printf("Distance Matrix was %lf times faster than CH!\n", chTime/dmTime);
    printf("Distance Matrix was %lf times faster than TNR!\n", tnrTime/dmTime);

}

// Compares the running times of Contraction Hierarchies and Transit Node Routing on a given set of trips (queries).
//______________________________________________________________________________________________________________________
void compareCHandTNR() {
    //TripsLoader tripsLoader = TripsLoader("../input/Prague_50000_randomTrips.txt");
    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    IntegerFlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<long long unsigned int> chDistances(trips.size());
    double chTime = IntegerCHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, chDistances);

    delete ch;

    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_map_n2000_debug.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<long long unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnrGraph, tnrDistances);

    IntegerCorrectnessValidator::validateVerbose(tnrDistances, chDistances);
    printf("TNR was %lf times faster than CH!\n", chTime/tnrTime);

    delete tnrGraph;

}

// Compares the running times of Transit Node Routing with different various transit node set sizes on a given set of
// trips (queries).
//______________________________________________________________________________________________________________________
void compareVariousTransitSetSizes() {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_50000_randomTrips.txt");
    //TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    TNRGLoader tnr500Loader = TNRGLoader("../input/Prague_map_1000_newFilter_500nodes.tnrg");
    TransitNodeRoutingGraph * tnr500Graph = tnr500Loader.loadTNRforDistanceQueries();

    vector<long long unsigned int> tnr500Distances(trips.size());
    double tnr500Time = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnr500Graph, tnr500Distances);

    delete tnr500Graph;

    TNRGLoader tnr1000Loader = TNRGLoader("../input/Prague_map_1000_newFilter.tnrg");
    TransitNodeRoutingGraph * tnr1000Graph = tnr1000Loader.loadTNRforDistanceQueries();

    vector<long long unsigned int> tnr1000Distances(trips.size());
    double tnr1000Time = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnr1000Graph, tnr1000Distances);

    delete tnr1000Graph;

    TNRGLoader tnr2000Loader = TNRGLoader("../input/Prague_map_1000_newFilter_2000nodes.tnrg");
    TransitNodeRoutingGraph * tnr2000Graph = tnr2000Loader.loadTNRforDistanceQueries();

    vector<long long unsigned int> tnr2000Distances(trips.size());
    double tnr2000Time = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnr2000Graph, tnr2000Distances);

    delete tnr2000Graph;

    IntegerCorrectnessValidator::validateVerbose(tnr500Distances, tnr1000Distances);
    IntegerCorrectnessValidator::validateVerbose(tnr500Distances, tnr2000Distances);

    printf("TNR with  500 transit nodes time: %lf (%lf for one query)\n", tnr500Time, tnr500Time/trips.size());
    printf("TNR with 1000 transit nodes time: %lf (%lf for one query)\n", tnr1000Time, tnr1000Time/trips.size());
    printf("TNR with 2000 transit nodes time: %lf (%lf for one query)\n", tnr2000Time, tnr2000Time/trips.size());



}

//______________________________________________________________________________________________________________________
void validateCHPathCorectness() {
    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    IntegerFlagsGraphWithUnpackingData * chGraph = chLoader.loadFlagsGraphWithUnpackingData();
    IntegerCHPathQueryManager queryManager = IntegerCHPathQueryManager(*chGraph);

    IntegerXenGraphLoader dijkstraLoader = IntegerXenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    IntegerGraph * originalGraph = dijkstraLoader.loadGraph();

    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    PathCorrectnessValidator::validateCHPaths(originalGraph, queryManager, trips);

    delete originalGraph;
    delete chGraph;

}

//______________________________________________________________________________________________________________________
void validateTNRPathCorectness() {
    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_n500_unpack.tnrg");
    TransitNodeRoutingGraphForPathQueries * tnrGraph = tnrLoader.loadTNRforPathQueries();
    TNRPathQueryManager queryManager(*tnrGraph);

    IntegerXenGraphLoader dijkstraLoader = IntegerXenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    IntegerGraph * originalGraph = dijkstraLoader.loadGraph();

    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    PathCorrectnessValidator::validateTNRPaths(originalGraph, queryManager, trips);

    delete originalGraph;
    delete tnrGraph;

}

//______________________________________________________________________________________________________________________
void memoryUsageOfDijkstra() {
    IntegerXenGraphLoader dijkstraGraphLoader = IntegerXenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    IntegerGraph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    delete dijkstraGraph;
}

//______________________________________________________________________________________________________________________
void memoryUsageOfCH() {
    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    IntegerFlagsGraph * ch = chLoader.loadFlagsGraph();

    delete ch;
}

//______________________________________________________________________________________________________________________
void memoryUsageOfTNR() {
    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_map_1000_newFilter.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    delete tnrGraph;
}

//______________________________________________________________________________________________________________________
void memoryUsageOfDM() {
    IntegerDistanceMatrixLoader distanceMatrixLoader = IntegerDistanceMatrixLoader("../input/Prague_int_1000prec.xdm");
    IntegerDistanceMatrix * distanceMatrix = distanceMatrixLoader.loadDistanceMatrix();

    delete distanceMatrix;
}

// Auxiliary function currently used for debug purposes. Prints the real distance from source to access node, between
// twe two access nodes and from access node to target. Might help determine which of these parts is being computed
// incorrectly in TNR.
//______________________________________________________________________________________________________________________
void checkDijkstraDistances(unsigned int sourceNode, unsigned int sourceAccess, unsigned int targetAccess, unsigned int targetNode) {
    IntegerXenGraphLoader dijkstraGraphLoader = IntegerXenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    IntegerGraph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    printf("~~~ Computing distances using Dijkstra ~~~\n");
    printf("%u -> %u: %llu\n", sourceNode, sourceAccess, BasicIntegerDijkstra::run(sourceNode, sourceAccess, *dijkstraGraph));
    printf("%u -> %u: %llu\n", sourceAccess, targetAccess, BasicIntegerDijkstra::run(sourceAccess, targetAccess, *dijkstraGraph));
    printf("%u -> %u: %llu\n", targetAccess, targetNode, BasicIntegerDijkstra::run(targetAccess, targetNode, *dijkstraGraph));
    printf("~~~ End of Dijkstra computation ~~~\n");
    delete dijkstraGraph;

}

//______________________________________________________________________________________________________________________
void getDijkstraPathForTrip(unsigned int tripNum) {
    IntegerXenGraphLoader dijkstraGraphLoader = IntegerXenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    IntegerGraph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    //BasicIntegerDijkstra::runWithPathOutput(trips[tripNum].first, trips[tripNum].second, *dijkstraGraph);
    BasicIntegerDijkstra::runWithPathOutput(trips[tripNum].first /*20652*/, trips[tripNum].second, *dijkstraGraph);

    delete dijkstraGraph;

}

// Will print the node sequence on a certain path computed by Contraction Hierarchies - this means that the paths will
// be unpacked from the shortcuts. Can be used for debug, especially if CH returns different paths than Dijkstra.
//______________________________________________________________________________________________________________________
void getCHPathForTrip(unsigned int tripNum) {
    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    IntegerFlagsGraphWithUnpackingData * chGraph = chLoader.loadFlagsGraphWithUnpackingData();

    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    IntegerCHPathQueryManager queryManager(*chGraph);
    //chGraph->debugPrint();
    long long unsigned int distance = queryManager.findDistanceOutputPath(trips[tripNum].first,
                                                                          trips[tripNum].second);
    printf("Returned distance: %llu\n", distance);

    printf("Rank of node %u: %u\n", trips[tripNum].first, chGraph->data(trips[tripNum].first).rank);
    printf("Rank of node %u: %u\n", trips[tripNum].second, chGraph->data(trips[tripNum].second).rank);

    delete chGraph;
}

// Will print the whole path computed by Transit Node Routing (all edges on the path). Can be used for debug if TNR
// returns different distances than for example Dijkstra.
//______________________________________________________________________________________________________________________
void getTNRPathForTrip(unsigned int tripNum) {
    //TNRGLoader tnrLoader = TNRGLoader("/home/xenty/sum/2019/ContractionHierarchies/contraction-hierarchies/input/Prague_map_1000_newFilter_500nodes.tnrg");
    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_n500_unpack.tnrg");
    TransitNodeRoutingGraphForPathQueries * tnrGraph = tnrLoader.loadTNRforPathQueries();

    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    TNRPathQueryManager queryManager(*tnrGraph);

    unsigned int distance = queryManager.findDistance(trips[tripNum].first, trips[tripNum].second);
    printf("Returned distance: %u\n", distance);

    delete tnrGraph;

}

// This function runs Contraction Hierarchies on a given set of path queries. Each returned path is validated in order
// to check if the paths actually exists in the original graph. This ensures that the unpacking routine for the path
// queries in Contraction Hierarchies works correctly and only valid paths are returned.
//______________________________________________________________________________________________________________________
void validateCHPaths() {
    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    IntegerFlagsGraphWithUnpackingData * chGraph = chLoader.loadFlagsGraphWithUnpackingData();

    IntegerXenGraphLoader dijkstraGraphLoader = IntegerXenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    IntegerGraph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    Timer timer("CH path correctness validation");
    timer.begin();

    DistanceCorrectnessValidator::validateOnGivenTrips(trips, *chGraph, *dijkstraGraph);

    timer.finish();
    timer.printMeasuredTime();

    delete chGraph;
    delete dijkstraGraph;
}

// Simple main function parsing the command line input and invoking the relevant functions if needed.
//______________________________________________________________________________________________________________________
int main(int argc, char * argv[]) {
    //createTNR();
    createTNRAF();
    //compareMethods();
    //compareFourMethods();
    //compareCHandTNR();
    //compareVariousTransitSetSizes();

    //validateCHPathCorectness();
    //validateTNRPathCorectness();

    //memoryUsageOfDijkstra();
    //memoryUsageOfCH();
    //memoryUsageOfTNR();
    //memoryUsageOfDM();

    //createDM();

    //createCH();
    //validateCHPaths();
    //getDijkstraPathForTrip(0);
    //getCHPathForTrip(172);
    //checkDijkstraDistances(23993, 18595, 22059, 23696);
    //getTNRPathForTrip(0);

    /*if (argc != 6) {
        printUsageInfo(argv[0]);
        return 0;
    }

    if (argv[1][0] != 'c') {
        printf("Invalid command. Valid command is only 'c' for create currently.\n"
               "Run the program simply as '%s' to get usage info.\n", argv[0]);
        return 0;
    }

    if (argv[2][0] != 'i' && argv[2][0] != 'f') {
        printf("Invalid precision. Valid are only 'i' and 'f' currently.\n"
               "Run the program simply as '%s' to get usage info.\n", argv[0]);
        return 0;
    }

    if (argv[3][0] != 'X' && argv[3][0] != 'D') {
        printf("Invalid input format. Valid are only 'X' (XenGraph) and 'D' (DIMACS) currently.\n"
               "Run the program simply as '%s' to get usage info.\n", argv[0]);
        return 0;
    }

    if (argv[2][0] == 'i') { // INTEGER
        if (argv[3][0] == 'X') { // int XenGraph
            createIntegerXenGraphHierarchy(argv[4], argv[5]);
        } else { // int DIMACS
            createIntegerDIMACSHierarchy(argv[4], argv[5]);
        }
    } else { // FLOATING POINT
        if (argv[3][0] == 'X') { // double XenGraph
            createFloatingPointXenGraphHierarchy(argv[4], argv[5]);
        } else { // double DIMACS - not implemented.
            printf("Invalid combination 'f D'. Only integer hierarchies can be created for graphs.\n"
                   "in the DIMACS input format. Try 'i D' instead if you have a DIMACS file.\n"
                   "Run the program simply as '%s' to get usage info.\n", argv[0]);
            return 0;
        }
    }*/

    return 0;
}
