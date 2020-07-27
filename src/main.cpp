//
// Author: Xenty (Michal Cvach)
// Created on: 01.08.19
//

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "GraphBuilding/Loaders/DIMACSLoader.h"
#include "GraphBuilding/Loaders/TNRGLoader.h"
#include "GraphBuilding/Loaders/XenGraphLoader.h"
#include "Timer/Timer.h"
#include "CH/CHPreprocessor.h"
#include "TNR/TNRPreprocessor.h"
#include "TNRAF/TNRAFPreprocessor.h"
#include "GraphBuilding/Loaders/TripsLoader.h"
#include "GraphBuilding/Loaders/DDSGLoader.h"
#include "Benchmarking/CHBenchmark.h"
#include "Benchmarking/CorrectnessValidator.h"
#include "Benchmarking/TNRBenchmark.h"
#include "Benchmarking/TNRAFBenchmark.h"
#include "GraphBuilding/Loaders/DistanceMatrixLoader.h"
#include "GraphBuilding/Loaders/TGAFLoader.h"
#include "Benchmarking/DijkstraBenchmark.h"
#include "Error/Error.h"
#include "GraphBuilding/Loaders/CsvGraphLoader.h"
#include "DistanceMatrix/DistanceMatrixComputor.h"
#include "DistanceMatrix/johnson.hpp"

using namespace std;

/**
 * Prints info about how to use the application to the user.
 *
 * @param[in] appName
 */
void printUsageInfo(
        char *appName) {
    printf("Hello! This application allows the user to preprocess graphs to create data structures\n"
           "for Contraction Hierarchies, Transit Node Routing and Transit Node Routing with Arc Flags.\n"
           "Additionally, the user can also benchmark the obtained data structures using a given set of queries.\n"
           "Please, see 'README.md' for a complete overview of use cases for this application.\n"
           "\n"
           "Some common examples of usage:\n"
           "  '%s create tnraf xengraph dm 1000 input_graph.xeng output_file'\n"
           "  '%s create ch xengraph input_graph.xeng output_file'\n", appName, appName);
}

constexpr auto INVALID_USAGE_INFO = "It seems that the given arguments do not match any use case for this application.\n"
                                    "Please, see 'README.md' for a complete overview of use cases for this application\n"
                                    "with examples.\n";

constexpr auto INVALID_FORMAT_INFO = "Please, make sure that your call has the right format. If not sure,\n"
                                     "refer to 'README.md' for a complete overview of use cases for this application\n"
                                     "with examples.\n";

/**
 * This function will create the Contraction Hierarchies data structure based on a given input file.
 *
 * @param graphLoader TODO
 * @param outputFilePath[in] Contains the desired output file path for the precomputed CH data structure.
 */
