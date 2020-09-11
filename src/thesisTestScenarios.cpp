//
// Author: Xenty (Michal Cvach)
// Created on: 6.5.20
//

#include "DistanceMatrix/DistanceMatrixComputorSlow.h"
#include "GraphBuilding/Loaders/XenGraphLoader.h"
#include "GraphBuilding/Loaders/DDSGLoader.h"
#include "GraphBuilding/Loaders/TNRGLoader.h"
#include "GraphBuilding/Loaders/TGAFLoader.h"
#include "GraphBuilding/Loaders/DistanceMatrixLoader.h"
#include "GraphBuilding/Loaders/TripsLoader.h"
#include "Benchmarking/DijkstraBenchmark.h"
#include "Benchmarking/CHBenchmark.h"
#include "Benchmarking/TNRBenchmark.h"
#include "Benchmarking/TNRAFBenchmark.h"
#include "Benchmarking/DistanceMatrixBenchmark.h"
#include "Benchmarking/CorrectnessValidator.h"
#include "CH/CHPreprocessor.h"
#include "GraphBuilding/Structures/UpdateableGraph.h"
#include "Timer/Timer.h"
#include "TNR/TNRPreprocessor.h"
#include "TNRAF/TNRAFPreprocessor.h"
#include "DistanceMatrix/DistanceMatrixXdmOutputter.h"
#include <boost/numeric/conversion/cast.hpp>

/*
 * The exact code in this file was used to obtain the benchmark data presented in the thesis.
 * Using this file, anyone can redo the tests to validate our results.
 */

/**
 * Auxiliary function that creates a Contraction Hierarchy.
 *
 * @param inputFilePath[in] Path to the input graph.
 * @param outputFilePath[in] The desired output file path for the precomputed data structure.
 */
void createCH(
        char const * inputFilePath,
        char const * outputFilePath) {
    Timer timer("Contraction Hierarchies from XenGraph preprocessing");
    timer.begin();

    XenGraphLoader graphLoader(inputFilePath);
    UpdateableGraph g1(graphLoader.nodes());
    graphLoader.loadGraph(g1, 1);

    UpdateableGraph graph(g1);
    CHPreprocessor::preprocessForDDSG(graph);
    graph.addAllEdges(g1);
    graph.flushInDdsgFormat(outputFilePath);

    timer.finish();
    timer.printMeasuredTime();
}

/**
 * Auxiliary function that precomputes the data structures for Transit Node Routing.
 *
 * @param inputFilePath[in] Path to the input graph.
 * @param outputFilePath[in] The desired output file path for the precomputed data structure.
 */
void createTNR(
        char const * inputFilePath,
        unsigned int transitNodeSetSize,
        char const * outputFilePath) {
    Timer timer("Transit Node Routing from XenGraph preprocessing (using distance matrix)");
    timer.begin();

    XenGraphLoader graphLoader(inputFilePath);
    UpdateableGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, 1);
    Graph *originalGraph = graph.createCopy();

    CHPreprocessor::preprocessForDDSG(graph);
    graph.addAllEdges(*originalGraph);

    TNRPreprocessor::preprocessWithDMvalidation(graph, *originalGraph, outputFilePath, transitNodeSetSize);

    delete originalGraph;

    timer.finish();
    timer.printMeasuredTime();
}

/**
 * Auxiliary function that precomputes the data structures for Transit Node Routing with Arc Flags.
 *
 * @param inputFilePath[in] Path to the input graph.
 * @param outputFilePath[in] The desired output file path for the precomputed data structure.
 */
void createTNRAF(
        char const * inputFilePath,
        unsigned int transitNodeSetSize,
        char const * outputFilePath) {
    Timer timer("Transit Node Routing with Arc Flags from XenGraph preprocessing (using distance matrix)");
    timer.begin();

    XenGraphLoader graphLoader(inputFilePath);
    UpdateableGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, 1);
    Graph *originalGraph = graph.createCopy();

    CHPreprocessor::preprocessForDDSG(graph);
    graph.addAllEdges(*originalGraph);

    TNRAFPreprocessor::preprocessUsingCH(graph, *originalGraph, outputFilePath, transitNodeSetSize, 32, true);

    delete originalGraph;

    timer.finish();
    timer.printMeasuredTime();
}

/**
 * Auxiliary function that precomputes the full Distance Matrix for the graph.
 *
 * @param inputFilePath[in] Path to the input graph.
 * @param outputFilePath[in] The desired output file path for the precomputed data structure.
 */
void createDM(
        char const * inputFilePath,
        char const * outputFilePath) {
    Timer timer("Whole Distance Matrix computation timer");
    timer.begin();

    XenGraphLoader dijkstraGraphLoader(inputFilePath);

    DistanceMatrixComputorSlow dmComputor;
    dmComputor.computeDistanceMatrix(dijkstraGraphLoader, 1);

    DistanceMatrix * dm = dmComputor.getDistanceMatrixInstance();
    DistanceMatrixXdmOutputter outputter;
    outputter.store(*dm, outputFilePath);

    timer.finish();
    timer.printMeasuredTime();

    delete dm;
}

/**
 * Auxiliary function that precomputes all the data structures used for the first benchmark (graph of Prague).
 *
 * @param inputFilePath[in] The path to the graph of Prague in the XenGraph format.
 */
