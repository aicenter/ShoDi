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
#include "GraphBuilding/Loaders/XenGraphLoader.h"
#include "Timer/Timer.h"
#include "CH/CHPreprocessor.h"
#include "CH/CHDistanceQueryManagerWithMapping.h"
#include "TNR/TNRPreprocessor.h"
#include "TNRAF/TNRAFPreprocessor.h"
#include "GraphBuilding/Loaders/TripsLoader.h"
#include "GraphBuilding/Loaders/DDSGLoader.h"
#include "Benchmarking/CHBenchmark.h"
#include "Benchmarking/DijkstraBenchmark.h"
#include "Benchmarking/CorrectnessValidator.h"
#include "Benchmarking/TNRBenchmark.h"
#include "Benchmarking/TNRAFBenchmark.h"
#include "Benchmarking/DistanceCorrectnessValidator.h"
#include "Benchmarking/PathCorrectnessValidator.h"
#include "CH/CHPathQueryManager.h"
#include "Dijkstra/BasicDijkstra.h"
#include "TNR/TNRPathQueryManager.h"
#include "DistanceMatrix/DistanceMatrixComputor.h"
#include "GraphBuilding/Loaders/DistanceMatrixLoader.h"
#include "Benchmarking/DistanceMatrixBenchmark.h"
#include "GraphBuilding/Loaders/TGAFLoader.h"
#include "TNRAF/TNRAFDistanceQueryManager.h"
#include "API/CHDistanceQueryManagerAPI.h"


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

    XenGraphLoader graphLoader = XenGraphLoader(inputFilePath);
    //DIMACSLoader graphLoader = DIMACSLoader("../input/graph.gr");
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    CHPreprocessor::preprocessForDDSG(*graph);
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
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);
    graph->flushInDdsgFormat(outputFilePath);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}



