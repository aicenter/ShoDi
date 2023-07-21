//
// Author: Xenty (Michal Cvach)
// Created on: 01.08.19
//

#include <boost/unordered/detail/implementation.hpp>
#include <fstream>
#include <iostream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <cstring>
#include <memory>
#include <iostream>
#include <boost/numeric/conversion/cast.hpp>
#include <string>
#include <tuple>
//#include "DistanceMatrix/Distance_matrix_travel_time_provider.h"
//#include "GraphBuilding/Loaders/DIMACSLoader.h"
//#include "GraphBuilding/Loaders/GraphLoader.h"
//#include "GraphBuilding/Loaders/TNRGLoader.h"
//#include "GraphBuilding/Loaders/XenGraphLoader.h"
//#include "GraphBuilding/Structures/UpdateableGraph.h"
//#include "Timer/Timer.h"
//#include "CH/CHPreprocessor.h"
//#include "TNR/TNRPreprocessor.h"
//#include "TNRAF/TNRAFPreprocessor.h"
//#include "GraphBuilding/Loaders/TripsLoader.h"
//#include "GraphBuilding/Loaders/DDSGLoader.h"
//#include "Benchmarking/CHBenchmark.h"
//#include "Benchmarking/CorrectnessValidator.h"
//#include "Benchmarking/TNRBenchmark.h"
//#include "Benchmarking/TNRAFBenchmark.h"
//#include "GraphBuilding/Loaders/DistanceMatrixLoader.h"
#include "GraphBuilding/Loaders/TGAFLoader.h"
//#include "Benchmarking/DijkstraBenchmark.h"
//#include "Error/Error.h"
#include "GraphBuilding/Loaders/CsvGraphLoader.h"
//#include "DistanceMatrix/DistanceMatrixComputorSlow.h"
//#include "DistanceMatrix/johnson.hpp"
//#include "DistanceMatrix/DistanceMatrixComputorFast.h"
//#include "DistanceMatrix/DistanceMatrixOutputter.h"
//#include "DistanceMatrix/DistanceMatrixXdmOutputter.h"
//#include "DistanceMatrix/DistanceMatrixCsvOutputter.h"
//
//
//
///**
// * Prints info about how to use the application to the user.
// *
// * @param[in] appName
// */
//void printUsageInfo(
//        char *appName) {
//    printf("Hello! This application allows the user to preprocess graphs to create data structures\n"
//           "for Contraction Hierarchies, Transit Node Routing and Transit Node Routing with Arc Flags.\n"
//           "Additionally, the user can also benchmark the obtained data structures using a given set of queries.\n"
//           "Please, see 'README.md' for a complete overview of use cases for this application.\n"
//           "\n"
//           "Some common examples of usage:\n"
//           "  '%s create tnraf xengraph dm 1000 input_graph.xeng output_file'\n"
//           "  '%s create ch xengraph input_graph.xeng output_file'\n", appName, appName);
//}
//
//constexpr auto INVALID_USAGE_INFO = "It seems that the given arguments do not match any use case for this application.\n"
//                                    "Please, see 'README.md' for a complete overview of use cases for this application\n"
//                                    "with examples.\n";
//
//constexpr auto INVALID_FORMAT_INFO = "Please, make sure that your call has the right format. If not sure,\n"
//                                     "refer to 'README.md' for a complete overview of use cases for this application\n"
//                                     "with examples.\n";
//
///**
// * This function will create the Contraction Hierarchies data structure based on a given input file.
// *
// * @param graphLoader TODO
// * @param outputFilePath[in] Contains the desired output file path for the precomputed CH data structure.
// */
//void createCH(
//        GraphLoader &graphLoader,
//        char *outputFilePath,
//        unsigned int precisionLoss) {
//    Timer timer("Contraction Hierarchies from DIMACS preprocessing");
//
//    UpdateableGraph graph(graphLoader.nodes());
//    graphLoader.loadGraph(graph, precisionLoss);
//
//    timer.begin();
//    CHPreprocessor::preprocessForDDSG(graph);
//    timer.finish();
//
//    graphLoader.loadGraph(graph, precisionLoss);
//    graph.flushInDdsgFormat(outputFilePath);
//
//    timer.printMeasuredTime();
//}
//
///**
// * This function will create the Transit Node Routing data structure based on a given input file.
// * In this case, the preprocessing mode is the 'fast' mode.
// *
// * @param transitNodeSetSize[in] The desired size of the transit nodes set.
// * @param graphLoader TODO
// * @param outputFilePath[in] Contains the desired output file path for the precomputed TNR data structure.
// */
//void createTNRFast(
//        char *transitNodeSetSize,
//        GraphLoader &graphLoader,
//        char *outputFilePath,
//        unsigned int precisionLoss) {
//    Timer timer("Transit Node Routing preprocessing (fast mode)");
//
//    UpdateableGraph graph(graphLoader.nodes());
//    graphLoader.loadGraph(graph, precisionLoss);
//
//    timer.begin();
//    CHPreprocessor::preprocessForDDSG(graph);
//    timer.finish();
//
//    graphLoader.loadGraph(graph, precisionLoss);
//
//    timer.begin();
//    TNRPreprocessor::preprocessUsingCH(graph, outputFilePath, boost::numeric_cast<unsigned int>(atol(transitNodeSetSize)));
//    timer.finish();
//
//    timer.printMeasuredTime();
//}
//
///**
// * This function will create the Transit Node Routing data structure based on a given input file.
// * In this case, the preprocessing mode is the 'slow' mode.
// *
// * @param transitNodeSetSize[in] The desired size of the transit nodes set.
// * @param graphLoader TODO
// * @param outputFilePath[in] Contains the desired output file path for the precomputed TNR data structure.
// */
//void createTNRSlow(
//        char *transitNodeSetSize,
//        GraphLoader &graphLoader,
//        char *outputFilePath,
//        unsigned int precisionLoss) {
//    Timer timer("Transit Node Routing preprocessing (slow mode)");
//
//    UpdateableGraph graph(graphLoader.nodes());
//    graphLoader.loadGraph(graph, precisionLoss);
//    Graph *originalGraph = graph.createCopy();
//
//    timer.begin();
//    CHPreprocessor::preprocessForDDSG(graph);
//    timer.finish();
//
//    graphLoader.loadGraph(graph, precisionLoss);
//
//    timer.begin();
//    TNRPreprocessor::preprocessUsingCHslower(
//        graph, *originalGraph, outputFilePath,
//        boost::numeric_cast<unsigned int>(atol(transitNodeSetSize)));
//    timer.finish();
//
//    delete originalGraph;
//
//    timer.printMeasuredTime();
//}
//
///**
// * This function will create the Transit Node Routing data structure based on a given input file.
// * In this case, the preprocessing mode is the 'dm' mode.
// *
// * @param transitNodeSetSize[in] The desired size of the transit nodes set.
// * @param graphLoader TODO
// * @param outputFilePath[in] Contains the desired output file path for the precomputed TNR data structure.
// */
//void createTNRUsingDM(
//        char *transitNodeSetSize,
//        GraphLoader &graphLoader,
//        char *outputFilePath,
//        unsigned int precisionLoss) {
//    Timer timer("Transit Node Routing preprocessing (using distance matrix)");
//
//    UpdateableGraph graph(graphLoader.nodes());
//    graphLoader.loadGraph(graph, precisionLoss);
//    Graph *originalGraph = graph.createCopy();
//
//    timer.begin();
//    CHPreprocessor::preprocessForDDSG(graph);
//    timer.finish();
//
//    graphLoader.loadGraph(graph, precisionLoss);
//
//    timer.begin();
//    TNRPreprocessor::preprocessWithDMvalidation(
//        graph, *originalGraph, outputFilePath,
//        boost::numeric_cast<unsigned int>(atol(transitNodeSetSize)));
//    timer.finish();
//
//    delete originalGraph;
//
//    timer.printMeasuredTime();
//}
//
///**
// * An input processing function used in the case when the user wants to precompute using the
// * Transit Node Routing method. This function checks if the mode and the input format arguments
// * are valid, and if so, calls the corresponding function that will handle the preprocessing.
// *
// * @param preprocessingMode[in] Contains the argument determining which preprocessing mode is used (fast, slow or DM)
// * @param transitNodeSetSize[in] Contains the argument determining the desired size of the transit node set.
// * @param graphLoader TODO
// * @param outputFilePath[in] Contains the desired output file path for the precomputed data structure.
// */
//void createTNR(
//        char *preprocessingMode,
//        char *transitNodeSetSize,
//        GraphLoader &graphLoader,
//        char *outputFilePath,
//        unsigned int precisionLoss) {
//    if (strcmp(preprocessingMode, "fast") == 0) {
//        createTNRFast(transitNodeSetSize, graphLoader, outputFilePath, precisionLoss);
//    } else if (strcmp(preprocessingMode, "slow") == 0) {
//        createTNRSlow(transitNodeSetSize, graphLoader, outputFilePath, precisionLoss);
//    } else if (strcmp(preprocessingMode, "dm") == 0) {
//        createTNRUsingDM(transitNodeSetSize, graphLoader, outputFilePath, precisionLoss);
//    } else {
//        throw input_error(std::string("Unknown preprocessing mode '") + preprocessingMode +
//                          "' for Transit Node Routing preprocessing.\n" + INVALID_FORMAT_INFO);
//    }
//}
//
///**
// * This function will create the Transit Node Routing with Arc Flags data structure based on a given input file.
// * In this case, the input format is XenGraph and the preprocessing mode is the 'slow' mode.
// *
// * @param transitNodeSetSize[in] The desired size of the transit nodes set.
// * @param graphLoader TODO
// * @param outputFilePath[in] Contains the desired output file path for the precomputed TNRAF data structure.
// */
//void createTNRAFSlow(
//        char *transitNodeSetSize,
//        GraphLoader &graphLoader,
//        char *outputFilePath,
//        unsigned int precisionLoss) {
//    Timer timer("Transit Node Routing with Arc Flags preprocessing (slow mode)");
//
//    UpdateableGraph graph(graphLoader.nodes());
//    graphLoader.loadGraph(graph, precisionLoss);
//    Graph *originalGraph = graph.createCopy();
//
//    timer.begin();
//    CHPreprocessor::preprocessForDDSG(graph);
//    timer.finish();
//
//    graphLoader.loadGraph(graph, precisionLoss);
//
//    timer.begin();
//    TNRAFPreprocessor::preprocessUsingCH(
//        graph, *originalGraph, outputFilePath,
//        boost::numeric_cast<unsigned int>(atol(transitNodeSetSize)), 32, false);
//    timer.finish();
//
//    timer.printMeasuredTime();
//}
//
///**
// * This function will create the Transit Node Routing with Arc Flags data structure based on a given input file.
// * In this case, the input format is XenGraph and the preprocessing mode is the 'dm' mode.
// *
// * @param transitNodeSetSize[in] The desired size of the transit nodes set.
// * @param graphLoader TODO
// * @param outputFilePath[in] Contains the desired output file path for the precomputed TNRAF data structure.
// */
//void createTNRAFUsingDM(
//        char *transitNodeSetSize,
//        GraphLoader &graphLoader,
//        char *outputFilePath,
//        unsigned int precisionLoss) {
//    Timer timer("Transit Node Routing with Arc Flags preprocessing (using distance matrix)");
//
//    // TODO tahle funkce je buhviproc uplne stejna jako createTNRAFSlow. Uz to tak bylo, tak jenom davam vedet.
//
//    UpdateableGraph graph(graphLoader.nodes());
//    graphLoader.loadGraph(graph, precisionLoss);
//    Graph *originalGraph = graph.createCopy();
//
//    timer.begin();
//    CHPreprocessor::preprocessForDDSG(graph);
//    timer.finish();
//
//    graphLoader.loadGraph(graph, precisionLoss);
//
//    timer.begin();
//    TNRAFPreprocessor::preprocessUsingCH(
//        graph, *originalGraph, outputFilePath,
//        boost::numeric_cast<unsigned int>(atol(transitNodeSetSize)), 32, true);
//    timer.finish();
//
//    timer.printMeasuredTime();
//}
//
///**
// * An input processing function used in the case when the user wants to precompute using the
// * Transit Node Routing with Arc Flags method. This function checks if the mode and the input format arguments
// * are valid, and if so, calls the corresponding function that will handle the preprocessing.
// *
// * @param preprocessingMode[in] Contains the argument determining which preprocessing mode is used (slow or DM)
// * @param transitNodeSetSize[in] Contains the argument determining the desired size of the transit node set.
// * @param graphLoader TODO
// * @param outputFilePath[in] Contains the desired output file path for the precomputed data structure.
// */
//void createTNRAF(
//        char *preprocessingMode,
//        char *transitNodeSetSize,
//        GraphLoader &graphLoader,
//        char *outputFilePath,
//        unsigned int precisionLoss) {
//    if (strcmp(preprocessingMode, "slow") == 0) {
//        createTNRAFSlow(transitNodeSetSize, graphLoader, outputFilePath, precisionLoss);
//    } else if (strcmp(preprocessingMode, "dm") == 0) {
//        createTNRAFUsingDM(transitNodeSetSize, graphLoader, outputFilePath, precisionLoss);
//    } else {
//        throw input_error(std::string("Unknown preprocessing mode '") + preprocessingMode +
//                          "' for Transit Node Routing with Arc Flags preprocessing.\n" + INVALID_FORMAT_INFO);
//    }
//}
//
//template<typename ComputorType>
//Distance_matrix_travel_time_provider* computeDistanceMatrix(GraphLoader &graphLoader, unsigned int precisionLoss, std::string timerName) {
//    ComputorType computor;
//    auto graph = computor.loadGraph(graphLoader, precisionLoss);
//
//    Timer timer(timerName);
//
//    timer.begin();
//    computor.computeDistanceMatrix(graph);
//    timer.finish();
//
//    timer.printMeasuredTime();
//
//    return computor.getDistanceMatrixInstance();
//}
//
//void createDM(
//        char *outputType,
//        char *preprocessingMode,
//        GraphLoader &graphLoader,
//        char *outputFilePath,
//        unsigned int precisionLoss) {
//    std::function<Distance_matrix_travel_time_provider* (GraphLoader &, unsigned int, std::string)> computor;
//
//    std::unique_ptr<DistanceMatrixOutputter> outputter{nullptr};
//    std::unique_ptr<Distance_matrix_travel_time_provider> dm{nullptr};
//
//    if (strcmp(preprocessingMode, "slow") == 0) {
//        computor = computeDistanceMatrix<DistanceMatrixComputorSlow>;
//    } else if (strcmp(preprocessingMode, "fast") == 0) {
//        computor = computeDistanceMatrix<DistanceMatrixComputorFast>;
//    } else {
//        throw input_error(std::string("Unknown preprocessing mode '") + preprocessingMode +
//                          "' for Distance Matrix preprocessing.\n" + INVALID_FORMAT_INFO);
//    }
//
//    if (strcmp(outputType, "xdm") == 0) {
//        outputter = std::unique_ptr<DistanceMatrixXdmOutputter> { new DistanceMatrixXdmOutputter()};
//    } else if (strcmp(outputType, "csv") == 0) {
//        outputter = std::unique_ptr<DistanceMatrixCsvOutputter> { new DistanceMatrixCsvOutputter()};
//    } else {
//        throw input_error(std::string("Unknown output type '") + outputType +
//                          "' for Distance Matrix preprocessing.\n" + INVALID_FORMAT_INFO);
//    }
//
//    dm = std::unique_ptr<Distance_matrix_travel_time_provider> {computor(graphLoader, precisionLoss, "Distance Matrix preprocessing")};
//    outputter->store(*dm, outputFilePath);
//}
//
///**
// * Benchmarks the basic Dijkstra's algorithm implementation using a given graph in the XenGraph input format and a given
// * set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
// * needed for one query in milliseconds. Additionally, the caller can specify an optional output file path,
// * where all the computed distances will be output. Those distances can then be for example compared with distances
// * computed by some other method to ensure correctness.
// *
// * @param inputFilePath[in] Path to the graph file that will be used for the benchmark. The input file must be in the
// * XenGraph input format.
// * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
// * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
// * to use them for example for verification purposes.
// * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
// * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
// */
//void benchmarkDijkstra(
//        char *inputFilePath,
//        char *queriesFilePath,
//        char *distancesOutputPath = nullptr,
//        bool outputDistances = false) {
//    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
//    std::vector<std::pair<unsigned int, unsigned int> > trips;
//    tripsLoader.loadTrips(trips);
//
//    XenGraphLoader dijkstraGraphLoader(inputFilePath);
//    Graph dijkstraGraph(dijkstraGraphLoader.nodes());
//    dijkstraGraphLoader.loadGraph(dijkstraGraph, 1);
//
//    std::vector<unsigned int> dijkstraDistances(trips.size());
//    double dijkstraTime = DijkstraBenchmark::benchmark(trips, dijkstraGraph, dijkstraDistances);
//
//    std::cout << "Run " << trips.size() << " queries using Dijkstra's algorithm in " << dijkstraTime << " seconds." << std::endl;
//    std::cout << "That means " << (dijkstraTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;
//
//    if (outputDistances) {
//        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;
//
//        std::ofstream output;
//        output.open(distancesOutputPath);
//
//        output << queriesFilePath << std::endl;
//        for (size_t i = 0; i < trips.size(); ++i) {
//            output << dijkstraDistances[i] << std::endl;
//        }
//
//        output.close();
//    }
//}
//
///**
// * Benchmarks the basic Dijkstra's algorithm implementation using a given graph in the XenGraph input format and a given
// * set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
// * needed for one query in milliseconds. Additionally, the caller can specify an optional output file path,
// * where all the computed distances will be output. Those distances can then be for example compared with distances
// * computed by some other method to ensure correctness.
// *
// * @param inputFilePath[in] Path to the graph file that will be used for the benchmark. The input file must be in the
// * XenGraph input format.
// * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
// * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
// * used internally in the data structure and the query algorithm.
// * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
// * to use them for example for verification purposes.
// * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
// * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
// */
//void benchmarkDijkstraWithMapping(
//        char *inputFilePath,
//        char *queriesFilePath,
//        char *mappingFilePath,
//        char *distancesOutputPath = nullptr,
//        bool outputDistances = false) {
//    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
//    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
//    tripsLoader.loadLongLongTrips(trips);
//
//    XenGraphLoader dijkstraGraphLoader(inputFilePath);
//    Graph dijkstraGraph(dijkstraGraphLoader.nodes());
//    dijkstraGraphLoader.loadGraph(dijkstraGraph, 1);
//
//    std::vector<unsigned int> dijkstraDistances(trips.size());
//    double dijkstraTime = DijkstraBenchmark::benchmarkUsingMapping(trips, dijkstraGraph, dijkstraDistances,
//                                                                   mappingFilePath);
//
//    std::cout << "Run " << trips.size() << " queries using Dijkstra's algorithm in " << dijkstraTime << " seconds" << std::endl;
//    std::cout << "using '" << mappingFilePath << "' as mapping." << std::endl;
//    std::cout << "That means " << (dijkstraTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;
//
//
//    if (outputDistances) {
//        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;
//
//        std::ofstream output;
//        output.open(distancesOutputPath);
//
//        output << queriesFilePath << std::endl;
//        for (size_t i = 0; i < trips.size(); ++i) {
//            output << dijkstraDistances[i] << std::endl;
//        }
//
//        output.close();
//    }
//}
//
///**
// * Benchmarks the Contraction Hierarchies query algorithm using a given precomputed data structure and a given
// * set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
// * needed for one query in milliseconds. Additionally, the caller can specify an optional output file path,
// * where all the computed distances will be output. Those distances can then be for example compared with distances
// * computed by some other method to ensure correctness.
// *
// * @param inputFilePath[in] Path to the file containing the precomputed Contraction Hierarchies data structure.
// * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
// * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
// * to use them for example for verification purposes.
// * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
// * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
// */
//void benchmarkCH(
//        char *inputFilePath,
//        char *queriesFilePath,
//        char *distancesOutputPath = nullptr,
//        bool outputDistances = false) {
//    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
//    std::vector<std::pair<unsigned int, unsigned int> > trips;
//    tripsLoader.loadTrips(trips);
//
//    DDSGLoader chLoader = DDSGLoader(inputFilePath);
//    FlagsGraph *ch = chLoader.loadFlagsGraph();
//
//    std::vector<unsigned int> chDistances(trips.size());
//    double chTime = CHBenchmark::benchmark(trips, *ch, chDistances);
//
//    delete ch;
//
//    std::cout << "Run " << trips.size() << " queries using Contraction Hierarchies query algorithm in " << chTime << " seconds." << std::endl;
//    std::cout << "That means " << (chTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;
//
//    if (outputDistances) {
//        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;
//
//        std::ofstream output;
//        output.open(distancesOutputPath);
//
//        output << queriesFilePath << std::endl;
//        for(size_t i = 0; i < trips.size(); ++i) {
//            output << chDistances[i] << std::endl;
//        }
//
//        output.close();
//    }
//}
//
///**
// * Benchmarks the Contraction Hierarchies query algorithm using a given precomputed data structure, a given
// * set of queries and a given mapping. Prints out the sum of the time required by all the queries in seconds
// * and the average time needed for one query in milliseconds. Additionally, the caller can specify an optional
// * output file path, where all the computed distances will be output. Those distances can then be for example
// * compared with distances computed by some other method to ensure correctness.
// *
// * @param inputFilePath[in] Path to the file containing the precomputed Contraction Hierarchies data structure.
// * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
// * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
// * used internally in the data structure and the query algorithm.
// * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
// * to use them for example for verification purposes.
// * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
// * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
// */
//void benchmarkCHwithMapping(
//        char *inputFilePath,
//        char *queriesFilePath,
//        char *mappingFilePath,
//        char *distancesOutputPath = nullptr,
//        bool outputDistances = false) {
//    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
//    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
//    tripsLoader.loadLongLongTrips(trips);
//
//    DDSGLoader chLoader = DDSGLoader(inputFilePath);
//    FlagsGraph *ch = chLoader.loadFlagsGraph();
//
//    std::vector<unsigned int> chDistances(trips.size());
//    double chTime = CHBenchmark::benchmarkUsingMapping(trips, *ch, chDistances, mappingFilePath);
//
//    delete ch;
//
//    std::cout << "Run " << trips.size() << " queries using Contraction Hierarchies query algorithm in " << chTime << " seconds" << std::endl;
//    std::cout << "using '" << mappingFilePath << "' as mapping." << std::endl;
//    std::cout << "That means " << (chTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;
//
//    if (outputDistances) {
//        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;
//
//        std::ofstream output;
//        output.open(distancesOutputPath);
//
//        output << queriesFilePath << std::endl;
//        for(size_t i = 0; i < trips.size(); ++i) {
//            output << chDistances[i] << std::endl;
//        }
//
//        output.close();
//    }
//}
//
///**
// * Benchmarks the Transit Node Routing query algorithm using a given precomputed data structure and a given
// * set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
// * needed for one query in milliseconds. Additionally, the caller can specify an optional output file path,
// * where all the computed distances will be output. Those distances can then be for example compared with distances
// * computed by some other method to ensure correctness.
// *
// * @param inputFilePath[in] Path to the file containing the precomputed Transit Node Routing data structure.
// * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
// * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
// * to use them for example for verification purposes.
// * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
// * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
// */
//void benchmarkTNR(
//        char *inputFilePath,
//        char *queriesFilePath,
//        char *distancesOutputPath = nullptr,
//        bool outputDistances = false) {
//    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
//    std::vector<std::pair<unsigned int, unsigned int> > trips;
//    tripsLoader.loadTrips(trips);
//
//    TNRGLoader tnrLoader = TNRGLoader(inputFilePath);
//    TransitNodeRoutingGraph *tnrGraph = tnrLoader.loadTNRforDistanceQueries();
//
//    std::vector<unsigned int> tnrDistances(trips.size());
//    double tnrTime = TNRBenchmark::benchmark(trips, *tnrGraph, tnrDistances);
//
//    delete tnrGraph;
//
//    std::cout << "Run " << trips.size() << " queries using Transit Node Routing query algorithm in " << tnrTime << " seconds." << std::endl;
//    std::cout << "That means " << (tnrTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;
//
//    if (outputDistances) {
//        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;
//
//        std::ofstream output;
//        output.open(distancesOutputPath);
//
//        output << queriesFilePath << std::endl;
//        for(size_t i = 0; i < trips.size(); ++i) {
//            output << tnrDistances[i] << std::endl;
//        }
//
//        output.close();
//    }
//}
//
///**
// * Benchmarks the Transit Node Routing query algorithm using a given precomputed data structure, a given set of queries
// * and a given mapping. Prints out the sum of the time required by all the queries in seconds and the average time
// * needed for one query in milliseconds. Additionally, the caller can specify an optional output file path,
// * where all the computed distances will be output. Those distances can then be for example compared with distances
// * computed by some other method to ensure correctness.
// *
// * @param inputFilePath[in] Path to the file containing the precomputed Transit Node Routing data structure.
// * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
// * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
// * used internally in the data structure and the query algorithm.
// * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
// * to use them for example for verification purposes.
// * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
// * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
// */
//void benchmarkTNRwithMapping(
//        char *inputFilePath,
//        char *queriesFilePath,
//        char *mappingFilePath,
//        char *distancesOutputPath = nullptr,
//        bool outputDistances = false) {
//    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
//    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
//    tripsLoader.loadLongLongTrips(trips);
//
//    TNRGLoader tnrLoader = TNRGLoader(inputFilePath);
//    TransitNodeRoutingGraph *tnrGraph = tnrLoader.loadTNRforDistanceQueries();
//
//    std::vector<unsigned int> tnrDistances(trips.size());
//    double tnrTime = TNRBenchmark::benchmarkWithMapping(trips, *tnrGraph, tnrDistances, mappingFilePath);
//
//    delete tnrGraph;
//
//    std::cout << "Run " << trips.size() << " queries using Transit Node Routing query algorithm in " << tnrTime << " seconds." << std::endl;
//    std::cout << "That means " << (tnrTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;
//
//    if (outputDistances) {
//        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;
//
//        std::ofstream output;
//        output.open(distancesOutputPath);
//
//        output << queriesFilePath << std::endl;
//        for(size_t i = 0; i < trips.size(); ++i) {
//            output << tnrDistances[i] << std::endl;
//        }
//
//        output.close();
//    }
//}
//
///**
// * Benchmarks the Transit Node Routing with Arc Flags query algorithm using a given precomputed data structure and
// * a given set of queries. Prints out the sum of the time required by all the queries in seconds and the average time
// * needed for one query in milliseconds. Additionally, the caller can specify an optional output file path,
// * where all the computed distances will be output. Those distances can then be for example compared with distances
// * computed by some other method to ensure correctness.
// *
// * @param inputFilePath[in] Path to the file containing the precomputed Transit Node Routing with Arc Flags data
// * structure.
// * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
// * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
// * to use them for example for verification purposes.
// * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
// * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
// */
//void benchmarkTNRAF(
//        char *inputFilePath,
//        char *queriesFilePath,
//        char *distancesOutputPath = nullptr,
//        bool outputDistances = false) {
//    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
//    std::vector<std::pair<unsigned int, unsigned int> > trips;
//    tripsLoader.loadTrips(trips);
//
//    TGAFLoader tnrafLoader = TGAFLoader(inputFilePath);
//    TransitNodeRoutingArcFlagsGraph *tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();
//
//    std::vector<unsigned int> tnrafDistances(trips.size());
//    double tnrafTime = TNRAFBenchmark::benchmark(trips, *tnrafGraph, tnrafDistances);
//
//    delete tnrafGraph;
//
//    std::cout << "Run " << trips.size() << " queries using Transit Node Routing query algorithm in " << tnrafTime << " seconds." << std::endl;
//    std::cout << "That means " << (tnrafTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;
//
//    if (outputDistances) {
//        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;
//
//        std::ofstream output;
//        output.open(distancesOutputPath);
//
//        output << queriesFilePath << std::endl;
//        for(size_t i = 0; i < trips.size(); ++i) {
//            output << tnrafDistances[i] << std::endl;
//        }
//
//        output.close();
//    }
//}
//
///**
// * Benchmarks the Transit Node Routing with Arc Flags query algorithm using a given precomputed data structure,
// * a given set of queries and a given mapping. Prints out the sum of the time required by all the queries in seconds
// * and the average time needed for one query in milliseconds. Additionally, the caller can specify an optional
// * output file path, where all the computed distances will be output. Those distances can then be for example
// * compared with distances computed by some other method to ensure correctness.
// *
// * @param inputFilePath[in] Path to the file containing the precomputed Transit Node Routing with Arc Flags data
// * structure.
// * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
// * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
// * used internally in the data structure and the query algorithm.
// * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
// * to use them for example for verification purposes.
// * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
// * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
// */
//void benchmarkTNRAFwithMapping(
//        char *inputFilePath,
//        char *queriesFilePath,
//        char *mappingFilePath,
//        char *distancesOutputPath = nullptr,
//        bool outputDistances = false) {
//    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
//    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
//    tripsLoader.loadLongLongTrips(trips);
//
//    TGAFLoader tnrafLoader = TGAFLoader(inputFilePath);
//    TransitNodeRoutingArcFlagsGraph *tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();
//
//    std::vector<unsigned int> tnrafDistances(trips.size());
//    double tnrafTime = TNRAFBenchmark::benchmarkWithMapping(trips, *tnrafGraph, tnrafDistances, mappingFilePath);
//
//    delete tnrafGraph;
//
//    std::cout << "Run " << trips.size() << " queries using Transit Node Routing query algorithm in " << tnrafTime << " seconds." << std::endl;
//    std::cout << "That means " << (tnrafTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;
//
//    if (outputDistances) {
//        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;
//
//        std::ofstream output;
//        output.open(distancesOutputPath);
//
//        output << queriesFilePath << std::endl;
//        for(size_t i = 0; i < trips.size(); ++i) {
//            output << tnrafDistances[i] << std::endl;
//        }
//
//        output.close();
//    }
//}
//
///**
// * TODO
// */
//GraphLoader *newGraphLoader(char *inputType, char *inputFilePath) {
//    if (strcmp(inputType, "xengraph") == 0) {
//        return new XenGraphLoader(inputFilePath);
//    } else if (strcmp(inputType, "dimacs") == 0) {
//        return new DIMACSLoader(inputFilePath);
//    } else if (strcmp(inputType, "csv") == 0) {
//        return new CsvGraphLoader(inputFilePath);
//    } else {
//        throw input_error(std::string("Unknown input format '") + inputType + "'.\n" + INVALID_FORMAT_INFO);
//    }
//}
//
///**
// * @mainpage Shortest Paths computation library
// *
// * This project is library that allows fast distance queries on directed weighted graphs by preprocessing those graphs
// * to speed up the queries. This means that given any directed weighted graph (can be a road network for example),
// * this library will spend some time precomputing auxiliary structures, that will then allow the queries to be answered
// * much quicker than using the Dijkstra's Algorithm. The memory required by the auxiliary structures is much lower than
// * in the case of full Distance Matrix, so this library should be used in cases where having the full distance matrix is
// * not possible (usually because of insufficient memory), but Dijkstra's Algorithm is too slow.
// *
// * There are three methods implemented in this library - Contraction Hierarchies, Transit Node Routing and Transit Node
// * Routing with Arc Flags. Those methods are listed from the slowest but least memory requiring one to the fastest but
// * most memory requiring one.
// *
// * # Useful information
// *
// * For more information about how to use this library, how to compile it, available command line arguments, info on
// * how to integrate this library with existing projects and so on, please refer to the README.md file in the
// * root directory of this project.
// *
// * <br> _Author: Michal Cvach_
// */
int main(int argc, char *argv[]) {
//    setvbuf(stdout, NULL, _IONBF, 0);
//
//    if (argc < 5 || argc > 8) {
//        printUsageInfo(argv[0]);
//        return 0;
//    }
//
//    try {
//        // The user wants to create data structures using some method.
//        if (strcmp(argv[1], "create") == 0) {
//            const auto preprocessings = {
//                make_tuple(std::string("ch"), std::string("Contraction Hierarchies"), 6,
//                    std::function<void (char**, GraphLoader&, unsigned int)>(
//                      [](char** argv, GraphLoader &graphLoader, unsigned int precisionLoss) {
//                        createCH(graphLoader, argv[5], precisionLoss);
//                        })),
//                make_tuple(std::string("tnr"), std::string("Transit Node Routing"), 8,
//                    std::function<void (char**, GraphLoader&, unsigned int)>(
//                      [](char** argv, GraphLoader &graphLoader, unsigned int precisionLoss) {
//                        createTNR(argv[4], argv[5], graphLoader, argv[7], precisionLoss);
//                        })),
//                make_tuple(std::string("tnraf"), std::string("Transit Node Routing with Arc Flags"), 8,
//                    std::function<void (char**, GraphLoader&, unsigned int)>(
//                      [](char** argv, GraphLoader &graphLoader, unsigned int precisionLoss) {
//                        createTNRAF(argv[4], argv[5], graphLoader, argv[7], precisionLoss);
//                        })),
//                make_tuple(std::string("dm"), std::string("Distance Matrix"), 8,
//                    std::function<void (char**, GraphLoader&, unsigned int)>(
//                      [](char** argv, GraphLoader& graphLoader, unsigned int precisionLoss) {
//                        createDM(argv[4], argv[5], graphLoader, argv[7], precisionLoss);
//                        }))
//            };
//
//            bool found = false;
//            for(auto &preprocessing : preprocessings) {
//                auto &name = get<0>(preprocessing);
//                if (name.compare(argv[2]) == 0) {
//                    auto expectedArgc = get<2>(preprocessing);
//
//                    if (argc != expectedArgc && argc != expectedArgc + 1) {
//                        throw input_error(std::string("Invalid amount of arguments for ")
//                                + get<1>(preprocessing)
//                                + " preprocessing.\n"
//                                + INVALID_FORMAT_INFO);
//                    }
//
//                    unsigned int precisionLoss;
//                    if (argc == expectedArgc)
//                        precisionLoss = 1;
//                    else
//                        precisionLoss = (unsigned int) stoi(argv[expectedArgc]);
//
//                    GraphLoader* graphLoader = newGraphLoader(argv[3], argv[expectedArgc - 2]);
//                    auto &func = get<3>(preprocessing);
//                    func(argv, *graphLoader, precisionLoss);
//                    delete graphLoader;
//
//                    found = true;
//                    break;
//                }
//            }
//
//            if(!found) {
//                throw input_error(INVALID_USAGE_INFO);
//            }
//        }
//        else if (strcmp(argv[1], "benchmark") == 0) {
//            const auto benchmarks = {
//                    make_tuple(std::string("dijkstra"), benchmarkDijkstra, benchmarkDijkstraWithMapping),
//                    make_tuple(std::string("ch"), benchmarkCH, benchmarkCHwithMapping),
//                    make_tuple(std::string("tnr"), benchmarkTNR, benchmarkTNRwithMapping),
//                    make_tuple(std::string("tnraf"), benchmarkTNRAF, benchmarkTNRAFwithMapping),
//            };
//
//            bool found = false;
//            for (auto &benchmark : benchmarks) {
//                auto &name = get<0>(benchmark);
//
//                if (name.compare(argv[2]) == 0) {
//                    if (strcmp(argv[3], "nomapping") == 0) {
//                        auto &func = get<1>(benchmark);
//                        if (argc == 7) {
//                            func(argv[4], argv[5], argv[6], true);
//                        } else if (argc == 6) {
//                            func(argv[4], argv[5], nullptr, false);
//                        } else {
//                            throw input_error(
//                                    std::string("Invalid amount of arguments for") + name + " benchmarking.\n" +
//                                    INVALID_FORMAT_INFO);
//                        }
//                    } else if (strcmp(argv[3], "mapping") == 0) {
//                        auto &func = get<2>(benchmark);
//                        if (argc == 8) {
//                            func(argv[4], argv[5], argv[6], argv[7], true);
//                        } else if (argc == 7) {
//                            func(argv[4], argv[5], argv[6], nullptr, false);
//                        } else {
//                            throw input_error(
//                                    std::string("Invalid amount of arguments for") + name + " benchmarking.\n" +
//                                    INVALID_FORMAT_INFO);
//                        }
//                    } else {
//                        throw input_error(std::string("Option '") + argv[3] + "' does not make sense for benchmarking.\n" +
//                                          "Either 'mapping' or 'nomapping' is expected.\n" + INVALID_FORMAT_INFO);
//                    }
//
//                    found = true;
//                    break;
//                }
//            }
//
//            if (!found) {
//                throw input_error(INVALID_USAGE_INFO);
//            }
//        } else {
//            throw input_error(INVALID_USAGE_INFO);
//        }
//    }
//    catch (input_error &e) {
//        std::cout << "Input Error: " << e.what();
//        return 1;
//    }
//    catch (not_implemented_error &e) {
//        std::cout << "Not Implemented Error: " << e.what();
//        return 1;
//    }
//    catch (const std::exception &e) {
//        std::cout << "Error: " << e.what();
//        return 1;
//    }
//
//    return 0;
}