void computeStructuresForAllMethodsPrague(char const * inputFilePath = "../thesisTestsData/Prague/Prague.xeng") {
    createCH(inputFilePath, "../thesisTestsData/Prague/Prague");
    createTNR(inputFilePath, 2000, "../thesisTestsData/Prague/Prague2000tnodes");
    createTNRAF(inputFilePath, 2000, "../thesisTestsData/Prague/Prague2000tnodes");
    createDM(inputFilePath, "../thesisTestsData/Prague/Prague");
}

/**
 * Auxiliary function that precomputes all the data structures used for the second benchmark (graph of Berlin).
 *
 * @param inputFilePath[in] The path to the graph of Berlin in the XenGraph format.
 */
void computeStructuresForAllMethodsBerlin(char const * inputFilePath = "../thesisTestsData/Berlin/Berlin.xeng") {
    createCH(inputFilePath, "../thesisTestsData/Berlin/Berlin");
    createTNR(inputFilePath, 5000, "../thesisTestsData/Berlin/Berlin5000tnodes");
    createTNRAF(inputFilePath, 5000, "../thesisTestsData/Berlin/Berlin5000tnodes");
    createDM(inputFilePath, "../thesisTestsData/Berlin/Berlin");
}

/**
 * Auxiliary function that precomputes all the data structures used for the third benchmark (graph of SouthwestBohemia).
 *
 * @param inputFilePath[in] The path to the graph of SouthwestBohemia in the XenGraph format.
 */
void computeStructuresForAllMethodsSouthwestBohemia(char const * inputFilePath = "../thesisTestsData/SouthwestBohemia/SouthwestBohemia.xeng") {
    //createCH(inputFilePath, "../thesisTestsData/SouthwestBohemia/SouthwestBohemia");
    createTNR(inputFilePath, 7000, "../thesisTestsData/SouthwestBohemia/SouthwestBohemia7000tnodes");
    createTNRAF(inputFilePath, 7000, "../thesisTestsData/SouthwestBohemia/SouthwestBohemia7000tnodes");
    createDM(inputFilePath, "../thesisTestsData/SouthwestBohemia/SouthwestBohemia");
}

/**
 * Auxiliary function that precomputes Transit Node Routing data structures for the graph of Prague with 6 different
 * transit node-set sizes.
 *
 * @param inputFilePath[in] The path to the graph of Prague in the XenGraph format.
 */
void computeTNRvariousTransitNodeSetSizes(char const * inputFilePath = "../thesisTestsData/Prague/Prague.xeng") {
    createTNR(inputFilePath, 200, "../thesisTestsData/Prague/Prague200tnodes");
    createTNR(inputFilePath, 500, "../thesisTestsData/Prague/Prague500tnodes");
    createTNR(inputFilePath, 1000, "../thesisTestsData/Prague/Prague1000tnodes");
    createTNR(inputFilePath, 2000, "../thesisTestsData/Prague/Prague2000tnodes");
    createTNR(inputFilePath, 5000, "../thesisTestsData/Prague/Prague5000tnodes");
    createTNR(inputFilePath, 10000, "../thesisTestsData/Prague/Prague10000tnodes");
}

/**
 * Auxiliary function that precomputes Transit Node Routing with Arc Flags data structures for the graph of Prague
 * with 6 different transit node-set sizes.
 *
 * @param inputFilePath[in] The path to the graph of Prague in the XenGraph format.
 */
void computeTNRAFvariousTransitNodeSetSizes(char const * inputFilePath = "../thesisTestsData/Prague/Prague.xeng") {
    createTNRAF(inputFilePath, 200, "../thesisTestsData/Prague/Prague200tnodes");
    createTNRAF(inputFilePath, 500, "../thesisTestsData/Prague/Prague500tnodes");
    createTNRAF(inputFilePath, 1000, "../thesisTestsData/Prague/Prague1000tnodes");
    createTNRAF(inputFilePath, 2000, "../thesisTestsData/Prague/Prague2000tnodes");
    createTNRAF(inputFilePath, 5000, "../thesisTestsData/Prague/Prague5000tnodes");
    createTNRAF(inputFilePath, 10000, "../thesisTestsData/Prague/Prague10000tnodes");
}

/**
 * Compares the five methods on the graph of Prague using a query set containing 100 000 random queries.
 * The results of this benchmark are reported in section 7.2.1 of the thesis.
 *
 * @param runs[in] The number of times the benchmark should be repeated. The average values are returned at the end.
 * More runs should provide more accurate values.
 */