// Creates a TNR structure based on the given XenGraph file.
//______________________________________________________________________________________________________________________
void createTNR() {
    Timer timer("Whole TNR construction timer");
    timer.begin();

    XenGraphLoader graphLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessUsingCH(*graph, "../input/Prague_n500_unpack", 500);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

//______________________________________________________________________________________________________________________
void createTNRslower() {
    Timer timer("Whole TNR construction timer (slower preprocessing mode)");
    timer.begin();

    XenGraphLoader graphLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessUsingCHslower(*graph, *originalGraph, "../input/Prague_n1000_slower_mar", 1000);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}


//______________________________________________________________________________________________________________________
void createTNRwithValidation() {
    Timer timer("Whole TNR construction timer");
    timer.begin();

    //XenGraphLoader graphLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    XenGraphLoader graphLoader = XenGraphLoader("../input/Prague1mar.xeng");
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessWithDMvalidation(*graph, *originalGraph, "../input/Prague_mar_2000_prec1", 2000);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Creates a TNR structure based on the given XenGraph file.
//______________________________________________________________________________________________________________________
void createTNRAF() {
    Timer timer("Whole TNR AF construction timer");
    timer.begin();

    XenGraphLoader graphLoader = XenGraphLoader("../input/Prague1mar.xeng");
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRAFPreprocessor::preprocessUsingCH(*graph, *originalGraph, "../input/Prague_mar_2000_prec1", 2000, 32, true);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Creates a Contraction Hierarchy based on the given XenGraph file.
//______________________________________________________________________________________________________________________
void createCH() {
    Timer timer("Whole CH construction timer");
    timer.begin();

    XenGraphLoader graphLoader = XenGraphLoader("../input/Prague1mar.xeng");
    //XenGraphLoader graphLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);
    graph->flushInDdsgFormat("../input/Prague_mar_prec1");

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

// Creates a Distance Matrix based on the given file containing a graph.
//______________________________________________________________________________________________________________________
void createDM() {
    Timer timer("Whole Distance Matrix computation timer");
    timer.begin();

    XenGraphLoader dijkstraGraphLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    Graph * graph = dijkstraGraphLoader.loadGraph();

    DistanceMatrixComputor dmComputor;
    dmComputor.computeDistanceMatrix(*graph);
    dmComputor.outputDistanceMatrixToFile("../input/Prague_int_1000prec");

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

//______________________________________________________________________________________________________________________
void compareCHwithDijkstra() {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_mar_5000_trips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader("../input/Prague_mar_prec1.ch");
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, chDistances);

    delete ch;

    //DIMACSLoader dijkstraLoader = DIMACSLoader("../input/graph.gr");
    XenGraphLoader dijkstraLoader = XenGraphLoader("../input/Prague1mar.xeng");
    Graph * dijkstraGraph = dijkstraLoader.loadGraph();

    vector<unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistances);

    CorrectnessValidator::validateVerbose(chDistances, dijkstraDistances);
    printf("Contraction Hierarchies were %lf times faster than Dijkstra!\n", dijkstraTime/chTime);

    delete dijkstraGraph;
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
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, chDistances);

    delete ch;

    //DIMACSLoader dijkstraLoader = DIMACSLoader("../input/graph.gr");
    XenGraphLoader dijkstraLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    Graph * dijkstraGraph = dijkstraLoader.loadGraph();

    vector<unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistances);

    CorrectnessValidator::validateVerbose(chDistances, dijkstraDistances);
    printf("Contraction Hierarchies were %lf times faster than Dijkstra!\n", dijkstraTime/chTime);

    delete dijkstraGraph;

    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_map_1000_sept.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnrGraph, tnrDistances);

    CorrectnessValidator::validateVerbose(tnrDistances, dijkstraDistances);
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
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, chDistances);

    delete ch;

    //DIMACSLoader dijkstraLoader = DIMACSLoader("../input/graph.gr");
    XenGraphLoader dijkstraLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    Graph * dijkstraGraph = dijkstraLoader.loadGraph();

    vector<unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistances);

    CorrectnessValidator::validateVerbose(chDistances, dijkstraDistances);
    printf("Contraction Hierarchies were %lf times faster than Dijkstra!\n", dijkstraTime/chTime);

    delete dijkstraGraph;

    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_n2000_DM_mar.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnrGraph, tnrDistances);

    CorrectnessValidator::validateVerbose(tnrDistances, dijkstraDistances);
    printf("TNR was %lf times faster than Dijkstra!\n", dijkstraTime/tnrTime);
    printf("TNR was %lf times faster than CH!\n", chTime/tnrTime);

    delete tnrGraph;

    DistanceMatrixLoader distanceMatrixLoader = DistanceMatrixLoader("../input/Prague_int_1000prec.xdm");
    DistanceMatrix * distanceMatrix = distanceMatrixLoader.loadDistanceMatrix();

    vector<unsigned int> dmDistances(trips.size());
    double dmTime = DistanceMatrixBenchmark::runAndMeasureOutputAndRetval(trips, *distanceMatrix, dmDistances);

    CorrectnessValidator::validateVerbose(dmDistances, dijkstraDistances);
    printf("Distance Matrix was %lf times faster than Dijkstra!\n", dijkstraTime/dmTime);
    printf("Distance Matrix was %lf times faster than CH!\n", chTime/dmTime);
    printf("Distance Matrix was %lf times faster than TNR!\n", tnrTime/dmTime);
}

//______________________________________________________________________________________________________________________
void compareThreeOnNewPrague() {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_mar_5000_trips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader("../input/Prague_mar_prec1.ch");
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, chDistances);

    delete ch;

    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_mar_2000_prec1.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnrGraph, tnrDistances);

    CorrectnessValidator::validateVerbose(tnrDistances, chDistances);
    printf("TNR was %lf times faster than CH!\n", chTime/tnrTime);

    delete tnrGraph;

    TGAFLoader tnrafLoader = TGAFLoader("../input/Prague_mar_2000_prec1.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    vector<unsigned int> tnrafDistances(trips.size());
    double tnrafTime = TNRAFBenchmark::runAndMeasureOutputAndRetval(trips, *tnrafGraph, tnrafDistances);

    CorrectnessValidator::validateVerbose(tnrafDistances, chDistances);
    printf("TNRAF was %lf times faster than CH!\n", chTime/tnrafTime);
    printf("TNRAF was %lf times faster than TNR!\n", tnrTime/tnrafTime);
}

