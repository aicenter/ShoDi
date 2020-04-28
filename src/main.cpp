//
// Author: Xenty (Michal Cvach)
// Created on: 01.08.19
//

#include <fstream>
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

using namespace std;

/**
 * Prints info about how to use the application to the user.
 *
 * @param[in] appName
 */
void printUsageInfo(char * appName) {
    printf("Hello! This application allows the user to preprocess graphs to create data structures\n"
           "for Contraction Hierarchies, Transit Node Routing and Transit Node Routing with Arc Flags.\n"
           "Additionally, the user can also benchmark the obtained data structures using a given set of queries.\n"
           "Please, see 'README.md' for a complete overview of use cases for this application.\n"
           "\n"
           "Some common examples of usage:\n"
           "  '%s create tnraf xengraph dm 1000 input_graph.xeng output_file'\n"
           "  '%s create ch xengraph input_graph.xeng output_file'\n", appName, appName);
}

/**
 * Prints a simple error message in case the user provides some invalid input.
 */
void printInvalidUsageInfo() {
    printf("It seems that the given arguments do not match any use case for this application.\n"
           "Please, see 'README.md' for a complete overview of use cases for this application\n"
           "with examples.\n");
}

/**
 * This function will create the Contraction Hierarchies data structure based on a given input file.
 * In this case, the input format is XenGraph.
 *
 * @param inputFilePath[in] Contains the file path to the input graph in the XenGraph format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed CH data structure.
 */