void compareAllMethodsOnPrague(unsigned int runs = 20) {
    XenGraphLoader dijkstraGraphLoader("../thesisTestsData/Prague/Prague.xeng");
    Graph dijkstraGraph(dijkstraGraphLoader.nodes());
    dijkstraGraphLoader.loadGraph(dijkstraGraph, 1);

    DDSGLoader chLoader = DDSGLoader("../thesisTestsData/Prague/Prague.ch");
    FlagsGraph * chGraph = chLoader.loadFlagsGraph();

    TNRGLoader tnrLoader = TNRGLoader("../thesisTestsData/Prague/Prague2000tnodes.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    TGAFLoader tnrafLoader = TGAFLoader("../thesisTestsData/Prague/Prague2000tnodes.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    DistanceMatrixLoader dmLoader = DistanceMatrixLoader("../thesisTestsData/Prague/Prague.xdm");
    DistanceMatrix * dm = dmLoader.loadDistanceMatrix();

    TripsLoader querySetLoader = TripsLoader("../thesisTestsData/Prague/Prague100000randomQueries.txt");
    vector<pair<unsigned int, unsigned int>> querySet;
    querySetLoader.loadTrips(querySet);
    unsigned int queriesCnt = boost::numeric_cast<unsigned int>(querySet.size());

    double cummulativeDijkstraTime = 0;
    double cummulativeCHTime = 0;
    double cummulativeTNRTime = 0;
    double cummulativeTNRAFTime = 0;
    double cummulativeDMTime = 0;

    bool validResults = true;

    for(unsigned int i = 0; i < runs; ++i) {
        printf("\r%u runs completed.", i);

        vector<unsigned int> dijkstraDistances(queriesCnt);
        vector<unsigned int> chDistances(queriesCnt);
        vector<unsigned int> tnrDistances(queriesCnt);
        vector<unsigned int> tnrafDistances(queriesCnt);
        vector<unsigned int> dmDistances(queriesCnt);

        cummulativeDijkstraTime += DijkstraBenchmark::benchmark(querySet, dijkstraGraph, dijkstraDistances);
        cummulativeCHTime += CHBenchmark::benchmark(querySet, *chGraph, chDistances);
        cummulativeTNRTime += TNRBenchmark::benchmark(querySet, *tnrGraph, tnrDistances);
        cummulativeTNRAFTime += TNRAFBenchmark::benchmark(querySet, *tnrafGraph, tnrafDistances);
        cummulativeDMTime += DistanceMatrixBenchmark::benchmark(querySet, *dm, dmDistances);

        if(CorrectnessValidator::validate(dijkstraDistances, chDistances) == false ||
           CorrectnessValidator::validate(dijkstraDistances, tnrDistances) == false ||
           CorrectnessValidator::validate(dijkstraDistances, tnrafDistances) == false ||
           CorrectnessValidator::validate(dijkstraDistances, dmDistances) == false) {
            printf("Oops! Something went wrong!\n"
                   "Some values were incorrect during the benchmark runs.\n"
                   "Stopping the benchmark, no times will be reported.\n");
            validResults = false;
            break;
        }
    }

    printf("\r%u runs completed.\n", runs);

    if(validResults) {
        printf("Reporting benchmark results using %u queries with %u runs.\n", queriesCnt, runs);

        printf("Dijkstra's Algorithm: average time to process the whole query set in seconds: %f\n", cummulativeDijkstraTime / runs);
        printf("Contraction Hierarchies: average time to process the whole query set in seconds: %f\n", cummulativeCHTime / runs);
        printf("Transit Node Routing: average time to process the whole query set in seconds: %f\n", cummulativeTNRTime / runs);
        printf("Transit Node Routing with Arc Flags: average time to process the whole query set in seconds: %f\n", cummulativeTNRAFTime / runs);
        printf("Distance Matrix: average time to process the whole query set in seconds: %f\n", cummulativeDMTime / runs);

        printf("Dijkstra's Algorithm: average time to process one query in milliseconds: %f\n", cummulativeDijkstraTime / (runs * queriesCnt * 0.001) );
        printf("Contraction Hierarchies: average time to process one query in milliseconds: %f\n", cummulativeCHTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing: average time to process one query in milliseconds: %f\n", cummulativeTNRTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing with Arc Flags: average time to process one query in milliseconds: %f\n", cummulativeTNRAFTime / (runs * queriesCnt * 0.001));
        printf("Distance Matrix: average time to process one query in milliseconds: %f\n", cummulativeDMTime / (runs * queriesCnt * 0.001));

        printf("\nContraction Hierarchies were:\n");
        printf("  %f times faster than Dijkstra's algorithm.\n", cummulativeDijkstraTime/cummulativeCHTime);

        printf("\nTransit Node Routing was:\n");
        printf("  %f times faster than Dijkstra's algorithm.\n", cummulativeDijkstraTime/cummulativeTNRTime);
        printf("  %f times faster than Contraction Hierarchies.\n", cummulativeCHTime/cummulativeTNRTime);

        printf("\nTransit Node Routing with Arc Flags was:\n");
        printf("  %f times faster than Dijkstra's algorithm.\n", cummulativeDijkstraTime/cummulativeTNRAFTime);
        printf("  %f times faster than Contraction Hierarchies.\n", cummulativeCHTime/cummulativeTNRAFTime);
        printf("  %f times faster than Transit Node Routing.\n", cummulativeTNRTime/cummulativeTNRAFTime);

        printf("\nDistance Matrix was:\n");
        printf("  %f times faster than Dijkstra's algorithm.\n", cummulativeDijkstraTime/cummulativeDMTime);
        printf("  %f times faster than Contraction Hierarchies.\n", cummulativeCHTime/cummulativeDMTime);
        printf("  %f times faster than Transit Node Routing.\n", cummulativeTNRTime/cummulativeDMTime);
        printf("  %f times faster than Transit Node Routing with Arc Flags.\n", cummulativeTNRAFTime/cummulativeDMTime);
    }

    delete chGraph;
    delete tnrGraph;
    delete tnrafGraph;
    delete dm;
}

/**
 * Compares the five methods on the graph of Berlin using a query set containing 100 000 random queries.
 * The results of this benchmark are reported in section 7.2.2 of the thesis.
 *
 * @param runs[in] The number of times the benchmark should be repeated. The average values are returned at the end.
 * More runs should provide more accurate values.
 */
void compareAllMethodsOnBerlin(unsigned int runs = 20) {
    XenGraphLoader dijkstraGraphLoader("../thesisTestsData/Berlin/Berlin.xeng");
    Graph dijkstraGraph(dijkstraGraphLoader.nodes());
    dijkstraGraphLoader.loadGraph(dijkstraGraph, 1);

    DDSGLoader chLoader = DDSGLoader("../thesisTestsData/Berlin/Berlin.ch");
    FlagsGraph * chGraph = chLoader.loadFlagsGraph();

    TNRGLoader tnrLoader = TNRGLoader("../thesisTestsData/Berlin/Berlin5000tnodes.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    TGAFLoader tnrafLoader = TGAFLoader("../thesisTestsData/Berlin/Berlin5000tnodes.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    DistanceMatrixLoader dmLoader = DistanceMatrixLoader("../thesisTestsData/Berlin/Berlin.xdm");
    DistanceMatrix * dm = dmLoader.loadDistanceMatrix();

    TripsLoader querySetLoader = TripsLoader("../thesisTestsData/Berlin/Berlin100000randomQueries.txt");
    vector<pair<unsigned int, unsigned int>> querySet;
    querySetLoader.loadTrips(querySet);
    unsigned int queriesCnt = boost::numeric_cast<unsigned int>(querySet.size());

    double cummulativeDijkstraTime = 0;
    double cummulativeCHTime = 0;
    double cummulativeTNRTime = 0;
    double cummulativeTNRAFTime = 0;
    double cummulativeDMTime = 0;

    bool validResults = true;

    for(unsigned int i = 0; i < runs; ++i) {
        printf("\r%u runs completed.", i);

        vector<unsigned int> dijkstraDistances(queriesCnt);
        vector<unsigned int> chDistances(queriesCnt);
        vector<unsigned int> tnrDistances(queriesCnt);
        vector<unsigned int> tnrafDistances(queriesCnt);
        vector<unsigned int> dmDistances(queriesCnt);

        cummulativeDijkstraTime += DijkstraBenchmark::benchmark(querySet, dijkstraGraph, dijkstraDistances);
        cummulativeCHTime += CHBenchmark::benchmark(querySet, *chGraph, chDistances);
        cummulativeTNRTime += TNRBenchmark::benchmark(querySet, *tnrGraph, tnrDistances);
        cummulativeTNRAFTime += TNRAFBenchmark::benchmark(querySet, *tnrafGraph, tnrafDistances);
        cummulativeDMTime += DistanceMatrixBenchmark::benchmark(querySet, *dm, dmDistances);

        if(CorrectnessValidator::validate(dijkstraDistances, chDistances) == false ||
           CorrectnessValidator::validate(dijkstraDistances, tnrDistances) == false ||
           CorrectnessValidator::validate(dijkstraDistances, tnrafDistances) == false ||
           CorrectnessValidator::validate(dijkstraDistances, dmDistances) == false) {
            printf("Oops! Something went wrong!\n"
                   "Some values were incorrect during the benchmark runs.\n"
                   "Stopping the benchmark, no times will be reported.\n");
            validResults = false;
            break;
        }
    }

    printf("\r%u runs completed.\n", runs);

    if(validResults) {
        printf("Reporting benchmark results using %u queries with %u runs.\n", queriesCnt, runs);

        printf("Dijkstra's Algorithm: average time to process the whole query set in seconds: %f\n", cummulativeDijkstraTime / runs);
        printf("Contraction Hierarchies: average time to process the whole query set in seconds: %f\n", cummulativeCHTime / runs);
        printf("Transit Node Routing: average time to process the whole query set in seconds: %f\n", cummulativeTNRTime / runs);
        printf("Transit Node Routing with Arc Flags: average time to process the whole query set in seconds: %f\n", cummulativeTNRAFTime / runs);
        printf("Distance Matrix: average time to process the whole query set in seconds: %f\n", cummulativeDMTime / runs);

        printf("Dijkstra's Algorithm: average time to process one query in milliseconds: %f\n", cummulativeDijkstraTime / (runs * queriesCnt * 0.001) );
        printf("Contraction Hierarchies: average time to process one query in milliseconds: %f\n", cummulativeCHTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing: average time to process one query in milliseconds: %f\n", cummulativeTNRTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing with Arc Flags: average time to process one query in milliseconds: %f\n", cummulativeTNRAFTime / (runs * queriesCnt * 0.001));
        printf("Distance Matrix: average time to process one query in milliseconds: %f\n", cummulativeDMTime / (runs * queriesCnt * 0.001));

        printf("\nContraction Hierarchies were:\n");
        printf("  %f times faster than Dijkstra's algorithm.\n", cummulativeDijkstraTime/cummulativeCHTime);

        printf("\nTransit Node Routing was:\n");
        printf("  %f times faster than Dijkstra's algorithm.\n", cummulativeDijkstraTime/cummulativeTNRTime);
        printf("  %f times faster than Contraction Hierarchies.\n", cummulativeCHTime/cummulativeTNRTime);

        printf("\nTransit Node Routing with Arc Flags was:\n");
        printf("  %f times faster than Dijkstra's algorithm.\n", cummulativeDijkstraTime/cummulativeTNRAFTime);
        printf("  %f times faster than Contraction Hierarchies.\n", cummulativeCHTime/cummulativeTNRAFTime);
        printf("  %f times faster than Transit Node Routing.\n", cummulativeTNRTime/cummulativeTNRAFTime);

        printf("\nDistance Matrix was:\n");
        printf("  %f times faster than Dijkstra's algorithm.\n", cummulativeDijkstraTime/cummulativeDMTime);
        printf("  %f times faster than Contraction Hierarchies.\n", cummulativeCHTime/cummulativeDMTime);
        printf("  %f times faster than Transit Node Routing.\n", cummulativeTNRTime/cummulativeDMTime);
        printf("  %f times faster than Transit Node Routing with Arc Flags.\n", cummulativeTNRAFTime/cummulativeDMTime);
    }

    delete chGraph;
    delete tnrGraph;
    delete tnrafGraph;
    delete dm;
}

/**
 * Compares the five methods on the graph of SouthwestBohemia using a query set containing 100 000 random queries.
 * The results of this benchmark are reported in section 7.2.3 of the thesis.
 *
 * @param runs[in] The number of times the benchmark should be repeated. The average values are returned at the end.
 * More runs should provide more accurate values.
 */
void compareAllMethodsOnSouthwestBohemia(unsigned int runs = 20) {
    XenGraphLoader dijkstraGraphLoader("../thesisTestsData/SouthwestBohemia/SouthwestBohemia.xeng");
    Graph dijkstraGraph(dijkstraGraphLoader.nodes());
    dijkstraGraphLoader.loadGraph(dijkstraGraph, 1);

    DDSGLoader chLoader = DDSGLoader("../thesisTestsData/SouthwestBohemia/SouthwestBohemia.ch");
    FlagsGraph * chGraph = chLoader.loadFlagsGraph();

    TNRGLoader tnrLoader = TNRGLoader("../thesisTestsData/SouthwestBohemia/SouthwestBohemia7000tnodes.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    TGAFLoader tnrafLoader = TGAFLoader("../thesisTestsData/SouthwestBohemia/SouthwestBohemia7000tnodes.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    DistanceMatrixLoader dmLoader = DistanceMatrixLoader("../thesisTestsData/SouthwestBohemia/SouthwestBohemia.xdm");
    DistanceMatrix * dm = dmLoader.loadDistanceMatrix();

    TripsLoader querySetLoader = TripsLoader("../thesisTestsData/SouthwestBohemia/SouthwestBohemia100000randomQueries.txt");
    vector<pair<unsigned int, unsigned int>> querySet;
    querySetLoader.loadTrips(querySet);
    unsigned int queriesCnt = boost::numeric_cast<unsigned int>(querySet.size());

    double cummulativeDijkstraTime = 0;
    double cummulativeCHTime = 0;
    double cummulativeTNRTime = 0;
    double cummulativeTNRAFTime = 0;
    double cummulativeDMTime = 0;

    bool validResults = true;

    for(unsigned int i = 0; i < runs; ++i) {
        printf("\r%u runs completed.", i);

        vector<unsigned int> dijkstraDistances(queriesCnt);
        vector<unsigned int> chDistances(queriesCnt);
        vector<unsigned int> tnrDistances(queriesCnt);
        vector<unsigned int> tnrafDistances(queriesCnt);
        vector<unsigned int> dmDistances(queriesCnt);

        cummulativeDijkstraTime += DijkstraBenchmark::benchmark(querySet, dijkstraGraph, dijkstraDistances);
        cummulativeCHTime += CHBenchmark::benchmark(querySet, *chGraph, chDistances);
        cummulativeTNRTime += TNRBenchmark::benchmark(querySet, *tnrGraph, tnrDistances);
        cummulativeTNRAFTime += TNRAFBenchmark::benchmark(querySet, *tnrafGraph, tnrafDistances);
        cummulativeDMTime += DistanceMatrixBenchmark::benchmark(querySet, *dm, dmDistances);

        if(CorrectnessValidator::validate(dijkstraDistances, chDistances) == false ||
           CorrectnessValidator::validate(dijkstraDistances, tnrDistances) == false ||
           CorrectnessValidator::validate(dijkstraDistances, tnrafDistances) == false ||
           CorrectnessValidator::validate(dijkstraDistances, dmDistances) == false) {
            printf("Oops! Something went wrong!\n"
                   "Some values were incorrect during the benchmark runs.\n"
                   "Stopping the benchmark, no times will be reported.\n");
            validResults = false;
            break;
        }
    }

    printf("\r%u runs completed.\n", runs);

    if(validResults) {
        printf("Reporting benchmark results using %u queries with %u runs.\n", queriesCnt, runs);

        printf("Dijkstra's Algorithm: average time to process the whole query set in seconds: %f\n", cummulativeDijkstraTime / runs);
        printf("Contraction Hierarchies: average time to process the whole query set in seconds: %f\n", cummulativeCHTime / runs);
        printf("Transit Node Routing: average time to process the whole query set in seconds: %f\n", cummulativeTNRTime / runs);
        printf("Transit Node Routing with Arc Flags: average time to process the whole query set in seconds: %f\n", cummulativeTNRAFTime / runs);
        printf("Distance Matrix: average time to process the whole query set in seconds: %f\n", cummulativeDMTime / runs);

        printf("Dijkstra's Algorithm: average time to process one query in milliseconds: %f\n", cummulativeDijkstraTime / (runs * queriesCnt * 0.001) );
        printf("Contraction Hierarchies: average time to process one query in milliseconds: %f\n", cummulativeCHTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing: average time to process one query in milliseconds: %f\n", cummulativeTNRTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing with Arc Flags: average time to process one query in milliseconds: %f\n", cummulativeTNRAFTime / (runs * queriesCnt * 0.001));
        printf("Distance Matrix: average time to process one query in milliseconds: %f\n", cummulativeDMTime / (runs * queriesCnt * 0.001));

        printf("\nContraction Hierarchies were:\n");
        printf("  %f times faster than Dijkstra's algorithm.\n", cummulativeDijkstraTime/cummulativeCHTime);

        printf("\nTransit Node Routing was:\n");
        printf("  %f times faster than Dijkstra's algorithm.\n", cummulativeDijkstraTime/cummulativeTNRTime);
        printf("  %f times faster than Contraction Hierarchies.\n", cummulativeCHTime/cummulativeTNRTime);

        printf("\nTransit Node Routing with Arc Flags was:\n");
        printf("  %f times faster than Dijkstra's algorithm.\n", cummulativeDijkstraTime/cummulativeTNRAFTime);
        printf("  %f times faster than Contraction Hierarchies.\n", cummulativeCHTime/cummulativeTNRAFTime);
        printf("  %f times faster than Transit Node Routing.\n", cummulativeTNRTime/cummulativeTNRAFTime);

        printf("\nDistance Matrix was:\n");
        printf("  %f times faster than Dijkstra's algorithm.\n", cummulativeDijkstraTime/cummulativeDMTime);
        printf("  %f times faster than Contraction Hierarchies.\n", cummulativeCHTime/cummulativeDMTime);
        printf("  %f times faster than Transit Node Routing.\n", cummulativeTNRTime/cummulativeDMTime);
        printf("  %f times faster than Transit Node Routing with Arc Flags.\n", cummulativeTNRAFTime/cummulativeDMTime);
    }

    delete chGraph;
    delete tnrGraph;
    delete tnrafGraph;
    delete dm;
}

/**
 * Compares Transit Node Routing with 6 different transit node-set sizes on the graph of Prague using a query
 * set containing 100 000 random queries.
 * The results of this benchmark are reported in section 7.2.4 of the thesis.
 *
 * @param runs[in] The number of times the benchmark should be repeated. The average values are returned at the end.
 * More runs should provide more accurate values.
 */
void compareTNRwithVariousTransitNodeSetSizes(unsigned int runs = 20) {
    TNRGLoader tnr200Loader = TNRGLoader("../thesisTestsData/Prague/Prague200tnodes.tnrg");
    TransitNodeRoutingGraph * tnr200Graph = tnr200Loader.loadTNRforDistanceQueries();

    TNRGLoader tnr500Loader = TNRGLoader("../thesisTestsData/Prague/Prague500tnodes.tnrg");
    TransitNodeRoutingGraph * tnr500Graph = tnr500Loader.loadTNRforDistanceQueries();

    TNRGLoader tnr1000Loader = TNRGLoader("../thesisTestsData/Prague/Prague1000tnodes.tnrg");
    TransitNodeRoutingGraph * tnr1000Graph = tnr1000Loader.loadTNRforDistanceQueries();

    TNRGLoader tnr2000Loader = TNRGLoader("../thesisTestsData/Prague/Prague2000tnodes.tnrg");
    TransitNodeRoutingGraph * tnr2000Graph = tnr2000Loader.loadTNRforDistanceQueries();

    TNRGLoader tnr5000Loader = TNRGLoader("../thesisTestsData/Prague/Prague5000tnodes.tnrg");
    TransitNodeRoutingGraph * tnr5000Graph = tnr5000Loader.loadTNRforDistanceQueries();

    TNRGLoader tnr10000Loader = TNRGLoader("../thesisTestsData/Prague/Prague10000tnodes.tnrg");
    TransitNodeRoutingGraph * tnr10000Graph = tnr10000Loader.loadTNRforDistanceQueries();

    TripsLoader querySetLoader = TripsLoader("../thesisTestsData/Prague/Prague100000randomQueries.txt");
    vector<pair<unsigned int, unsigned int>> querySet;
    querySetLoader.loadTrips(querySet);
    unsigned int queriesCnt = boost::numeric_cast<unsigned int>(querySet.size());

    double cummulative200tNodesTime = 0;
    double cummulative500tNodesTime = 0;
    double cummulative1000tNodesTime = 0;
    double cummulative2000tNodesTime = 0;
    double cummulative5000tNodesTime = 0;
    double cummulative10000tNodesTime = 0;

    bool validResults = true;

    for(unsigned int i = 0; i < runs; ++i) {
        printf("\r%u runs completed.", i);

        vector<unsigned int> distances200tNodes(queriesCnt);
        vector<unsigned int> distances500tNodes(queriesCnt);
        vector<unsigned int> distances1000tNodes(queriesCnt);
        vector<unsigned int> distances2000tNodes(queriesCnt);
        vector<unsigned int> distances5000tNodes(queriesCnt);
        vector<unsigned int> distances10000tNodes(queriesCnt);

        cummulative200tNodesTime += TNRBenchmark::benchmark(querySet, *tnr200Graph, distances200tNodes);
        cummulative500tNodesTime += TNRBenchmark::benchmark(querySet, *tnr500Graph, distances500tNodes);
        cummulative1000tNodesTime += TNRBenchmark::benchmark(querySet, *tnr1000Graph, distances1000tNodes);
        cummulative2000tNodesTime += TNRBenchmark::benchmark(querySet, *tnr2000Graph, distances2000tNodes);
        cummulative5000tNodesTime += TNRBenchmark::benchmark(querySet, *tnr5000Graph, distances5000tNodes);
        cummulative10000tNodesTime += TNRBenchmark::benchmark(querySet, *tnr10000Graph, distances10000tNodes);

        if(CorrectnessValidator::validate(distances2000tNodes, distances200tNodes) == false ||
           CorrectnessValidator::validate(distances2000tNodes, distances500tNodes) == false ||
           CorrectnessValidator::validate(distances2000tNodes, distances1000tNodes) == false ||
           CorrectnessValidator::validate(distances2000tNodes, distances5000tNodes) == false ||
                CorrectnessValidator::validate(distances2000tNodes, distances10000tNodes) == false) {
            printf("Oops! Something went wrong!\n"
                   "Some values were incorrect during the benchmark runs.\n"
                   "Stopping the benchmark, no times will be reported.\n");
            validResults = false;
            break;
        }
    }

    printf("\r%u runs completed.\n", runs);

    if(validResults) {
        printf("Reporting benchmark results using %u queries with %u runs.\n", queriesCnt, runs);

        printf("Transit Node Routing (200 transit nodes): average time to process the whole query set in seconds: %f\n", cummulative200tNodesTime / runs);
        printf("Transit Node Routing (500 transit nodes): average time to process the whole query set in seconds: %f\n", cummulative500tNodesTime / runs);
        printf("Transit Node Routing (1000 transit nodes): average time to process the whole query set in seconds: %f\n", cummulative1000tNodesTime / runs);
        printf("Transit Node Routing (2000 transit nodes): average time to process the whole query set in seconds: %f\n", cummulative2000tNodesTime / runs);
        printf("Transit Node Routing (5000 transit nodes): average time to process the whole query set in seconds: %f\n", cummulative5000tNodesTime / runs);
        printf("Transit Node Routing (10000 transit nodes): average time to process the whole query set in seconds: %f\n", cummulative10000tNodesTime / runs);

        printf("Transit Node Routing (200 transit nodes): average time to process one query in milliseconds: %f\n", cummulative200tNodesTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing (500 transit nodes): average time to process one query in milliseconds: %f\n", cummulative500tNodesTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing (1000 transit nodes): average time to process one query in milliseconds: %f\n", cummulative1000tNodesTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing (2000 transit nodes): average time to process one query in milliseconds: %f\n", cummulative2000tNodesTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing (5000 transit nodes): average time to process one query in milliseconds: %f\n", cummulative5000tNodesTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing (10000 transit nodes): average time to process one query in milliseconds: %f\n", cummulative10000tNodesTime / (runs * queriesCnt * 0.001));
    }

    delete tnr200Graph;
    delete tnr500Graph;
    delete tnr1000Graph;
    delete tnr2000Graph;
    delete tnr5000Graph;
    delete tnr10000Graph;
}

/**
 * Compares Transit Node Routing with Arc Flags with 6 different transit node-set sizes
 * on the graph of Prague using a query set containing 100 000 random queries.
 * The results of this benchmark are reported in section 7.2.5 of the thesis.
 *
 * @param runs[in] The number of times the benchmark should be repeated. The average values are returned at the end.
 * More runs should provide more accurate values.
 */
void compareTNRAFwithVariousTransitNodeSetSizes(unsigned int runs = 20) {
    TGAFLoader tnraf200Loader = TGAFLoader("../thesisTestsData/Prague/Prague200tnodes.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnraf200Graph = tnraf200Loader.loadTNRAFforDistanceQueries();

    TGAFLoader tnraf500Loader = TGAFLoader("../thesisTestsData/Prague/Prague500tnodes.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnraf500Graph = tnraf500Loader.loadTNRAFforDistanceQueries();

    TGAFLoader tnraf1000Loader = TGAFLoader("../thesisTestsData/Prague/Prague1000tnodes.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnraf1000Graph = tnraf1000Loader.loadTNRAFforDistanceQueries();

    TGAFLoader tnraf2000Loader = TGAFLoader("../thesisTestsData/Prague/Prague2000tnodes.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnraf2000Graph = tnraf2000Loader.loadTNRAFforDistanceQueries();

    TGAFLoader tnraf5000Loader = TGAFLoader("../thesisTestsData/Prague/Prague5000tnodes.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnraf5000Graph = tnraf5000Loader.loadTNRAFforDistanceQueries();

    TGAFLoader tnraf10000Loader = TGAFLoader("../thesisTestsData/Prague/Prague10000tnodes.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnraf10000Graph = tnraf10000Loader.loadTNRAFforDistanceQueries();

    TripsLoader querySetLoader = TripsLoader("../thesisTestsData/Prague/Prague100000randomQueries.txt");
    vector<pair<unsigned int, unsigned int>> querySet;
    querySetLoader.loadTrips(querySet);
    unsigned int queriesCnt = boost::numeric_cast<unsigned int>(querySet.size());

    double cummulative200tNodesTime = 0;
    double cummulative500tNodesTime = 0;
    double cummulative1000tNodesTime = 0;
    double cummulative2000tNodesTime = 0;
    double cummulative5000tNodesTime = 0;
    double cummulative10000tNodesTime = 0;

    bool validResults = true;

    for(unsigned int i = 0; i < runs; ++i) {
        printf("\r%u runs completed.", i);

        vector<unsigned int> distances200tNodes(queriesCnt);
        vector<unsigned int> distances500tNodes(queriesCnt);
        vector<unsigned int> distances1000tNodes(queriesCnt);
        vector<unsigned int> distances2000tNodes(queriesCnt);
        vector<unsigned int> distances5000tNodes(queriesCnt);
        vector<unsigned int> distances10000tNodes(queriesCnt);

        cummulative200tNodesTime += TNRAFBenchmark::benchmark(querySet, *tnraf200Graph, distances200tNodes);
        cummulative500tNodesTime += TNRAFBenchmark::benchmark(querySet, *tnraf500Graph, distances500tNodes);
        cummulative1000tNodesTime += TNRAFBenchmark::benchmark(querySet, *tnraf1000Graph, distances1000tNodes);
        cummulative2000tNodesTime += TNRAFBenchmark::benchmark(querySet, *tnraf2000Graph, distances2000tNodes);
        cummulative5000tNodesTime += TNRAFBenchmark::benchmark(querySet, *tnraf5000Graph, distances5000tNodes);
        cummulative10000tNodesTime += TNRAFBenchmark::benchmark(querySet, *tnraf10000Graph, distances10000tNodes);

        if(CorrectnessValidator::validate(distances2000tNodes, distances200tNodes) == false ||
           CorrectnessValidator::validate(distances2000tNodes, distances500tNodes) == false ||
           CorrectnessValidator::validate(distances2000tNodes, distances1000tNodes) == false ||
           CorrectnessValidator::validate(distances2000tNodes, distances5000tNodes) == false ||
           CorrectnessValidator::validate(distances2000tNodes, distances10000tNodes) == false) {
            printf("Oops! Something went wrong!\n"
                   "Some values were incorrect during the benchmark runs.\n"
                   "Stopping the benchmark, no times will be reported.\n");
            validResults = false;
            break;
        }
    }

    printf("\r%u runs completed.\n", runs);

    if(validResults) {
        printf("Reporting benchmark results using %u queries with %u runs.\n", queriesCnt, runs);

        printf("Transit Node Routing with Arc Flags (200 transit nodes): average time to process the whole query set in seconds: %f\n", cummulative200tNodesTime / runs);
        printf("Transit Node Routing with Arc Flags (500 transit nodes): average time to process the whole query set in seconds: %f\n", cummulative500tNodesTime / runs);
        printf("Transit Node Routing with Arc Flags (1000 transit nodes): average time to process the whole query set in seconds: %f\n", cummulative1000tNodesTime / runs);
        printf("Transit Node Routing with Arc Flags (2000 transit nodes): average time to process the whole query set in seconds: %f\n", cummulative2000tNodesTime / runs);
        printf("Transit Node Routing with Arc Flags (5000 transit nodes): average time to process the whole query set in seconds: %f\n", cummulative5000tNodesTime / runs);
        printf("Transit Node Routing with Arc Flags (10000 transit nodes): average time to process the whole query set in seconds: %f\n", cummulative10000tNodesTime / runs);

        printf("Transit Node Routing with Arc Flags (200 transit nodes): average time to process one query in milliseconds: %f\n", cummulative200tNodesTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing with Arc Flags (500 transit nodes): average time to process one query in milliseconds: %f\n", cummulative500tNodesTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing with Arc Flags (1000 transit nodes): average time to process one query in milliseconds: %f\n", cummulative1000tNodesTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing with Arc Flags (2000 transit nodes): average time to process one query in milliseconds: %f\n", cummulative2000tNodesTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing with Arc Flags (5000 transit nodes): average time to process one query in milliseconds: %f\n", cummulative5000tNodesTime / (runs * queriesCnt * 0.001));
        printf("Transit Node Routing with Arc Flags (10000 transit nodes): average time to process one query in milliseconds: %f\n", cummulative10000tNodesTime / (runs * queriesCnt * 0.001));
    }

    delete tnraf200Graph;
    delete tnraf500Graph;
    delete tnraf1000Graph;
    delete tnraf2000Graph;
    delete tnraf5000Graph;
    delete tnraf10000Graph;
}

/**
 * A main function that by default runs all the tests. You can comment out the tests you do not want to perform.
 */
int main() {
    setbuf(stdout, NULL);

    //computeStructuresForAllMethodsPrague("../thesisTestsData/Prague/Prague.xeng");
    //computeStructuresForAllMethodsBerlin("../thesisTestsData/Berlin/Berlin.xeng");
    //computeStructuresForAllMethodsSouthwestBohemia("../thesisTestsData/SouthwestBohemia/SouthwestBohemia.xeng");

    //computeTNRvariousTransitNodeSetSizes("../thesisTestsData/Prague/Prague.xeng");
    //computeTNRAFvariousTransitNodeSetSizes("../thesisTestsData/Prague/Prague.xeng");

    compareAllMethodsOnPrague(20);
    compareAllMethodsOnBerlin(20);
    compareAllMethodsOnSouthwestBohemia(20);

    compareTNRwithVariousTransitNodeSetSizes(20);
    compareTNRAFwithVariousTransitNodeSetSizes(20);

    return 0;
}