// Compares the running times of Dijkstra, Contraction Hierarchies, Transit Node Routing and Distance Matrix on a given
// set of trips (queries).
//______________________________________________________________________________________________________________________
void compareFiveMethods() {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_50000_randomTrips.txt");
    //TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, chDistances);

    delete ch;

    //DIMACSLoader dijkstraLoader = DIMACSLoader("../input/graph.gr");
    XenGraphLoader dijkstraLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    Graph * dijkstraGraph = dijkstraLoader.loadGraph();

    vector<unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistances);

    CorrectnessValidator::validateVerbose(chDistances, dijkstraDistances);
    printf("Contraction Hierarchies were %lf times faster than Dijkstra!\n", dijkstraTime/chTime);

    delete dijkstraGraph;

    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_n2000_DM_mar.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnrGraph, tnrDistances);

    CorrectnessValidator::validateVerbose(tnrDistances, dijkstraDistances);
    printf("TNR was %lf times faster than Dijkstra!\n", dijkstraTime/tnrTime);
    printf("TNR was %lf times faster than CH!\n", chTime/tnrTime);

    delete tnrGraph;

    TGAFLoader tnrafLoader = TGAFLoader("../input/Prague_n2000_useDM_mar_clust.tgaf");
    //TGAFLoader tnrafLoader = TGAFLoader("../input/Prague_n500_useDM_mar_clust_var2.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    vector<unsigned int> tnrafDistances(trips.size());
    double tnrafTime = TNRAFBenchmark::runAndMeasureOutputAndRetval(trips, *tnrafGraph, tnrafDistances);

    CorrectnessValidator::validateVerbose(tnrafDistances, dijkstraDistances);
    printf("TNRAF was %lf times faster than Dijkstra!\n", dijkstraTime/tnrafTime);
    printf("TNRAF was %lf times faster than CH!\n", chTime/tnrafTime);
    printf("TNRAF was %lf times faster than TNR!\n", tnrTime/tnrafTime);

    delete tnrafGraph;

    DistanceMatrixLoader distanceMatrixLoader = DistanceMatrixLoader("../input/Prague_int_1000prec.xdm");
    DistanceMatrix * distanceMatrix = distanceMatrixLoader.loadDistanceMatrix();

    vector<unsigned int> dmDistances(trips.size());
    double dmTime = DistanceMatrixBenchmark::runAndMeasureOutputAndRetval(trips, *distanceMatrix, dmDistances);

    CorrectnessValidator::validateVerbose(dmDistances, dijkstraDistances);
    printf("Distance Matrix was %lf times faster than Dijkstra!\n", dijkstraTime/dmTime);
    printf("Distance Matrix was %lf times faster than CH!\n", chTime/dmTime);
    printf("Distance Matrix was %lf times faster than TNR!\n", tnrTime/dmTime);
    printf("Distance Matrix was %lf times faster than TNRAF!\n", tnrafTime/dmTime);

    delete distanceMatrix;
}

// Compares the running times of Contraction Hierarchies and Transit Node Routing on a given set of trips (queries).
//______________________________________________________________________________________________________________________
void compareNewPragueCHandTNRAF() {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_mar_50000_trips.txt");
    //TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader("../input/Prague_mar.ch");
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, chDistances);

    delete ch;

    TGAFLoader tnrafLoader = TGAFLoader("../input/Prague_mar_2000_integ.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    vector<unsigned int> tnrafDistances(trips.size());
    double tnrafTime = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnrafGraph, tnrafDistances);

    CorrectnessValidator::validateVerbose(tnrafDistances, chDistances);
    printf("TNRAF was %lf times faster than CH!\n", chTime/tnrafTime);

    delete tnrafGraph;
}


// Compares the running times of Contraction Hierarchies and Transit Node Routing on a given set of trips (queries).
//______________________________________________________________________________________________________________________
void compareCHandTNR() {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_50000_randomTrips.txt");
    //TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    //DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    DDSGLoader chLoader = DDSGLoader("../input/old_Prague_mar.ch");
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, chDistances);

    delete ch;

    //TNRGLoader tnrLoader = TNRGLoader("../input/Prague_n500_unpack.tnrg");
    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_n2000_DM_mar.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnrGraph, tnrDistances);

    CorrectnessValidator::validateVerbose(tnrDistances, chDistances);
    printf("TNR was %lf times faster than CH!\n", chTime/tnrTime);

    delete tnrGraph;

}