void createCHfromXenGraph(char * inputFilePath, char * outputFilePath) {
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

/**
 * This function will create the Contraction Hierarchies data structure based on a given input file.
 * In this case, the input format is DIMACS.
 *
 * @param inputFilePath[in] Contains the file path to the input graph in the DIMACS format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed CH data structure.
 */
void createCHfromDIMACS(char * inputFilePath, char * outputFilePath) {
    Timer timer("Contraction Hierarchies from DIMACS preprocessing");
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


/**
 * An input processing function used in the case when the user wants to precompute using the
 * Contraction Hierarchies method. This function checks if the input format argument is valid,
 * and if it is, then calls the corresponding function that will handle the preprocessing.
 *
 * @param inputType[in] Contains the argument determining which input format is used (either XenGraph or DIMACS)
 * @param inputFilePath[in] Contains the file path to the input graph in the chosen format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed data structure.
 */
void createCH(char * inputType, char * inputFilePath, char * outputFilePath) {
    if(strcmp(inputType, "xengraph") == 0) {
        createCHfromXenGraph(inputFilePath, outputFilePath);
    } else if(strcmp(inputType, "dimacs") == 0) {
        createCHfromDIMACS(inputFilePath, outputFilePath);
    } else {
        printf("Unknown input format '%s' for Contraction Hierarchies preprocessing.\n"
               "Please, make sure that your call has the right format. If not sure,\n"
               "refer to 'README.md' for a complete overview of use cases for this application\n"
               "with examples.\n", inputType);
    }
}

/**
 * This function will create the Transit Node Routing data structure based on a given input file.
 * In this case, the input format is XenGraph and the preprocessing mode is the 'fast' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param inputFilePath[in] Contains the file path to the input graph in the XenGraph format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNR data structure.
 */
void createTNRfromXenGraphFast(char * transitNodeSetSize, char * inputFilePath, char * outputFilePath) {
    Timer timer("Transit Node Routing from XenGraph preprocessing (fast mode)");
    timer.begin();

    XenGraphLoader graphLoader = XenGraphLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessUsingCH(*graph, outputFilePath, atol(transitNodeSetSize));

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * This function will create the Transit Node Routing data structure based on a given input file.
 * In this case, the input format is XenGraph and the preprocessing mode is the 'slow' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param inputFilePath[in] Contains the file path to the input graph in the XenGraph format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNR data structure.
 */
void createTNRfromXenGraphSlow(char * transitNodeSetSize, char * inputFilePath, char * outputFilePath) {
    Timer timer("Transit Node Routing from XenGraph preprocessing (slow mode)");
    timer.begin();

    XenGraphLoader graphLoader = XenGraphLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessUsingCHslower(*graph, *originalGraph, outputFilePath, atol(transitNodeSetSize));

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * This function will create the Transit Node Routing data structure based on a given input file.
 * In this case, the input format is XenGraph and the preprocessing mode is the 'dm' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param inputFilePath[in] Contains the file path to the input graph in the XenGraph format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNR data structure.
 */
void createTNRfromXenGraphUsingDM(char * transitNodeSetSize, char * inputFilePath, char * outputFilePath) {
    Timer timer("Transit Node Routing from XenGraph preprocessing (using distance matrix)");
    timer.begin();

    XenGraphLoader graphLoader = XenGraphLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessWithDMvalidation(*graph, *originalGraph, outputFilePath, atol(transitNodeSetSize));

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * This function will create the Transit Node Routing data structure based on a given input file.
 * In this case, the input format is DIMACS and the preprocessing mode is the 'fast' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param inputFilePath[in] Contains the file path to the input graph in the DIMACS format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNR data structure.
 */
void createTNRfromDIMACSFast(char * transitNodeSetSize, char * inputFilePath, char * outputFilePath) {
    Timer timer("Transit Node Routing from DIMACS preprocessing (fast mode)");
    timer.begin();

    DIMACSLoader graphLoader = DIMACSLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessUsingCH(*graph, outputFilePath, atol(transitNodeSetSize));

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * This function will create the Transit Node Routing data structure based on a given input file.
 * In this case, the input format is DIMACS and the preprocessing mode is the 'slow' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param inputFilePath[in] Contains the file path to the input graph in the DIMACS format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNR data structure.
 */
void createTNRfromDIMACSSlow(char * transitNodeSetSize, char * inputFilePath, char * outputFilePath) {
    Timer timer("Transit Node Routing from DIMACS preprocessing (slow mode)");
    timer.begin();

    DIMACSLoader graphLoader = DIMACSLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRPreprocessor::preprocessUsingCHslower(*graph, *originalGraph, outputFilePath, atol(transitNodeSetSize));

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * This function will create the Transit Node Routing data structure based on a given input file.
 * In this case, the input format is DIMACS and the preprocessing mode is the 'dm' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param inputFilePath[in] Contains the file path to the input graph in the DIMACS format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNR data structure.
 */
void createTNRfromDIMACSUsingDM(char * transitNodeSetSize, char * inputFilePath, char * outputFilePath) {
    Timer timer("Transit Node Routing from DIMACS preprocessing (using distance matrix)");
    timer.begin();

    DIMACSLoader graphLoader = DIMACSLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
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
 * @param inputType[in] Contains the argument determining which input format is used (either XenGraph or DIMACS)
 * @param preprocessingMode[in] Contains the argument determining which preprocessing mode is used (fast, slow or DM)
 * @param transitNodeSetSize[in] Contains the argument determining the desired size of the transit node set.
 * @param inputFilePath[in] Contains the file path to the input graph in the chosen format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed data structure.
 */
void createTNR(char * inputType, char * preprocessingMode, char * transitNodeSetSize, char * inputFilePath, char * outputFilePath) {
    if(strcmp(inputType, "xengraph") == 0) {
        if(strcmp(preprocessingMode, "fast") == 0) {
            createTNRfromXenGraphFast(transitNodeSetSize, inputFilePath, outputFilePath);
        } else if(strcmp(preprocessingMode, "slow") == 0) {
            createTNRfromXenGraphSlow(transitNodeSetSize, inputFilePath, outputFilePath);
        } else if(strcmp(preprocessingMode, "dm") == 0) {
            createTNRfromXenGraphUsingDM(transitNodeSetSize, inputFilePath, outputFilePath);
        } else {
            printf("Unknown preprocessing mode '%s' for Transit Node Routing preprocessing.\n"
                   "Please, make sure that your call has the right format. If not sure,\n"
                   "refer to 'README.md' for a complete overview of use cases for this application\n"
                   "with examples.\n", preprocessingMode);
        }
    } else if(strcmp(inputType, "dimacs") == 0) {
        if(strcmp(preprocessingMode, "fast") == 0) {
            createTNRfromDIMACSFast(transitNodeSetSize, inputFilePath, outputFilePath);
        } else if(strcmp(preprocessingMode, "slow") == 0) {
            createTNRfromDIMACSSlow(transitNodeSetSize, inputFilePath, outputFilePath);
        } else if(strcmp(preprocessingMode, "dm") == 0) {
            createTNRfromDIMACSUsingDM(transitNodeSetSize, inputFilePath, outputFilePath);
        } else {
            printf("Unknown preprocessing mode '%s' for Transit Node Routing preprocessing.\n"
                   "Please, make sure that your call has the right format. If not sure,\n"
                   "refer to 'README.md' for a complete overview of use cases for this application\n"
                   "with examples.\n", preprocessingMode);
        }
    } else {
        printf("Unknown input format '%s' for Transit Node Routing preprocessing.\n"
               "Please, make sure that your call has the right format. If not sure,\n"
               "refer to 'README.md' for a complete overview of use cases for this application\n"
               "with examples.\n", inputType);
    }
}

/**
 * This function will create the Transit Node Routing with Arc Flags data structure based on a given input file.
 * In this case, the input format is XenGraph and the preprocessing mode is the 'slow' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param inputFilePath[in] Contains the file path to the input graph in the XenGraph format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNRAF data structure.
 */
void createTNRAFfromXenGraphSlow(char * transitNodeSetSize, char * inputFilePath, char * outputFilePath) {
    Timer timer("Transit Node Routing with Arc Flags from XenGraph preprocessing (slow mode)");
    timer.begin();

    XenGraphLoader graphLoader = XenGraphLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
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
 * @param inputFilePath[in] Contains the file path to the input graph in the XenGraph format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNRAF data structure.
 */
void createTNRAFfromXenGraphUsingDM(char * transitNodeSetSize, char * inputFilePath, char * outputFilePath) {
    Timer timer("Transit Node Routing with Arc Flags from XenGraph preprocessing (using distance matrix)");
    timer.begin();

    XenGraphLoader graphLoader = XenGraphLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRAFPreprocessor::preprocessUsingCH(*graph, *originalGraph, outputFilePath, atol(transitNodeSetSize), 32, true);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * This function will create the Transit Node Routing with Arc Flags data structure based on a given input file.
 * In this case, the input format is DIMACS and the preprocessing mode is the 'slow' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param inputFilePath[in] Contains the file path to the input graph in the DIMACS format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNRAF data structure.
 */
void createTNRAFfromDIMACSSlow(char * transitNodeSetSize, char * inputFilePath, char * outputFilePath) {
    Timer timer("Transit Node Routing with Arc Flags from DIMACS preprocessing (slow mode)");
    timer.begin();

    DIMACSLoader graphLoader = DIMACSLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);

    TNRAFPreprocessor::preprocessUsingCH(*graph, *originalGraph, outputFilePath, atol(transitNodeSetSize), 32, false);

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}

/**
 * This function will create the Transit Node Routing with Arc Flags data structure based on a given input file.
 * In this case, the input format is DIMACS and the preprocessing mode is the 'dm' mode.
 *
 * @param transitNodeSetSize[in] The desired size of the transit nodes set.
 * @param inputFilePath[in] Contains the file path to the input graph in the DIMACS format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed TNRAF data structure.
 */
void createTNRAFfromDIMACSUsingDM(char * transitNodeSetSize, char * inputFilePath, char * outputFilePath) {
    Timer timer("Transit Node Routing with Arc Flags from DIMACS preprocessing (using distance matrix)");
    timer.begin();

    DIMACSLoader graphLoader = DIMACSLoader(inputFilePath);
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    Graph * originalGraph = graph->createCopy();
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
 * @param inputType[in] Contains the argument determining which input format is used (either XenGraph or DIMACS)
 * @param preprocessingMode[in] Contains the argument determining which preprocessing mode is used (slow or DM)
 * @param transitNodeSetSize[in] Contains the argument determining the desired size of the transit node set.
 * @param inputFilePath[in] Contains the file path to the input graph in the chosen format.
 * @param outputFilePath[in] Contains the desired output file path for the precomputed data structure.
 */
void createTNRAF(char * inputType, char * preprocessingMode, char * transitNodeSetSize, char * inputFilePath, char * outputFilePath) {
    if(strcmp(inputType, "xengraph") == 0) {
        if(strcmp(preprocessingMode, "slow") == 0) {
            createTNRAFfromXenGraphSlow(transitNodeSetSize, inputFilePath, outputFilePath);
        } else if(strcmp(preprocessingMode, "dm") == 0) {
            createTNRAFfromXenGraphUsingDM(transitNodeSetSize, inputFilePath, outputFilePath);
        } else {
            printf("Unknown preprocessing mode '%s' for Transit Node Routing with Arc Flags preprocessing.\n"
                   "Please, make sure that your call has the right format. If not sure,\n"
                   "refer to 'README.md' for a complete overview of use cases for this application\n"
                   "with examples.\n", preprocessingMode);
        }
    } else if(strcmp(inputType, "dimacs") == 0) {
        if(strcmp(preprocessingMode, "slow") == 0) {
            createTNRAFfromDIMACSSlow(transitNodeSetSize, inputFilePath, outputFilePath);
        } else if(strcmp(preprocessingMode, "dm") == 0) {
            createTNRAFfromDIMACSUsingDM(transitNodeSetSize, inputFilePath, outputFilePath);
        } else {
            printf("Unknown preprocessing mode '%s' for Transit Node Routing with Arc Flags preprocessing.\n"
                   "Please, make sure that your call has the right format. If not sure,\n"
                   "refer to 'README.md' for a complete overview of use cases for this application\n"
                   "with examples.\n", preprocessingMode);
        }
    } else {
        printf("Unknown input format '%s' for Transit Node Routing with Arc Flags preprocessing.\n"
               "Please, make sure that your call has the right format. If not sure,\n"
               "refer to 'README.md' for a complete overview of use cases for this application\n"
               "with examples.\n", inputType);
    }
}

/**
 * Benchmarks the basic Dijkstra's algorithm implementation using a given graph in the XenGraph input format and a given
 * set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
 * needed for one query in milliseconds.
 *
 * @param inputFilePath[in] Path to the graph file that will be used for the benchmark. The input file must be in the
 * XenGraph input format.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 */
void benchmarkDijkstra(char * inputFilePath, char * queriesFilePath) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    XenGraphLoader dijkstraGraphLoader = XenGraphLoader(inputFilePath);
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    vector<unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::benchmark(trips, *dijkstraGraph, dijkstraDistances);

    delete dijkstraGraph;

    printf("Run %lu queries using Dijkstra's algorithm in %f seconds.\n"
           "That means %f ms per query.\n", trips.size(), dijkstraTime, (dijkstraTime / trips.size()) * 1000);
}

/**
 * Benchmarks the basic Dijkstra's algorithm implementation using a given graph in the XenGraph input format and a given
 * set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
 * needed for one query in milliseconds.
 *
 * @param inputFilePath[in] Path to the graph file that will be used for the benchmark. The input file must be in the
 * XenGraph input format.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
 * used internally in the data structure and the query algorithm.
 */
void benchmarkDijkstraWithMapping(char * inputFilePath, char * queriesFilePath, char * mappingFilePath) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector< pair < long long unsigned int, long long unsigned int > > trips;
    tripsLoader.loadLongLongTrips(trips);

    XenGraphLoader dijkstraGraphLoader = XenGraphLoader(inputFilePath);
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    vector<unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::benchmarkUsingMapping(trips, *dijkstraGraph, dijkstraDistances, mappingFilePath);

    delete dijkstraGraph;

    printf("Run %lu queries using Dijkstra's algorithm in %f seconds\n"
           "using '%s' as mapping."
           "That means %f ms per query.\n", trips.size(), dijkstraTime, mappingFilePath, (dijkstraTime / trips.size()) * 1000);
}

/**
 * Benchmarks the Contraction Hierarchies query algorithm using a given precomputed data structure and a given
 * set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
 * needed for one query in milliseconds.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed Contraction Hierarchies data structure.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 */
void benchmarkCH(char * inputFilePath, char * queriesFilePath) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader(inputFilePath);
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::benchmark(trips, *ch, chDistances);

    delete ch;

    printf("Run %lu queries using Contraction Hierarchies query algorithm in %f seconds.\n"
           "That means %f ms per query.\n", trips.size(), chTime, (chTime / trips.size()) * 1000);
}

/**
 * Benchmarks the Contraction Hierarchies query algorithm using a given precomputed data structure, a given
 * set of queries and a given mapping. Prints out the sum of the time required by all the queries in seconds
 * and the average time needed for one query in milliseconds.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed Contraction Hierarchies data structure.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
 * used internally in the data structure and the query algorithm.
 */
void benchmarkCHwithMapping(char * inputFilePath, char * queriesFilePath, char * mappingFilePath) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector< pair < long long unsigned int, long long unsigned int > > trips;
    tripsLoader.loadLongLongTrips(trips);

    DDSGLoader chLoader = DDSGLoader(inputFilePath);
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::benchmarkUsingMapping(trips, *ch, chDistances, mappingFilePath);

    delete ch;

    printf("Run %lu queries using Contraction Hierarchies query algorithm in %f seconds\n"
           "using '%s' as mapping."
           "That means %f ms per query.\n", trips.size(), chTime, mappingFilePath, (chTime / trips.size()) * 1000);
}

/**
 * Benchmarks the Transit Node Routing query algorithm using a given precomputed data structure and a given
 * set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
 * needed for one query in milliseconds.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed Transit Node Routing data structure.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 */
void benchmarkTNR(char * inputFilePath, char * queriesFilePath) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    TNRGLoader tnrLoader = TNRGLoader(inputFilePath);
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::benchmark(trips, *tnrGraph, tnrDistances);

    delete tnrGraph;

    printf("Run %lu queries using Transit Node Routing query algorithm in %f seconds.\n"
           "That means %f ms per query.\n", trips.size(), tnrTime, (tnrTime / trips.size()) * 1000);
}

/**
 * Benchmarks the Transit Node Routing query algorithm using a given precomputed data structure, a given set of queries
 * and a given mapping. Prints out the sum of the time required by all the queries in seconds and the average time
 * needed for one query in milliseconds.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed Transit Node Routing data structure.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
 * used internally in the data structure and the query algorithm.
 */
void benchmarkTNRwithMapping(char * inputFilePath, char * queriesFilePath, char * mappingFilePath) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector< pair < long long unsigned int, long long unsigned int > > trips;
    tripsLoader.loadLongLongTrips(trips);

    TNRGLoader tnrLoader = TNRGLoader(inputFilePath);
    TransitNodeRoutingGraph * tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    vector<unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::benchmarkWithMapping(trips, *tnrGraph, tnrDistances, mappingFilePath);

    delete tnrGraph;

    printf("Run %lu queries using Transit Node Routing query algorithm in %f seconds.\n"
           "That means %f ms per query.\n", trips.size(), tnrTime, (tnrTime / trips.size()) * 1000);
}

/**
 * Benchmarks the Transit Node Routing with Arc Flags query algorithm using a given precomputed data structure and
 * a given set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
 * needed for one query in milliseconds.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed Transit Node Routing with Arc Flags data
 * structure.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 */
void benchmarkTNRAF(char * inputFilePath, char * queriesFilePath) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    TGAFLoader tnrafLoader = TGAFLoader(inputFilePath);
    TransitNodeRoutingArcFlagsGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    vector<unsigned int> tnrafDistances(trips.size());
    double tnrafTime = TNRAFBenchmark::benchmark(trips, *tnrafGraph, tnrafDistances);

    delete tnrafGraph;

    printf("Run %lu queries using Transit Node Routing query algorithm in %f seconds.\n"
           "That means %f ms per query.\n", trips.size(), tnrafTime, (tnrafTime / trips.size()) * 1000);
}

/**
 * Benchmarks the Transit Node Routing with Arc Flags query algorithm using a given precomputed data structure,
 * a given set of queries and a given mapping. Prints out the sum of the time required by all the queries in seconds
 * and the average time needed for one query in milliseconds.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed Transit Node Routing with Arc Flags data
 * structure.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
 * used internally in the data structure and the query algorithm.
 */
void benchmarkTNRAFwithMapping(char * inputFilePath, char * queriesFilePath, char * mappingFilePath) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    vector< pair < long long unsigned int, long long unsigned int > > trips;
    tripsLoader.loadLongLongTrips(trips);

    TGAFLoader tnrafLoader = TGAFLoader(inputFilePath);
    TransitNodeRoutingArcFlagsGraph * tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    vector<unsigned int> tnrafDistances(trips.size());
    double tnrafTime = TNRAFBenchmark::benchmarkWithMapping(trips, *tnrafGraph, tnrafDistances, mappingFilePath);

    delete tnrafGraph;

    printf("Run %lu queries using Transit Node Routing query algorithm in %f seconds.\n"
           "That means %f ms per query.\n", trips.size(), tnrafTime, (tnrafTime / trips.size()) * 1000);
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
int main(int argc, char * argv[]) {
    if(argc < 5 || argc > 8) {
        printUsageInfo(argv[0]);
        return 0;
    }

    // The user wants to create data structures using some method.
    if(strcmp(argv[1], "create") == 0) {
        // Contraction Hierarchies preprocessing.
        if(strcmp(argv[2], "ch") == 0) {
            if(argc != 6) {
                printf("Invalid amount of arguments for Contraction Hierarchies preprocessing.\n"
                       "Please, make sure that your call has the right format. If not sure,\n"
                       "refer to 'README.md' for a complete overview of use cases for this application\n"
                       "with examples.\n");
                return 0;
            }
            createCH(argv[3], argv[4], argv[5]);

        // Transit Node Routing preprocessing.
        } else if(strcmp(argv[2], "tnr") == 0) {
            if(argc != 8) {
                printf("Invalid amount of arguments for Transit Node Routing preprocessing.\n"
                       "Please, make sure that your call has the right format. If not sure,\n"
                       "refer to 'README.md' for a complete overview of use cases for this application\n"
                       "with examples.\n");
                return 0;
            }
            createTNR(argv[3], argv[4], argv[5], argv[6], argv[7]);

        // Transit Node Routing with Arc Flags preprocessing.
        } else if(strcmp(argv[2], "tnraf") == 0) {
            if(argc != 8) {
                printf("Invalid amount of arguments for Transit Node Routing with Arc Flags preprocessing.\n"
                       "Please, make sure that your call has the right format. If not sure,\n"
                       "refer to 'README.md' for a complete overview of use cases for this application\n"
                       "with examples.\n");
                return 0;
            }
            createTNRAF(argv[3], argv[4], argv[5], argv[6], argv[7]);

        } else {
            printInvalidUsageInfo();
            return 0;
        }


    // The user has data structures computed using some method and wants to benchmark those using a set of queries.
    } else if (strcmp(argv[1], "benchmark") == 0) {
        // Dijkstra benchmarking
        if(strcmp(argv[2], "dijkstra") == 0) {
            if(strcmp(argv[3], "nomapping") == 0) {
                if(argc == 6) {
                    benchmarkDijkstra(argv[4], argv[5]);
                } else {
                    printf("Invalid amount of arguments for Dijkstra's Algorithm benchmarking.\n"
                           "Please, make sure that your call has the right format. If not sure,\n"
                           "refer to 'README.md' for a complete overview of use cases for this application\n"
                           "with examples.\n");
                }
            } else if(strcmp(argv[3], "mapping") == 0) {
                if(argc == 7) {
                    benchmarkDijkstraWithMapping(argv[4], argv[5], argv[6]);
                } else {
                    printf("Invalid amount of arguments for Dijkstra's Algorithm  benchmarking.\n"
                           "Please, make sure that your call has the right format. If not sure,\n"
                           "refer to 'README.md' for a complete overview of use cases for this application\n"
                           "with examples.\n");
                }
            } else {
                printf("Option '%s' does not make sense for benchmarking.\n"
                       "Either 'mapping' or 'nomapping' is expected.\n"
                       "Please, make sure that your call has the right format. If not sure,\n"
                       "refer to 'README.md' for a complete overview of use cases for this application\n"
                       "with examples.\n", argv[3]);
            }
        }
        // Contraction Hierarchies benchmarking.
        if(strcmp(argv[2], "ch") == 0) {
            if(strcmp(argv[3], "nomapping") == 0) {
                if(argc == 6) {
                    benchmarkCH(argv[4], argv[5]);
                } else {
                    printf("Invalid amount of arguments for Contraction Hierarchies benchmarking.\n"
                           "Please, make sure that your call has the right format. If not sure,\n"
                           "refer to 'README.md' for a complete overview of use cases for this application\n"
                           "with examples.\n");
                }
            } else if(strcmp(argv[3], "mapping") == 0) {
                if(argc == 7) {
                    benchmarkCHwithMapping(argv[4], argv[5], argv[6]);
                } else {
                    printf("Invalid amount of arguments for Contraction Hierarchies benchmarking.\n"
                           "Please, make sure that your call has the right format. If not sure,\n"
                           "refer to 'README.md' for a complete overview of use cases for this application\n"
                           "with examples.\n");
                }
            } else {
                printf("Option '%s' does not make sense for benchmarking.\n"
                       "Either 'mapping' or 'nomapping' is expected.\n"
                       "Please, make sure that your call has the right format. If not sure,\n"
                       "refer to 'README.md' for a complete overview of use cases for this application\n"
                       "with examples.\n", argv[3]);
            }

        // Transit Node Routing benchmarking.
        } else if(strcmp(argv[2], "tnr") == 0) {
            if(strcmp(argv[3], "nomapping") == 0) {
                if(argc == 6) {
                    benchmarkTNR(argv[4], argv[5]);
                } else {
                    printf("Invalid amount of arguments for Transit Node Routing benchmarking.\n"
                           "Please, make sure that your call has the right format. If not sure,\n"
                           "refer to 'README.md' for a complete overview of use cases for this application\n"
                           "with examples.\n");
                }
            } else if(strcmp(argv[3], "mapping") == 0) {
                if(argc == 7) {
                    benchmarkTNRwithMapping(argv[4], argv[5], argv[6]);
                } else {
                    printf("Invalid amount of arguments for Transit Node Routing benchmarking.\n"
                           "Please, make sure that your call has the right format. If not sure,\n"
                           "refer to 'README.md' for a complete overview of use cases for this application\n"
                           "with examples.\n");
                }
            } else {
                printf("Option '%s' does not make sense for benchmarking.\n"
                       "Either 'mapping' or 'nomapping' is expected.\n"
                       "Please, make sure that your call has the right format. If not sure,\n"
                       "refer to 'README.md' for a complete overview of use cases for this application\n"
                       "with examples.\n", argv[3]);
            }

        // Transit Node Routing with Arc Flags benchmarking.
        } else if(strcmp(argv[2], "tnraf") == 0) {
            if(strcmp(argv[3], "nomapping") == 0) {
                if(argc == 6) {
                    benchmarkTNRAF(argv[4], argv[5]);
                } else {
                    printf("Invalid amount of arguments for Transit Node Routing with Arc Flags benchmarking.\n"
                           "Please, make sure that your call has the right format. If not sure,\n"
                           "refer to 'README.md' for a complete overview of use cases for this application\n"
                           "with examples.\n");
                }
            } else if(strcmp(argv[3], "mapping") == 0) {
                if(argc == 7) {
                    benchmarkTNRAFwithMapping(argv[4], argv[5], argv[6]);
                } else {
                    printf("Invalid amount of arguments for Transit Node Routing with Arc Flags benchmarking.\n"
                           "Please, make sure that your call has the right format. If not sure,\n"
                           "refer to 'README.md' for a complete overview of use cases for this application\n"
                           "with examples.\n");
                }
            } else {
                printf("Option '%s' does not make sense for benchmarking.\n"
                       "Either 'mapping' or 'nomapping' is expected.\n"
                       "Please, make sure that your call has the right format. If not sure,\n"
                       "refer to 'README.md' for a complete overview of use cases for this application\n"
                       "with examples.\n", argv[3]);
            }

        } else {
            printInvalidUsageInfo();
            return 0;
        }
    } else {
        printInvalidUsageInfo();
        return 0;
    }

    return 0;
}
