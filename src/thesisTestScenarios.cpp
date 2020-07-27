//
// Author: Xenty (Michal Cvach)
// Created on: 6.5.20
//

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
#include "Timer/Timer.h"
#include "TNR/TNRPreprocessor.h"
#include "TNRAF/TNRAFPreprocessor.h"
#include "DistanceMatrix/DistanceMatrixComputor.h"

void createCH(
        char const * inputFilePath,
        char const * outputFilePath) {
    Timer timer("Contraction Hierarchies from XenGraph preprocessing");
    timer.begin();

    XenGraphLoader graphLoader = XenGraphLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);
    graph->flushInDdsgFormat(outputFilePath);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

void createTNR(
        char const * inputFilePath,
        unsigned int transitNodeSetSize,
        char const * outputFilePath) {
    Timer timer("Transit Node Routing from XenGraph preprocessing (using distance matrix)");
    timer.begin();

    XenGraphLoader graphLoader = XenGraphLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessWithDMvalidation(*graph, *originalGraph, outputFilePath, transitNodeSetSize);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

void createTNRAF(
        char const * inputFilePath,
        unsigned int transitNodeSetSize,
        char const * outputFilePath) {
    Timer timer("Transit Node Routing with Arc Flags from XenGraph preprocessing (using distance matrix)");
    timer.begin();

    XenGraphLoader graphLoader = XenGraphLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRAFPreprocessor::preprocessUsingCH(*graph, *originalGraph, outputFilePath, transitNodeSetSize, 32, true);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

void createDM(
        char const * inputFilePath,
        char const * outputFilePath) {
    Timer timer("Whole Distance Matrix computation timer");
    timer.begin();

    XenGraphLoader dijkstraGraphLoader = XenGraphLoader(inputFilePath);
    Graph * graph = dijkstraGraphLoader.loadGraph();

    DistanceMatrixComputor dmComputor;
    dmComputor.computeDistanceMatrix(*graph);

    DistanceMatrix * dm = dmComputor.getDistanceMatrixInstance();
    dm->outputToXdm(outputFilePath);

    timer.finish();
    timer.printMeasuredTime();

    delete dm;
    delete graph;
}

void computeStructuresForAllMethodsBerlin(char const * inputFilePath = "../thesisTestsData/Berlin/Berlin.xeng") {
    createCH(inputFilePath, "../thesisTestsData/Berlin/Berlin");
    createTNR(inputFilePath, 5000, "../thesisTestsData/Berlin/Berlin5000tnodes");
    createTNRAF(inputFilePath, 5000, "../thesisTestsData/Berlin/Berlin5000tnodes");
    createDM(inputFilePath, "../thesisTestsData/Berlin/Berlin");
}

void computeStructuresForAllMethodsPrague(char const * inputFilePath = "../thesisTestsData/Prague/Prague.xeng") {
    createCH(inputFilePath, "../thesisTestsData/Prague/Prague");
    createTNR(inputFilePath, 2000, "../thesisTestsData/Prague/Prague2000tnodes");
    createTNRAF(inputFilePath, 2000, "../thesisTestsData/Prague/Prague2000tnodes");
    createDM(inputFilePath, "../thesisTestsData/Prague/Prague");
}

void computeStructuresForAllMethodsPragueToPilsen(char const * inputFilePath = "../thesisTestsData/PragueToPilsen/PragueToPilsen.xeng") {
    createCH(inputFilePath, "../thesisTestsData/PragueToPilsen/PragueToPilsen");
    createTNR(inputFilePath, 7000, "../thesisTestsData/PragueToPilsen/PragueToPilsen7000tnodes");
    createTNRAF(inputFilePath, 7000, "../thesisTestsData/PragueToPilsen/PragueToPilsen7000tnodes");
    createDM(inputFilePath, "../thesisTestsData/PragueToPilsen/PragueToPilsen");
}

void computeTNRvariousTransitNodeSetSizes(char const * inputFilePath = "../thesisTestsData/Prague/Prague.xeng") {
    createTNR(inputFilePath, 200, "../thesisTestsData/Prague/Prague200tnodes");
    createTNR(inputFilePath, 500, "../thesisTestsData/Prague/Prague500tnodes");
    createTNR(inputFilePath, 1000, "../thesisTestsData/Prague/Prague1000tnodes");
    createTNR(inputFilePath, 2000, "../thesisTestsData/Prague/Prague2000tnodes");
    createTNR(inputFilePath, 5000, "../thesisTestsData/Prague/Prague5000tnodes");
    createTNR(inputFilePath, 10000, "../thesisTestsData/Prague/Prague10000tnodes");
}

void computeTNRAFvariousTransitNodeSetSizes(char const * inputFilePath = "../thesisTestsData/Prague/Prague.xeng") {
    createTNRAF(inputFilePath, 200, "../thesisTestsData/Prague/Prague200tnodes");
    createTNRAF(inputFilePath, 500, "../thesisTestsData/Prague/Prague500tnodes");
    createTNRAF(inputFilePath, 1000, "../thesisTestsData/Prague/Prague1000tnodes");
    createTNRAF(inputFilePath, 2000, "../thesisTestsData/Prague/Prague2000tnodes");
    createTNRAF(inputFilePath, 5000, "../thesisTestsData/Prague/Prague5000tnodes");
    createTNRAF(inputFilePath, 10000, "../thesisTestsData/Prague/Prague10000tnodes");
}

void compareAllMethodsOnPrague(unsigned int runs = 20) {
    XenGraphLoader dijkstraGraphLoader = XenGraphLoader("../thesisTestsData/Prague/Prague.xeng");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

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
    unsigned int queriesCnt = querySet.size();

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

        cummulativeDijkstraTime += DijkstraBenchmark::benchmark(querySet, *dijkstraGraph, dijkstraDistances);
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

    delete dijkstraGraph;
    delete chGraph;
    delete tnrGraph;
    delete tnrafGraph;
    delete dm;
}

void compareAllMethodsOnBerlin(unsigned int runs = 20) {
    XenGraphLoader dijkstraGraphLoader = XenGraphLoader("../thesisTestsData/Berlin/Berlin.xeng");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

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
    unsigned int queriesCnt = querySet.size();

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

        cummulativeDijkstraTime += DijkstraBenchmark::benchmark(querySet, *dijkstraGraph, dijkstraDistances);
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

    delete dijkstraGraph;
    delete chGraph;
    delete tnrGraph;
    delete tnrafGraph;
    delete dm;
}

void compareAllMethodsOnPragueToPilsen(unsigned int runs = 20) {
    XenGraphLoader dijkstraGraphLoader = XenGraphLoader("../thesisTestsData/PragueToPilsen/PragueToPilsen.xeng");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    DDSGLoader chLoader = DDSGLoader("../thesisTestsData/PragueToPilsen/PragueToPilsen.ch");
    FlagsGraph * chGraph = chLoader.loadFlagsGraph();

    TNRGLoader tnrLoader = TNRGLoader("../thesisTestsData/PragueToPilsen/PragueToPilsen7000tnodes.tnrg");
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    TGAFLoader tnrafLoader = TGAFLoader("../thesisTestsData/PragueToPilsen/PragueToPilsen7000tnodes.tgaf");
    TransitNodeRoutingArcFlagsGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    DistanceMatrixLoader dmLoader = DistanceMatrixLoader("../thesisTestsData/PragueToPilsen/PragueToPilsen.xdm");
    DistanceMatrix * dm = dmLoader.loadDistanceMatrix();

    TripsLoader querySetLoader = TripsLoader("../thesisTestsData/PragueToPilsen/PragueToPilsen100000randomQueries.txt");
    vector<pair<unsigned int, unsigned int>> querySet;
    querySetLoader.loadTrips(querySet);
    unsigned int queriesCnt = querySet.size();

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

        cummulativeDijkstraTime += DijkstraBenchmark::benchmark(querySet, *dijkstraGraph, dijkstraDistances);
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

    delete dijkstraGraph;
    delete chGraph;
    delete tnrGraph;
    delete tnrafGraph;
    delete dm;
}

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
    unsigned int queriesCnt = querySet.size();

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
    unsigned int queriesCnt = querySet.size();

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

int main(int argc, char * argv[]) {
    setbuf(stdout, NULL);

    computeStructuresForAllMethodsPrague("../thesisTestsData/Prague.xeng");
    computeStructuresForAllMethodsBerlin("../thesisTestsData/Berlin/Berlin.xeng");
    computeStructuresForAllMethodsPragueToPilsen("../thesisTestsData/PragueToPilsen/PragueToPilsen.xeng");

    computeTNRvariousTransitNodeSetSizes("../thesisTestsData/Prague.xeng");
    computeTNRAFvariousTransitNodeSetSizes("../thesisTestsData/Prague.xeng");

    compareAllMethodsOnPrague(20);
    compareAllMethodsOnBerlin(20);
    compareAllMethodsOnPragueToPilsen(20);

    compareTNRwithVariousTransitNodeSetSizes(20);
    compareTNRAFwithVariousTransitNodeSetSizes(20);

    return 0;
}