//______________________________________________________________________________________________________________________
void compareTNRandTNRAF() {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_50000_randomTrips.txt");
    //TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    //TNRGLoader tnrLoader = TNRGLoader("../input/Prague_n500_unpack.tnrg");
    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_n2000_DM_mar.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnrGraph, tnrDistances);

    delete tnrGraph;

    TGAFLoader tnrafLoader = TGAFLoader("../input/Prague_n5000_useDM_mar_clust.tgaf");
    //TGAFLoader tnrafLoader = TGAFLoader("../input/Prague_n500_useDM_mar_clust_var2.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    vector<unsigned int> tnrafDistances(trips.size());
    double tnrafTime = TNRAFBenchmark::runAndMeasureOutputAndRetval(trips, *tnrafGraph, tnrafDistances);

    delete tnrafGraph;

    CorrectnessValidator::validateVerbose(tnrDistances, tnrafDistances);
    printf("TNR with Arc Flags was %lf times faster than basic TNR!\n", tnrTime/tnrafTime);


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

    vector<unsigned int> tnr500Distances(trips.size());
    double tnr500Time = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnr500Graph, tnr500Distances);

    delete tnr500Graph;

    TNRGLoader tnr1000Loader = TNRGLoader("../input/Prague_map_1000_newFilter.tnrg");
    TransitNodeRoutingGraph * tnr1000Graph = tnr1000Loader.loadTNRforDistanceQueries();

    vector<unsigned int> tnr1000Distances(trips.size());
    double tnr1000Time = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnr1000Graph, tnr1000Distances);

    delete tnr1000Graph;

    TNRGLoader tnr2000Loader = TNRGLoader("../input/Prague_map_1000_newFilter_2000nodes.tnrg");
    TransitNodeRoutingGraph * tnr2000Graph = tnr2000Loader.loadTNRforDistanceQueries();

    vector<unsigned int> tnr2000Distances(trips.size());
    double tnr2000Time = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnr2000Graph, tnr2000Distances);

    delete tnr2000Graph;

    CorrectnessValidator::validateVerbose(tnr500Distances, tnr1000Distances);
    CorrectnessValidator::validateVerbose(tnr500Distances, tnr2000Distances);

    printf("TNR with  500 transit nodes time: %lf (%lf for one query)\n", tnr500Time, tnr500Time/trips.size());
    printf("TNR with 1000 transit nodes time: %lf (%lf for one query)\n", tnr1000Time, tnr1000Time/trips.size());
    printf("TNR with 2000 transit nodes time: %lf (%lf for one query)\n", tnr2000Time, tnr2000Time/trips.size());



}

//______________________________________________________________________________________________________________________
void compareTwoTNRinstances() {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_50000_randomTrips.txt");
    //TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    TNRGLoader tnr1loader = TNRGLoader("../input/Prague_n1000_DM_mar.tnrg");
    TransitNodeRoutingGraph * tnr1graph = tnr1loader.loadTNRforDistanceQueries();

    vector<unsigned int> tnr1distances(trips.size());
    double tnr1time = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnr1graph, tnr1distances);

    delete tnr1graph;

    TNRGLoader tnr2loader = TNRGLoader("../input/Prague_n1000_slower_mar.tnrg");
    TransitNodeRoutingGraph * tnr2graph = tnr2loader.loadTNRforDistanceQueries();

    vector<unsigned int> tnr2distances(trips.size());
    double tnr2time = TNRBenchmark::runAndMeasureOutputAndRetval(trips, *tnr2graph, tnr2distances);

    delete tnr2graph;

    CorrectnessValidator::validateVerbose(tnr1distances, tnr2distances);

    printf("TNR instance 1 time: %lf (%lf for one query)\n", tnr1time, tnr1time / trips.size());
    printf("TNR instance 2 time: %lf (%lf for one query)\n", tnr2time, tnr2time / trips.size());
}