void createCH(
        GraphLoader *graphLoader,
        char *outputFilePath) {
    Timer timer("Contraction Hierarchies from DIMACS preprocessing");
    timer.begin();

    UpdateableGraph *graph = graphLoader->loadUpdateableGraph();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader->putAllEdgesIntoUpdateableGraph(*graph);
    graph->flushInDdsgFormat(outputFilePath);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * This function will create the Transit Node Routing data structure based on a given input file.
 * In this case, the preprocessing mode is the 'fast' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param graphLoader TODO
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNR data structure.
 */
void createTNRFast(
        char *transitNodeSetSize,
        GraphLoader &graphLoader,
        char *outputFilePath) {
    Timer timer("Transit Node Routing preprocessing (fast mode)");
    timer.begin();

    UpdateableGraph *graph = graphLoader.loadUpdateableGraph();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessUsingCH(*graph, outputFilePath, atol(transitNodeSetSize));

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * This function will create the Transit Node Routing data structure based on a given input file.
 * In this case, the preprocessing mode is the 'slow' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param graphLoader TODO
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNR data structure.
 */
void createTNRSlow(
        char *transitNodeSetSize,
        GraphLoader &graphLoader,
        char *outputFilePath) {
    Timer timer("Transit Node Routing preprocessing (slow mode)");
    timer.begin();

    UpdateableGraph *graph = graphLoader.loadUpdateableGraph();
    Graph *originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessUsingCHslower(*graph, *originalGraph, outputFilePath, atol(transitNodeSetSize));

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * This function will create the Transit Node Routing data structure based on a given input file.
 * In this case, the preprocessing mode is the 'dm' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param graphLoader TODO
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNR data structure.
 */
void createTNRUsingDM(
        char *transitNodeSetSize,
        GraphLoader &graphLoader,
        char *outputFilePath) {
    Timer timer("Transit Node Routing preprocessing (using distance matrix)");
    timer.begin();

    UpdateableGraph *graph = graphLoader.loadUpdateableGraph();
    Graph *originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessWithDMvalidation(*graph, *originalGraph, outputFilePath, atol(transitNodeSetSize));

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * An input processing function used in the case when the user wants to precompute using the
 * Transit Node Routing method. This function checks if the mode and the input format arguments
 * are valid, and if so, calls the corresponding function that will handle the preprocessing.
 *
 * @param preprocessingMode[in] Contains the argument determining which preprocessing mode is used (fast, slow or DM)
 * @param transitNodeSetSize[in] Contains the argument determining the desired size of the transit node set.
 * @param graphLoader TODO
 * @param outputFilePath[in] Contains the desired output file path for the precomputed data structure.
 */
void createTNR(
        char *preprocessingMode,
        char *transitNodeSetSize,
        GraphLoader &graphLoader,
        char *outputFilePath) {
    if (strcmp(preprocessingMode, "fast") == 0) {
        createTNRFast(transitNodeSetSize, graphLoader, outputFilePath);
    } else if (strcmp(preprocessingMode, "slow") == 0) {
        createTNRSlow(transitNodeSetSize, graphLoader, outputFilePath);
    } else if (strcmp(preprocessingMode, "dm") == 0) {
        createTNRUsingDM(transitNodeSetSize, graphLoader, outputFilePath);
    } else {
        throw input_error(string("Unknown preprocessing mode '") + preprocessingMode +
                          "' for Transit Node Routing preprocessing.\n" + INVALID_FORMAT_INFO);
    }
}

/**
 * This function will create the Transit Node Routing with Arc Flags data structure based on a given input file.
 * In this case, the input format is XenGraph and the preprocessing mode is the 'slow' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param graphLoader TODO
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNRAF data structure.
 */
void createTNRAFSlow(
        char *transitNodeSetSize,
        GraphLoader &graphLoader,
        char *outputFilePath) {
    Timer timer("Transit Node Routing with Arc Flags preprocessing (slow mode)");
    timer.begin();

    UpdateableGraph *graph = graphLoader.loadUpdateableGraph();
    Graph *originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRAFPreprocessor::preprocessUsingCH(*graph, *originalGraph, outputFilePath, atol(transitNodeSetSize), 32, false);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * This function will create the Transit Node Routing with Arc Flags data structure based on a given input file.
 * In this case, the input format is XenGraph and the preprocessing mode is the 'dm' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param graphLoader TODO
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNRAF data structure.
 */
void createTNRAFUsingDM(
        char *transitNodeSetSize,
        GraphLoader &graphLoader,
        char *outputFilePath) {
    Timer timer("Transit Node Routing with Arc Flags preprocessing (using distance matrix)");
    timer.begin();

    // TODO tahle funkce je buhviproc uplne stejna jako createTNRAFSlow. Uz to tak bylo, tak jenom davam vedet.

    UpdateableGraph *graph = graphLoader.loadUpdateableGraph();
    Graph *originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRAFPreprocessor::preprocessUsingCH(*graph, *originalGraph, outputFilePath, atol(transitNodeSetSize), 32, true);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * An input processing function used in the case when the user wants to precompute using the
 * Transit Node Routing with Arc Flags method. This function checks if the mode and the input format arguments
 * are valid, and if so, calls the corresponding function that will handle the preprocessing.
 *
 * @param preprocessingMode[in] Contains the argument determining which preprocessing mode is used (slow or DM)
 * @param transitNodeSetSize[in] Contains the argument determining the desired size of the transit node set.
 * @param graphLoader TODO
 * @param outputFilePath[in] Contains the desired output file path for the precomputed data structure.
 */
void createTNRAF(
        char *preprocessingMode,
        char *transitNodeSetSize,
        GraphLoader &graphLoader,
        char *outputFilePath) {
    if (strcmp(preprocessingMode, "slow") == 0) {
        createTNRAFSlow(transitNodeSetSize, graphLoader, outputFilePath);
    } else if (strcmp(preprocessingMode, "dm") == 0) {
        createTNRAFUsingDM(transitNodeSetSize, graphLoader, outputFilePath);
    } else {
        throw input_error(string("Unknown preprocessing mode '") + preprocessingMode +
                          "' for Transit Node Routing with Arc Flags preprocessing.\n" + INVALID_FORMAT_INFO);
    }
}

void createDM(
        char *outputType,
        char *preprocessingMode,
        GraphLoader &graphLoader,
        char *outputFilePath
) {
    Timer timer("Distance Matrix preprocessing");
    timer.begin();

    DistanceMatrix *dm;

    if (strcmp(preprocessingMode, "slow") == 0) {
        Graph *graph = graphLoader.loadGraph();
        DistanceMatrixComputor computor;
        computor.computeDistanceMatrix(*graph);
        dm = computor.getDistanceMatrixInstance();
        delete graph;
    } else if (strcmp(preprocessingMode, "fast") == 0) {
        vector<int> adj = graphLoader.loadAdjacencyMatrix();
        vector<int> output(adj.size());
        johnson::graph_t * gr = johnson::johnson_init(adj);
        johnson::johnson_parallel(gr, output.data());

        dm = new DistanceMatrix(std::move(output));
    } else {
        throw input_error(string("Unknown preprocessing mode '") + preprocessingMode +
                          "' for Distance Matrix preprocessing.\n" + INVALID_FORMAT_INFO);
    }

    if (strcmp(outputType, "xdm") == 0) {
        dm->outputToXdm(outputFilePath);
    } else if (strcmp(outputType, "csv") == 0) {
        dm->outputToCsv(outputFilePath);
    } else {
        throw input_error(string("Unknown output type '") + outputType +
                          "' for Distance Matrix preprocessing.\n" + INVALID_FORMAT_INFO);
    }

    timer.finish();
    timer.printMeasuredTime();

    delete dm;
}

/**
 * Benchmarks the basic Dijkstra's algorithm implementation using a given graph in the XenGraph input format and a given
 * set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
 * needed for one query in milliseconds. Additionally, the caller can specify an optional output file path,
 * where all the computed distances will be output. Those distances can then be for example compared with distances
 * computed by some other method to ensure correctness.
 *
 * @param inputFilePath[in] Path to the graph file that will be used for the benchmark. The input file must be in the
 * XenGraph input format.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
 * to use them for example for verification purposes.
 * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
 * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
 */
void benchmarkDijkstra(
        char *inputFilePath,
        char *queriesFilePath,
        char *distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector<pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    XenGraphLoader dijkstraGraphLoader = XenGraphLoader(inputFilePath);
    Graph *dijkstraGraph = dijkstraGraphLoader.loadGraph();

    vector<unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::benchmark(trips, *dijkstraGraph, dijkstraDistances);

    delete dijkstraGraph;

    printf("Run %lu queries using Dijkstra's algorithm in %f seconds.\n"
           "That means %f ms per query.\n", trips.size(), dijkstraTime, (dijkstraTime / trips.size()) * 1000);

    if (outputDistances) {
        printf("Now outputting distances to '%s'.\n", distancesOutputPath);

        ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << endl;
        for (unsigned int i = 0; i < trips.size(); ++i) {
            output << dijkstraDistances[i] << endl;
        }

        output.close();
    }
}

/**
 * Benchmarks the basic Dijkstra's algorithm implementation using a given graph in the XenGraph input format and a given
 * set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
 * needed for one query in milliseconds. Additionally, the caller can specify an optional output file path,
 * where all the computed distances will be output. Those distances can then be for example compared with distances
 * computed by some other method to ensure correctness.
 *
 * @param inputFilePath[in] Path to the graph file that will be used for the benchmark. The input file must be in the
 * XenGraph input format.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
 * used internally in the data structure and the query algorithm.
 * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
 * to use them for example for verification purposes.
 * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
 * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
 */
void benchmarkDijkstraWithMapping(
        char *inputFilePath,
        char *queriesFilePath,
        char *mappingFilePath,
        char *distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector<pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    XenGraphLoader dijkstraGraphLoader = XenGraphLoader(inputFilePath);
    Graph *dijkstraGraph = dijkstraGraphLoader.loadGraph();

    vector<unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::benchmarkUsingMapping(trips, *dijkstraGraph, dijkstraDistances,
                                                                   mappingFilePath);

    delete dijkstraGraph;

    printf("Run %lu queries using Dijkstra's algorithm in %f seconds\n"
           "using '%s' as mapping."
           "That means %f ms per query.\n", trips.size(), dijkstraTime, mappingFilePath,
           (dijkstraTime / trips.size()) * 1000);

    if (outputDistances) {
        printf("Now outputting distances to '%s'.\n", distancesOutputPath);

        ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << endl;
        for (unsigned int i = 0; i < trips.size(); ++i) {
            output << dijkstraDistances[i] << endl;
        }

        output.close();
    }
}

/**
 * Benchmarks the Contraction Hierarchies query algorithm using a given precomputed data structure and a given
 * set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
 * needed for one query in milliseconds. Additionally, the caller can specify an optional output file path,
 * where all the computed distances will be output. Those distances can then be for example compared with distances
 * computed by some other method to ensure correctness.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed Contraction Hierarchies data structure.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
 * to use them for example for verification purposes.
 * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
 * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
 */
void benchmarkCH(
        char *inputFilePath,
        char *queriesFilePath,
        char *distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector<pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader(inputFilePath);
    FlagsGraph *ch = chLoader.loadFlagsGraph();

    vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::benchmark(trips, *ch, chDistances);

    delete ch;

    printf("Run %lu queries using Contraction Hierarchies query algorithm in %f seconds.\n"
           "That means %f ms per query.\n", trips.size(), chTime, (chTime / trips.size()) * 1000);

    if (outputDistances) {
        printf("Now outputting distances to '%s'.\n", distancesOutputPath);

        ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << endl;
        for (unsigned int i = 0; i < trips.size(); ++i) {
            output << chDistances[i] << endl;
        }

        output.close();
    }
}

/**
 * Benchmarks the Contraction Hierarchies query algorithm using a given precomputed data structure, a given
 * set of queries and a given mapping. Prints out the sum of the time required by all the queries in seconds
 * and the average time needed for one query in milliseconds. Additionally, the caller can specify an optional
 * output file path, where all the computed distances will be output. Those distances can then be for example
 * compared with distances computed by some other method to ensure correctness.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed Contraction Hierarchies data structure.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
 * used internally in the data structure and the query algorithm.
 * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
 * to use them for example for verification purposes.
 * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
 * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
 */
void benchmarkCHwithMapping(
        char *inputFilePath,
        char *queriesFilePath,
        char *mappingFilePath,
        char *distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector<pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    DDSGLoader chLoader = DDSGLoader(inputFilePath);
    FlagsGraph *ch = chLoader.loadFlagsGraph();

    vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::benchmarkUsingMapping(trips, *ch, chDistances, mappingFilePath);

    delete ch;

    printf("Run %lu queries using Contraction Hierarchies query algorithm in %f seconds\n"
           "using '%s' as mapping."
           "That means %f ms per query.\n", trips.size(), chTime, mappingFilePath, (chTime / trips.size()) * 1000);

    if (outputDistances) {
        printf("Now outputting distances to '%s'.\n", distancesOutputPath);

        ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << endl;
        for (unsigned int i = 0; i < trips.size(); ++i) {
            output << chDistances[i] << endl;
        }

        output.close();
    }
}

/**
 * Benchmarks the Transit Node Routing query algorithm using a given precomputed data structure and a given
 * set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
 * needed for one query in milliseconds. Additionally, the caller can specify an optional output file path,
 * where all the computed distances will be output. Those distances can then be for example compared with distances
 * computed by some other method to ensure correctness.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed Transit Node Routing data structure.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
 * to use them for example for verification purposes.
 * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
 * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
 */
void benchmarkTNR(
        char *inputFilePath,
        char *queriesFilePath,
        char *distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector<pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    TNRGLoader tnrLoader = TNRGLoader(inputFilePath);
    TransitNodeRoutingGraph *tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::benchmark(trips, *tnrGraph, tnrDistances);

    delete tnrGraph;

    printf("Run %lu queries using Transit Node Routing query algorithm in %f seconds.\n"
           "That means %f ms per query.\n", trips.size(), tnrTime, (tnrTime / trips.size()) * 1000);

    if (outputDistances) {
        printf("Now outputting distances to '%s'.\n", distancesOutputPath);

        ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << endl;
        for (unsigned int i = 0; i < trips.size(); ++i) {
            output << tnrDistances[i] << endl;
        }

        output.close();
    }
}

/**
 * Benchmarks the Transit Node Routing query algorithm using a given precomputed data structure, a given set of queries
 * and a given mapping. Prints out the sum of the time required by all the queries in seconds and the average time
 * needed for one query in milliseconds. Additionally, the caller can specify an optional output file path,
 * where all the computed distances will be output. Those distances can then be for example compared with distances
 * computed by some other method to ensure correctness.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed Transit Node Routing data structure.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
 * used internally in the data structure and the query algorithm.
 * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
 * to use them for example for verification purposes.
 * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
 * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
 */
void benchmarkTNRwithMapping(
        char *inputFilePath,
        char *queriesFilePath,
        char *mappingFilePath,
        char *distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector<pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    TNRGLoader tnrLoader = TNRGLoader(inputFilePath);
    TransitNodeRoutingGraph *tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::benchmarkWithMapping(trips, *tnrGraph, tnrDistances, mappingFilePath);

    delete tnrGraph;

    printf("Run %lu queries using Transit Node Routing query algorithm in %f seconds.\n"
           "That means %f ms per query.\n", trips.size(), tnrTime, (tnrTime / trips.size()) * 1000);

    if (outputDistances) {
        printf("Now outputting distances to '%s'.\n", distancesOutputPath);

        ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << endl;
        for (unsigned int i = 0; i < trips.size(); ++i) {
            output << tnrDistances[i] << endl;
        }

        output.close();
    }
}

/**
 * Benchmarks the Transit Node Routing with Arc Flags query algorithm using a given precomputed data structure and
 * a given set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
 * needed for one query in milliseconds. Additionally, the caller can specify an optional output file path,
 * where all the computed distances will be output. Those distances can then be for example compared with distances
 * computed by some other method to ensure correctness.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed Transit Node Routing with Arc Flags data
 * structure.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
 * to use them for example for verification purposes.
 * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
 * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
 */
void benchmarkTNRAF(
        char *inputFilePath,
        char *queriesFilePath,
        char *distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector<pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    TGAFLoader tnrafLoader = TGAFLoader(inputFilePath);
    TransitNodeRoutingArcFlagsGraph *tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    vector<unsigned int> tnrafDistances(trips.size());
    double tnrafTime = TNRAFBenchmark::benchmark(trips, *tnrafGraph, tnrafDistances);

    delete tnrafGraph;

    printf("Run %lu queries using Transit Node Routing query algorithm in %f seconds.\n"
           "That means %f ms per query.\n", trips.size(), tnrafTime, (tnrafTime / trips.size()) * 1000);

    if (outputDistances) {
        printf("Now outputting distances to '%s'.\n", distancesOutputPath);

        ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << endl;
        for (unsigned int i = 0; i < trips.size(); ++i) {
            output << tnrafDistances[i] << endl;
        }

        output.close();
    }
}

/**
 * Benchmarks the Transit Node Routing with Arc Flags query algorithm using a given precomputed data structure,
 * a given set of queries and a given mapping. Prints out the sum of the time required by all the queries in seconds
 * and the average time needed for one query in milliseconds. Additionally, the caller can specify an optional
 * output file path, where all the computed distances will be output. Those distances can then be for example
 * compared with distances computed by some other method to ensure correctness.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed Transit Node Routing with Arc Flags data
 * structure.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
 * used internally in the data structure and the query algorithm.
 * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
 * to use them for example for verification purposes.
 * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
 * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
 */
void benchmarkTNRAFwithMapping(
        char *inputFilePath,
        char *queriesFilePath,
        char *mappingFilePath,
        char *distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector<pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    TGAFLoader tnrafLoader = TGAFLoader(inputFilePath);
    TransitNodeRoutingArcFlagsGraph *tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    vector<unsigned int> tnrafDistances(trips.size());
    double tnrafTime = TNRAFBenchmark::benchmarkWithMapping(trips, *tnrafGraph, tnrafDistances, mappingFilePath);

    delete tnrafGraph;

    printf("Run %lu queries using Transit Node Routing query algorithm in %f seconds.\n"
           "That means %f ms per query.\n", trips.size(), tnrafTime, (tnrafTime / trips.size()) * 1000);

    if (outputDistances) {
        printf("Now outputting distances to '%s'.\n", distancesOutputPath);

        ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << endl;
        for (unsigned int i = 0; i < trips.size(); ++i) {
            output << tnrafDistances[i] << endl;
        }

        output.close();
    }
}

/**
 * TODO
 */
GraphLoader *newGraphLoader(char *inputType, char *inputFilePath) {
    if (strcmp(inputType, "xengraph") == 0) {
        return new XenGraphLoader(inputFilePath);
    } else if (strcmp(inputType, "dimacs") == 0) {
        return new DIMACSLoader(inputFilePath);
    } else if (strcmp(inputType, "csv") == 0) {
        return new CsvGraphLoader(inputFilePath);
    } else {
        throw input_error(string("Unknown input format '") + inputType + "'.\n" + INVALID_FORMAT_INFO);
    }
}

/**
 * @mainpage Shortest Paths computation library
 *
 * This project is library that allows fast distance queries on directed weighted graphs by preprocessing those graphs
 * to speed up the queries. This means that given any directed weighted graph (can be a road network for example),
 * this library will spend some time precomputing auxiliary structures, that will then allow the queries to be answered
 * much quicker than using the Dijkstra's Algorithm. The memory required by the auxiliary structures is much lower than
 * in the case of full Distance Matrix, so this library should be used in cases where having the full distance matrix is
 * not possible (usually because of insufficient memory), but Dijkstra's Algorithm is too slow.
 *
 * There are three methods implemented in this library - Contraction Hierarchies, Transit Node Routing and Transit Node
 * Routing with Arc Flags. Those methods are listed from the slowest but least memory requiring one to the fastest but
 * most memory requiring one.
 *
 * # Useful information
 *
 * For more information about how to use this library, how to compile it, available command line arguments, info on
 * how to integrate this library with existing projects and so on, please refer to the README.md file in the
 * root directory of this project.
 *
 * <br> _Author: Michal Cvach_
 */
int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);

    if (argc < 5 || argc > 8) {
        printUsageInfo(argv[0]);
        return 0;
    }

    try {
        // The user wants to create data structures using some method.
        if (strcmp(argv[1], "create") == 0) {
            // Contraction Hierarchies preprocessing.
            if (strcmp(argv[2], "ch") == 0) {
                if (argc != 6) {
                    throw input_error(
                            string("Invalid amount of arguments for Contraction Hierarchies preprocessing.\n") +
                            INVALID_FORMAT_INFO);
                }
                GraphLoader *graphLoader = newGraphLoader(argv[3], argv[4]);
                createCH(graphLoader, argv[5]);
                delete graphLoader;

                // Transit Node Routing preprocessing.
            } else if (strcmp(argv[2], "tnr") == 0) {
                if (argc != 8) {
                    throw input_error(
                            string("Invalid amount of arguments for Transit Node Routing preprocessing.\n") +
                            INVALID_FORMAT_INFO);
                }
                GraphLoader *graphLoader = newGraphLoader(argv[3], argv[6]);
                createTNR(argv[4], argv[5], *graphLoader, argv[7]);
                delete graphLoader;

                // Transit Node Routing with Arc Flags preprocessing.
            } else if (strcmp(argv[2], "tnraf") == 0) {
                if (argc != 8) {
                    throw input_error(
                            string("Invalid amount of arguments for Transit Node Routing with Arc Flags preprocessing.\n") +
                            INVALID_FORMAT_INFO);
                }
                GraphLoader *graphLoader = newGraphLoader(argv[3], argv[6]);
                createTNRAF(argv[4], argv[5], *graphLoader, argv[7]);
                delete graphLoader;
            } else if (strcmp(argv[2], "dm") == 0) {
                if (argc != 8) {
                    throw input_error(string("Invalid amount of arguments for Distance Matrix preprocessing.\n") +
                                      INVALID_FORMAT_INFO);
                }

                GraphLoader *graphLoader = newGraphLoader(argv[3], argv[6]);
                createDM(argv[4], argv[5], *graphLoader, argv[7]);
                delete graphLoader;
            } else {
                throw input_error(INVALID_USAGE_INFO);
            }


            // The user has data structures computed using some method and wants to benchmark those using a set of queries.
        } else if (strcmp(argv[1], "benchmark") == 0) {
            const auto benchmarks = {
                    make_tuple(string("dijkstra"), benchmarkDijkstra, benchmarkDijkstraWithMapping),
                    make_tuple(string("ch"), benchmarkCH, benchmarkCHwithMapping),
                    make_tuple(string("tnr"), benchmarkTNR, benchmarkTNRwithMapping),
                    make_tuple(string("tnraf"), benchmarkTNRAF, benchmarkTNRAFwithMapping),
            };

            bool found = false;
            for (auto &benchmark : benchmarks) {
                auto &name = get<0>(benchmark);

                if (name.compare(argv[2])) {
                    if (strcmp(argv[3], "nomapping") == 0) {
                        auto &func = get<1>(benchmark);
                        if (argc == 7) {
                            func(argv[4], argv[5], argv[6], true);
                        } else if (argc == 6) {
                            func(argv[4], argv[5], nullptr, false);
                        } else {
                            throw input_error(
                                    string("Invalid amount of arguments for") + name + " benchmarking.\n" +
                                    INVALID_FORMAT_INFO);
                        }
                    } else if (strcmp(argv[3], "mapping") == 0) {
                        auto &func = get<2>(benchmark);
                        if (argc == 8) {
                            func(argv[4], argv[5], argv[6], argv[7], true);
                        } else if (argc == 7) {
                            func(argv[4], argv[5], argv[6], nullptr, false);
                        } else {
                            throw input_error(
                                    string("Invalid amount of arguments for") + name + " benchmarking.\n" +
                                    INVALID_FORMAT_INFO);
                        }
                    } else {
                        throw input_error(string("Option '") + argv[3] + "' does not make sense for benchmarking.\n" +
                                          "Either 'mapping' or 'nomapping' is expected.\n" + INVALID_FORMAT_INFO);
                    }

                    found = true;
                    break;
                }
            }

            if (!found) {
                throw input_error(INVALID_USAGE_INFO);
            }
        } else {
            throw input_error(INVALID_USAGE_INFO);
        }
    }
    catch (input_error &e) {
        cout << "Input Error for";
        for (int i = 0; i < argc; ++i)
            cout << " " << argv[i];
        cout << ": " << e.what();
        return 1;
    }
    catch (not_implemented_error &e) {
        cout << "Not Implemented Error for";
        for (int i = 0; i < argc; ++i)
            cout << " " << argv[i];
        cout << ": " << e.what();
        return 1;
    }

    return 0;
}