//______________________________________________________________________________________________________________________
void compareTwoTNRAFinstances() {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_50000_randomTrips.txt");
    //TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    TGAFLoader tnraf1Loader = TGAFLoader("../input/Prague_n1000_useDM_mar_clust.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnraf1Graph = tnraf1Loader.loadTNRAFforDistanceQueries();

    vector<unsigned int> tnraf1Distances(trips.size());
    double tnraf1Time = TNRAFBenchmark::runAndMeasureOutputAndRetval(trips, *tnraf1Graph, tnraf1Distances);

    delete tnraf1Graph;

    TGAFLoader tnraf2Loader = TGAFLoader("../input/Prague_n1000_slower_mar_v2.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnraf2Graph = tnraf2Loader.loadTNRAFforDistanceQueries();

    vector<unsigned int> tnraf2Distances(trips.size());
    double tnraf2Time = TNRAFBenchmark::runAndMeasureOutputAndRetval(trips, *tnraf2Graph, tnraf2Distances);

    delete tnraf2Graph;

    CorrectnessValidator::validateVerbose(tnraf1Distances, tnraf2Distances);

    printf("TNRAF instance 1 time: %lf (%lf for one query)\n", tnraf1Time, tnraf1Time / trips.size());
    printf("TNRAF instance 2 time: %lf (%lf for one query)\n", tnraf2Time, tnraf2Time / trips.size());
}

//______________________________________________________________________________________________________________________
void testCHWithMapping() {
    TripsLoader tripsLoader = TripsLoader("../input/test5000queries.txt");
    vector< pair < long long unsigned int, long long unsigned int > > trips;
    tripsLoader.loadLongLongTrips(trips);

    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    CHDistanceQueryManagerWithMapping qm(*ch, "../input/Prague_int_graph.xeni");

    printf("Printing distances:\n");
    for(unsigned int i = 0; i < trips.size(); ++i) {
        printf("%lf\n", (double) qm.findDistance(trips[i].first, trips[i].second));
    }

    delete ch;

}

//______________________________________________________________________________________________________________________
void generateRandomTestset(unsigned int queriesCnt = 5000) {
    XenGraphLoader mappingLoader("../input/Prague_int_graph.xeni");
    unordered_map <long long unsigned int, unsigned int> mapping;
    mappingLoader.loadNodesMapping(mapping);

    vector<long long unsigned int> reversedMapping;
    for(auto iter = mapping.begin(); iter != mapping.end(); ++iter) {
        reversedMapping.push_back((*iter).first);
    }

    printf("%u\n", queriesCnt);
    for(unsigned int i = 0; i < queriesCnt; ++i) {
        printf("%llu %llu\n", reversedMapping[rand() % static_cast<int>(queriesCnt)], reversedMapping[rand() % static_cast<int>(queriesCnt)]);
    }
}

//______________________________________________________________________________________________________________________
void validateCHPathCorectness() {
    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    FlagsGraphWithUnpackingData * chGraph = chLoader.loadFlagsGraphWithUnpackingData();
    CHPathQueryManager queryManager = CHPathQueryManager(*chGraph);

    XenGraphLoader dijkstraLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    Graph * originalGraph = dijkstraLoader.loadGraph();

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

    XenGraphLoader dijkstraLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    Graph * originalGraph = dijkstraLoader.loadGraph();

    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    PathCorrectnessValidator::validateTNRPaths(originalGraph, queryManager, trips);

    delete originalGraph;
    delete tnrGraph;

}

//______________________________________________________________________________________________________________________
void perform10CHqueries() {
    DDSGLoader chLoader = DDSGLoader("../input/Prague_mar_prec1.ch");
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    CHDistanceQueryManager dqm = CHDistanceQueryManager(*ch);
    for(unsigned int i = 0; i < 10; ++i) {
        unsigned int dist = dqm.findDistance(i*100,i*1000+500);
        printf("Returned distance %u for query %u.\n", dist, i);
    }

    delete ch;
}

//______________________________________________________________________________________________________________________
void perform5CHqueriesUsingAPI() {
    CHDistanceQueryManagerAPI api;
    api.initializeCH("../input/Prague_mar_prec1.ch", "../input/Prague1mar.xeni");
    printf("Returned distance %u for a query.\n", api.distanceQuery(1440730650034861,1467438550039272));
    printf("Returned distance %u for a query.\n", api.distanceQuery(1444249950054331,1443750050126777));
    printf("Returned distance %u for a query.\n", api.distanceQuery(1450395350122241,1464074449944693));
    printf("Returned distance %u for a query.\n", api.distanceQuery(1464451550153981,1453186149972638));
    printf("Returned distance %u for a query.\n", api.distanceQuery(1432635949952211, 1456436050112046));
    //printf("Returned distance %u for a query.\n", api.distanceQuery(1432635949952211, 1456436050112046));
    //printf("Returned distance %u for a query.\n", api.distanceQuery(1432635949952211, 1456436050112046));
    //printf("Returned distance %u for a query.\n", api.distanceQuery(1432635949952211, 1456436050112046));
    api.clearStructures();
}


//______________________________________________________________________________________________________________________
void memoryUsageOfDijkstra() {
    XenGraphLoader dijkstraGraphLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    delete dijkstraGraph;
}

//______________________________________________________________________________________________________________________
void memoryUsageOfCH() {
    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    delete ch;
}

//______________________________________________________________________________________________________________________
void memoryUsageOfTNR() {
    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_n2000_DM_mar.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    delete tnrGraph;
}

//______________________________________________________________________________________________________________________
void memoryUsageOfTNRAF() {
    TGAFLoader tnrafLoader = TGAFLoader("../input/Prague_n2000_useDM_mar_clust.tgaf");
    TransitNodeRoutingGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    delete tnrafGraph;
}

//______________________________________________________________________________________________________________________
void memoryUsageOfDM() {
    DistanceMatrixLoader distanceMatrixLoader = DistanceMatrixLoader("../input/Prague_int_1000prec.xdm");
    DistanceMatrix * distanceMatrix = distanceMatrixLoader.loadDistanceMatrix();

    delete distanceMatrix;
}

//______________________________________________________________________________________________________________________
void checkDMBigValues() {
    DistanceMatrixLoader distanceMatrixLoader = DistanceMatrixLoader("../input/Prague_int_1000prec.xdm");
    DistanceMatrix * distanceMatrix = distanceMatrixLoader.loadDistanceMatrix();
    distanceMatrix -> printInfo();

    delete distanceMatrix;
}

// Auxiliary function currently used for debug purposes. Prints the real distance from source to access node, between
// twe two access nodes and from access node to target. Might help determine which of these parts is being computed
// incorrectly in TNR.
//______________________________________________________________________________________________________________________
void checkDijkstraDistances(unsigned int sourceNode, unsigned int sourceAccess, unsigned int targetAccess, unsigned int targetNode) {
    XenGraphLoader dijkstraGraphLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    printf("~~~ Computing distances using Dijkstra ~~~\n");
    printf("%u -> %u: %u\n", sourceNode, sourceAccess, BasicDijkstra::run(sourceNode, sourceAccess, *dijkstraGraph));
    printf("%u -> %u: %u\n", sourceAccess, targetAccess, BasicDijkstra::run(sourceAccess, targetAccess, *dijkstraGraph));
    printf("%u -> %u: %u\n", targetAccess, targetNode, BasicDijkstra::run(targetAccess, targetNode, *dijkstraGraph));
    printf("~~~ End of Dijkstra computation ~~~\n");
    delete dijkstraGraph;

}

//______________________________________________________________________________________________________________________
void getDijkstraPathForTrip(unsigned int tripNum) {
    XenGraphLoader dijkstraGraphLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    //BasicDijkstra::runWithPathOutput(trips[tripNum].first, trips[tripNum].second, *dijkstraGraph);
    BasicDijkstra::runWithPathOutput(trips[tripNum].first /*20652*/, trips[tripNum].second, *dijkstraGraph);

    delete dijkstraGraph;

}

// Will print the node sequence on a certain path computed by Contraction Hierarchies - this means that the paths will
// be unpacked from the shortcuts. Can be used for debug, especially if CH returns different paths than Dijkstra.
//______________________________________________________________________________________________________________________
void getCHPathForTrip(unsigned int tripNum) {
    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    FlagsGraphWithUnpackingData * chGraph = chLoader.loadFlagsGraphWithUnpackingData();

    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    CHPathQueryManager queryManager(*chGraph);
    //chGraph->debugPrint();
    unsigned int distance = queryManager.findDistanceOutputPath(trips[tripNum].first,
                                                                          trips[tripNum].second);
    printf("Returned distance: %u\n", distance);

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

//______________________________________________________________________________________________________________________
void computeTNRAFPathForTrip(unsigned int tripNum, unsigned int expectedDistance) {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    TGAFLoader tnrafLoader = TGAFLoader("../input/Prague_n500_useDM_mar.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    TNRAFDistanceQueryManager queryManager(*tnrafGraph);

    unsigned int distance = queryManager.findDistance(trips[tripNum].first, trips[tripNum].second);
    printf("Returned distance: %u\n", distance);
    printf("Expected distance was: %u\n", expectedDistance);

    delete tnrafGraph;
}

//______________________________________________________________________________________________________________________
void computeTNRAFPath3Times(unsigned int tripNum, unsigned int expectedDistance) {
    TripsLoader tripsLoader = TripsLoader("../input/Prague_map_5000randomTrips.txt");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    TGAFLoader tnrafLoader = TGAFLoader("../input/Prague_n500_useDM_mar.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    TNRAFDistanceQueryManager queryManager(*tnrafGraph);

    unsigned int distance = queryManager.findDistance(trips[tripNum].first, trips[tripNum].second);
    printf("1st try: Returned distance: %u\n", distance);
    distance = queryManager.findDistance(trips[tripNum].first, trips[tripNum].second);
    printf("2nd try: Returned distance: %u\n", distance);
    distance = queryManager.findDistance(trips[tripNum].first, trips[tripNum].second);
    printf("3rd try: Returned distance: %u\n", distance);
    printf("Expected distance was: %u\n", expectedDistance);

    delete tnrafGraph;
}

// This function runs Contraction Hierarchies on a given set of path queries. Each returned path is validated in order
// to check if the paths actually exists in the original graph. This ensures that the unpacking routine for the path
// queries in Contraction Hierarchies works correctly and only valid paths are returned.
//______________________________________________________________________________________________________________________
void validateCHPaths() {
    DDSGLoader chLoader = DDSGLoader("../input/Prague_map_int_prec1000.ch");
    FlagsGraphWithUnpackingData * chGraph = chLoader.loadFlagsGraphWithUnpackingData();

    XenGraphLoader dijkstraGraphLoader = XenGraphLoader("../input/Prague_int_graph_1000prec.xeng");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

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

//______________________________________________________________________________________________________________________
void validateTNRAccessNodes() {
    DistanceMatrixLoader distanceMatrixLoader = DistanceMatrixLoader("../input/Prague_int_1000prec.xdm");
    DistanceMatrix * distanceMatrix = distanceMatrixLoader.loadDistanceMatrix();

    TNRGLoader tnrLoader = TNRGLoader("../input/Prague_n500_unpack.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    tnrGraph->accessNodesTest(*distanceMatrix);

    delete distanceMatrix;
    delete tnrGraph;
}

// Simple main function parsing the command line input and invoking the relevant functions if needed.
//______________________________________________________________________________________________________________________
int main(int argc, char * argv[]) {
    //createTNR();
    //createTNRslower();
    //createTNRwithValidation();
    //createTNRAF();
    //compareCHwithDijkstra();
    //compareMethods();
    //compareThreeOnNewPrague();
    //compareFourMethods();
    //compareFiveMethods();
    //compareCHandTNR();
    //compareTNRandTNRAF();
    //compareTwoTNRinstances();
    //compareTwoTNRAFinstances();
    //compareVariousTransitSetSizes();
    //compareNewPragueCHandTNRAF();

    //testCHWithMapping();

    //generateRandomTestset();

    //validateCHPathCorectness();
    //validateTNRPathCorectness();
    //validateTNRAccessNodes();

    //perform10CHqueries();
    perform5CHqueriesUsingAPI();

    //memoryUsageOfDijkstra();
    //memoryUsageOfCH();
    //memoryUsageOfTNR();
    //memoryUsageOfTNRAF();
    //memoryUsageOfDM();

    //checkDMBigValues();

    //createDM();

    //createCH();
    //validateCHPaths();
    //getDijkstraPathForTrip(0);
    //getCHPathForTrip(172);
    //checkDijkstraDistances(23993, 18595, 22059, 23696);
    //getTNRPathForTrip(0);
    //computeTNRAFPathForTrip(4939, 649324);
    //computeTNRAFPathForTrip(4369, 692498);
    //computeTNRAFPathForTrip(4510, 494056);
    //computeTNRAFPath3Times(4369, 692498);
    //computeTNRAFPath3Times(4510, 494056);
    //computeTNRAFPath3Times(4939, 649324);

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
