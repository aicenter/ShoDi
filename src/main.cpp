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
#include <filesystem>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/optional/optional_io.hpp>
#include <string>
#include <tuple>
#include "DistanceMatrix/Distance_matrix_travel_time_provider.h"
#include "GraphBuilding/Loaders/DIMACSLoader.h"
#include "GraphBuilding/Loaders/GraphLoader.h"
#include "GraphBuilding/Loaders/TNRGLoader.h"
#include "GraphBuilding/Loaders/XenGraphLoader.h"
#include "GraphBuilding/Structures/UpdateableGraph.h"
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
#include "Benchmarking/DistanceMatrixBenchmark.h"
#include "Benchmarking/memory.h"
#include "GraphBuilding/Loaders/DistanceMatrixLoader.h"
#include "GraphBuilding/Loaders/TGAFLoader.h"
#include "Benchmarking/DijkstraBenchmark.h"
#include "Benchmarking/AstarBenchmark.h"
#include "Benchmarking/LocationTransformer.h"
#include "Error/Error.h"
#include "GraphBuilding/Loaders/AdjGraphLoader.h"
#include "GraphBuilding/Loaders/CsvGraphLoader.h"
#include "DistanceMatrix/DistanceMatrixComputorSlow.h"
#include "DistanceMatrix/johnson.hpp"
#include "DistanceMatrix/DistanceMatrixComputorFast.h"
#include "DistanceMatrix/DistanceMatrixOutputter.h"
#include "DistanceMatrix/DistanceMatrixXdmOutputter.h"
#include "DistanceMatrix/DistanceMatrixCsvOutputter.h"
#include "DistanceMatrix/DistanceMatrixHdfOutputter.h"



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
           "  '%s create -m tnraf -f xengraph -i input_graph.xeng -o output_file --preprocessing-mode dm --tnodes-cnt 1000'\n"
           "  '%s create -m ch -f xengraph -i input_graph.xeng -o output_file'\n"
           "  '%s benchmark -m tnraf --input-structure output_file.tgaf --query-set queries.txt'\n", appName, appName, appName);
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
        GraphLoader& graphLoader,
        const std::string& outputFilePath,
        int scaling_factor) {
    Timer timer("Contraction Hierarchies from DIMACS preprocessing");

    UpdateableGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, scaling_factor);

    timer.begin();
    CHPreprocessor::preprocessForDDSG(graph);
    timer.finish();

    graphLoader.loadGraph(graph, scaling_factor);
    graph.flushInDdsgFormat(outputFilePath);

    timer.printMeasuredTime();
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
        unsigned int transitNodeSetSize,
        GraphLoader& graphLoader,
        const std::string& outputFilePath,
        int scaling_factor) {
    Timer timer("Transit Node Routing preprocessing (fast mode)");

    UpdateableGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, scaling_factor);

    timer.begin();
    CHPreprocessor::preprocessForDDSG(graph);
    timer.finish();

    graphLoader.loadGraph(graph, scaling_factor);

    timer.begin();
    TNRPreprocessor::preprocessUsingCH(graph, outputFilePath, transitNodeSetSize);
    timer.finish();

    timer.printMeasuredTime();
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
        unsigned int transitNodeSetSize,
        GraphLoader& graphLoader,
        const std::string& outputFilePath,
        int scaling_factor) {
    Timer timer("Transit Node Routing preprocessing (slow mode)");

    UpdateableGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, scaling_factor);
    Graph* originalGraph = graph.createCopy();

    timer.begin();
    CHPreprocessor::preprocessForDDSG(graph);
    timer.finish();

    graphLoader.loadGraph(graph, scaling_factor);

    timer.begin();
    TNRPreprocessor::preprocessUsingCHslower(
        graph, *originalGraph, outputFilePath, transitNodeSetSize);
    timer.finish();

    delete originalGraph;

    timer.printMeasuredTime();
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
        unsigned int transitNodeSetSize,
        unsigned int intSize,
        GraphLoader& graphLoader,
        const std::string& outputFilePath,
        int scaling_factor) {
    Timer timer("Transit Node Routing preprocessing (using distance matrix)");

    UpdateableGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, scaling_factor);
    Graph* originalGraph = graph.createCopy();

    timer.begin();
    CHPreprocessor::preprocessForDDSG(graph);
    timer.finish();

    graphLoader.loadGraph(graph, scaling_factor);

    timer.begin();
    TNRPreprocessor::preprocessWithDMvalidation(graph, *originalGraph, outputFilePath, transitNodeSetSize, intSize);
    timer.finish();

    delete originalGraph;

    timer.printMeasuredTime();
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
        std::string preprocessingMode,
        unsigned int transitNodeSetSize,
        unsigned int dmIntSize,
        GraphLoader& graphLoader,
        const std::string& outputFilePath,
        int scaling_factor) {
    if (preprocessingMode == "fast") {
        createTNRFast(transitNodeSetSize, graphLoader, outputFilePath, scaling_factor);
    } else if (preprocessingMode == "slow") {
        createTNRSlow(transitNodeSetSize, graphLoader, outputFilePath, scaling_factor);
    } else if (preprocessingMode == "dm") {
        createTNRUsingDM(transitNodeSetSize, dmIntSize, graphLoader, outputFilePath, scaling_factor);
    } else {
        throw input_error(std::string("Unknown preprocessing mode '") + preprocessingMode +
                          "' for Transit Node Routing preprocessing.\n" + INVALID_FORMAT_INFO);
    }
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
        const std::string& preprocessingMode,
        unsigned int transitNodeSetSize,
        unsigned int dmIntSize,
        GraphLoader& graphLoader,
        const std::string& outputFilePath,
        int scaling_factor) {
	bool dm_mode;
    if (preprocessingMode == "slow") {
//        createTNRAFSlow(transitNodeSetSize, graphLoader, outputFilePath, scaling_factor);
		dm_mode = false;
    } else if (preprocessingMode == "dm") {
//        createTNRAFUsingDM(transitNodeSetSize, graphLoader, outputFilePath, scaling_factor);
		dm_mode = true;
    } else {
        throw input_error(std::string("Unknown preprocessing mode '") + preprocessingMode +
                          "' for Transit Node Routing with Arc Flags preprocessing.\n" + INVALID_FORMAT_INFO);
    }

	Timer timer("Transit Node Routing with Arc Flags preprocessing");

	UpdateableGraph graph(graphLoader.nodes());
	graphLoader.loadGraph(graph, scaling_factor);
	Graph* originalGraph = graph.createCopy();

	timer.begin();
	CHPreprocessor::preprocessForDDSG(graph);
	timer.finish();

	graphLoader.loadGraph(graph, scaling_factor);

	auto num_regions = std::min(graph.nodes(), 32u);

	timer.begin();
	TNRAFPreprocessor::preprocessUsingCH(
		graph, *originalGraph, outputFilePath, transitNodeSetSize,
		    num_regions, dmIntSize, dm_mode);
	timer.finish();

	timer.printMeasuredTime();
}

/*template<class ComputorType, class IntType>
Distance_matrix_travel_time_provider<IntType>* computeDistanceMatrix(GraphLoader &graphLoader, int scaling_factor, std::string timerName) {
    ComputorType computor;
    auto graph = computor.loadGraph(graphLoader, scaling_factor);

    Timer timer(timerName);

    timer.begin();
    computor.computeDistanceMatrix(graph);
    timer.finish();

    timer.printMeasuredTime();

    return computor.getDistanceMatrixInstance();
}*/

template<class IntType>
void createDM(
        const std::string& outputFormat,
        const std::string& preprocessingMode,
        GraphLoader& graphLoader,
        const std::string& outputFilePath,
        int scaling_factor) {
    std::unique_ptr<DistanceMatrixOutputter<IntType>> outputter{nullptr};
    bool fast = false;

    if (preprocessingMode == "slow") {
        //dm.computeDistanceMatrix(false, graphLoader, scaling_factor, "Distance Matrix preprocessing");
    } else if (preprocessingMode == "fast") {
        fast = true;
    } else {
        throw input_error(std::string("Unknown preprocessing mode '") + preprocessingMode +
                          "' for Distance Matrix preprocessing.\n" + INVALID_FORMAT_INFO);
    }

    if (outputFormat == "xdm") {
        outputter = std::unique_ptr<DistanceMatrixXdmOutputter<IntType>> { new DistanceMatrixXdmOutputter<IntType>()};
    } else if (outputFormat == "csv") {
        outputter = std::unique_ptr<DistanceMatrixCsvOutputter<IntType>> { new DistanceMatrixCsvOutputter<IntType>()};
    } else if (outputFormat == "hdf") {
        outputter = std::unique_ptr<DistanceMatrixHdfOutputter<IntType>>{ new DistanceMatrixHdfOutputter<IntType>() };
    } else {
        throw input_error(std::string("Unknown output type '") + outputFormat +
                          "' for Distance Matrix preprocessing.\n" + INVALID_FORMAT_INFO);
    }
    auto dm = std::make_unique<Distance_matrix_travel_time_provider<IntType>>(fast, graphLoader, scaling_factor);
    outputter->store(*dm, outputFilePath);
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
 * @return Total time in seconds.
 */
double benchmarkDijkstra(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& distancesOutputPath = "",
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    CsvGraphLoader csvGraphLoader = CsvGraphLoader(inputFilePath);
    Graph dijkstraGraph(csvGraphLoader.nodes());
    csvGraphLoader.loadGraph(dijkstraGraph, 1);

    std::vector<unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::benchmark(trips, dijkstraGraph, dijkstraDistances);

    std::cout << "Run " << trips.size() << " queries using Dijkstra's algorithm in " << dijkstraTime << " seconds." << std::endl;
    std::cout << "That means " << (dijkstraTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;

    if (outputDistances) {
        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;

        std::ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << std::endl;
        for (size_t i = 0; i < trips.size(); ++i) {
            output << dijkstraDistances[i] << std::endl;
        }

        output.close();
    }
    return dijkstraTime;
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
 * @return Total time in seconds.
 */
double benchmarkDijkstraWithMapping(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& mappingFilePath,
        const std::string& distancesOutputPath = "",
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    CsvGraphLoader csvGraphLoader = CsvGraphLoader(inputFilePath);
    Graph dijkstraGraph(csvGraphLoader.nodes());
    csvGraphLoader.loadGraph(dijkstraGraph, 1);

    std::vector<unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::benchmarkUsingMapping(trips, dijkstraGraph, dijkstraDistances,
                                                                   mappingFilePath);

    std::cout << "Run " << trips.size() << " queries using Dijkstra's algorithm in " << dijkstraTime << " seconds" << std::endl;
    std::cout << "using '" << mappingFilePath << "' as mapping." << std::endl;
    std::cout << "That means " << (dijkstraTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;


    if (outputDistances) {
        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;

        std::ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << std::endl;
        for (size_t i = 0; i < trips.size(); ++i) {
            output << dijkstraDistances[i] << std::endl;
        }

        output.close();
    }
    return dijkstraTime;
}

/**
 * Benchmarks the A* algorithm implementation using a given graph in the CSV input format and a given
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
 * @return Total time in seconds.
 */
double benchmarkAstar(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& distancesOutputPath = "",
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    CsvGraphLoader csvGraphLoader = CsvGraphLoader(inputFilePath);
    Graph aStarGraph(csvGraphLoader.nodes());
    csvGraphLoader.loadGraph(aStarGraph, 1);
    auto gpsLocations = std::vector<std::pair<double, double>>(csvGraphLoader.nodes());
    auto projectedLocations = std::vector<std::pair<double, double>>();
    csvGraphLoader.loadLocations(gpsLocations);
    LocationTransformer::transformLocations(gpsLocations, projectedLocations);

    std::vector<unsigned int> aStarDistances(trips.size());
    double aStarTime = AstarBenchmark::benchmark(trips, aStarGraph, projectedLocations, aStarDistances);

    std::cout << "Run " << trips.size() << " queries using A* algorithm in " << aStarTime << " seconds." << std::endl;
    std::cout << "That means " << (aStarTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;

    if (outputDistances) {
        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;

        std::ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << std::endl;
        for (size_t i = 0; i < trips.size(); ++i) {
            output << aStarDistances[i] << std::endl;
        }

        output.close();
    }
    return aStarTime;
}

/**
 * Benchmarks the A* algorithm implementation using a given graph in the CSV input format and a given
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
 * @return Total time in seconds.
 */
double benchmarkAstarWithMapping(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& mappingFilePath,
        const std::string& distancesOutputPath = "",
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    CsvGraphLoader csvGraphLoader = CsvGraphLoader(inputFilePath);
    Graph aStarGraph(csvGraphLoader.nodes());
    csvGraphLoader.loadGraph(aStarGraph, 1);
    auto gpsLocations = std::vector<std::pair<double, double>>(csvGraphLoader.nodes());
    auto projectedLocations = std::vector<std::pair<double, double>>();
    csvGraphLoader.loadLocations(gpsLocations);
    LocationTransformer::transformLocations(gpsLocations, projectedLocations);

    std::vector<unsigned int> aStarDistances(trips.size());
    double aStarTime = AstarBenchmark::benchmarkUsingMapping(trips, aStarGraph, projectedLocations,
                                                             aStarDistances, mappingFilePath);

    std::cout << "Run " << trips.size() << " queries using A* algorithm in " << aStarTime << " seconds" << std::endl;
    std::cout << "using '" << mappingFilePath << "' as mapping." << std::endl;
    std::cout << "That means " << (aStarTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;


    if (outputDistances) {
        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;

        std::ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << std::endl;
        for (size_t i = 0; i < trips.size(); ++i) {
            output << aStarDistances[i] << std::endl;
        }

        output.close();
    }
    return aStarTime;
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
 * @return Total time in seconds.
 */
double benchmarkCH(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& distancesOutputPath = "",
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader(inputFilePath);
    FlagsGraph<NodeData>* ch = chLoader.loadFlagsGraph();

    std::vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::benchmark(trips, *ch, chDistances);

    delete ch;

    std::cout << "Run " << trips.size() << " queries using Contraction Hierarchies query algorithm in " << chTime << " seconds." << std::endl;
    std::cout << "That means " << (chTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;

    if (outputDistances) {
        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;

        std::ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << std::endl;
        for(size_t i = 0; i < trips.size(); ++i) {
            output << chDistances[i] << std::endl;
        }

        output.close();
    }
    return chTime;
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
 * @return Total time in seconds.
 */
double benchmarkCHwithMapping(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& mappingFilePath,
        const std::string& distancesOutputPath = "",
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    DDSGLoader chLoader = DDSGLoader(inputFilePath);
    FlagsGraph<NodeData>* ch = chLoader.loadFlagsGraph();

    std::vector<unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::benchmarkUsingMapping(trips, *ch, chDistances, mappingFilePath);

    delete ch;

    std::cout << "Run " << trips.size() << " queries using Contraction Hierarchies query algorithm in " << chTime << " seconds" << std::endl;
    std::cout << "using '" << mappingFilePath << "' as mapping." << std::endl;
    std::cout << "That means " << (chTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;

    if (outputDistances) {
        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;

        std::ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << std::endl;
        for(size_t i = 0; i < trips.size(); ++i) {
            output << chDistances[i] << std::endl;
        }

        output.close();
    }
    return chTime;
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
 * @return Total time in seconds.
 */
double benchmarkTNR(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& distancesOutputPath = "",
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    TNRGLoader tnrLoader = TNRGLoader(inputFilePath);
    TransitNodeRoutingGraph<NodeData>* tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    std::vector<unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::benchmark(trips, *tnrGraph, tnrDistances);

    delete tnrGraph;

    std::cout << "Run " << trips.size() << " queries using Transit Node Routing query algorithm in " << tnrTime << " seconds." << std::endl;
    std::cout << "That means " << (tnrTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;

    if (outputDistances) {
        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;

        std::ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << std::endl;
        for(size_t i = 0; i < trips.size(); ++i) {
            output << tnrDistances[i] << std::endl;
        }

        output.close();
    }
    return tnrTime;
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
 * @return Total time in seconds.
 */
double benchmarkTNRwithMapping(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& mappingFilePath,
        const std::string& distancesOutputPath = "",
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    TNRGLoader tnrLoader = TNRGLoader(inputFilePath);
    TransitNodeRoutingGraph<NodeData>* tnrGraph = tnrLoader.loadTNRforDistanceQueries();

    std::vector<unsigned int> tnrDistances(trips.size());
    double tnrTime = TNRBenchmark::benchmarkWithMapping(trips, *tnrGraph, tnrDistances, mappingFilePath);

    delete tnrGraph;

    std::cout << "Run " << trips.size() << " queries using Transit Node Routing query algorithm in " << tnrTime << " seconds." << std::endl;
    std::cout << "That means " << (tnrTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;

    if (outputDistances) {
        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;

        std::ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << std::endl;
        for(size_t i = 0; i < trips.size(); ++i) {
            output << tnrDistances[i] << std::endl;
        }

        output.close();
    }
    return tnrTime;
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
 * @return Total time in seconds.
 */
double benchmarkTNRAF(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& distancesOutputPath = "",
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    TGAFLoader tnrafLoader = TGAFLoader(inputFilePath);
    TransitNodeRoutingArcFlagsGraph* tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    std::vector<unsigned int> tnrafDistances(trips.size());
    double tnrafTime = TNRAFBenchmark::benchmark(trips, *tnrafGraph, tnrafDistances);

    delete tnrafGraph;

    std::cout << "Run " << trips.size() << " queries using Transit Node Routing query algorithm in " << tnrafTime << " seconds." << std::endl;
    std::cout << "That means " << (tnrafTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;

    if (outputDistances) {
        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;

        std::ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << std::endl;
        for(size_t i = 0; i < trips.size(); ++i) {
            output << tnrafDistances[i] << std::endl;
        }

        output.close();
    }
    return tnrafTime;
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
 * @return Total time in seconds.
 */
double benchmarkTNRAFwithMapping(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& mappingFilePath,
        const std::string& distancesOutputPath = "",
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    TGAFLoader tnrafLoader = TGAFLoader(inputFilePath);
    TransitNodeRoutingArcFlagsGraph* tnrafGraph = tnrafLoader.loadTNRAFforDistanceQueries();

    std::vector<unsigned int> tnrafDistances(trips.size());
    double tnrafTime = TNRAFBenchmark::benchmarkWithMapping(trips, *tnrafGraph, tnrafDistances, mappingFilePath);

    delete tnrafGraph;

    std::cout << "Run " << trips.size() << " queries using Transit Node Routing query algorithm in " << tnrafTime << " seconds." << std::endl;
    std::cout << "That means " << (tnrafTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;

    if (outputDistances) {
        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;

        std::ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << std::endl;
        for(size_t i = 0; i < trips.size(); ++i) {
            output << tnrafDistances[i] << std::endl;
        }

        output.close();
    }
    return tnrafTime;
}

/**
 * Benchmarks the Distance Matrix method using a given precomputed data structure and a given set of queries.
 * Prints out the sum of the time required by all the queries in seconds and the average time needed for one query
 * in milliseconds. Additionally, the caller can specify an optional output file path, where all the computed
 * distances will be output. Those distances can then be for example * compared with distances computed by some
 * other method to ensure correctness.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed distance matrix.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
 * to use them for example for verification purposes.
 * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
 * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
 * @return Total time in seconds.
 */
double benchmarkDM(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& distancesOutputPath = "",
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    DistanceMatrixLoader dmLoader = DistanceMatrixLoader(inputFilePath);
    DistanceMatrixInterface* dm = dmLoader.loadHDF();

    std::vector<unsigned int> dmDistances(trips.size());
    double dmTime = DistanceMatrixBenchmark::benchmark(trips, *dm, dmDistances);

    delete dm;

    std::cout << "Run " << trips.size() << " queries using Distance Matrix in " << dmTime << " seconds." << std::endl;
    std::cout << "That means " << (dmTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;

    if (outputDistances) {
        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;

        std::ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << std::endl;
        for(size_t i = 0; i < trips.size(); ++i) {
            output << dmDistances[i] << std::endl;
        }

        output.close();
    }
    return dmTime;
}

/**
 * Benchmarks the Distance Matrix method using a given precomputed data structure, a given set of queries and a given
 * mapping. Prints out the sum of the time required by all the queries in seconds and the average time needed for one
 * query in milliseconds. Additionally, the caller can specify an optional output file path, where all the computed
 * distances will be output. Those distances can then be for example compared with distances computed by some other
 * method to ensure correctness.
 *
 * @param inputFilePath[in] Path to the file containing the precomputed distance matrix.
 * @param queriesFilePath[in] Path to the file containing the queries used for the benchmark.
 * @param mappingFilePath[in] Path to the file containing the mapping from original IDs (used in the queries) to IDs
 * used internally in the data structure and the query algorithm.
 * @param distancesOutputPath[in] Optional path where the computed distances can be output if the caller wants
 * to use them for example for verification purposes.
 * @param outputDistances[in] Specifies whether the computed distances should be output into a plain text file or not.
 * If the parameter is set to 'true', distances are output into a file, otherwise they are not.
 * @return Total time in seconds.
 */
double benchmarkDMwithMapping(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& mappingFilePath,
        const std::string& distancesOutputPath = "",
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    DistanceMatrixLoader dmLoader = DistanceMatrixLoader(inputFilePath);
    Distance_matrix_travel_time_provider<dist_t>* dm = dmLoader.loadXDM();

    std::vector<unsigned int> dmDistances(trips.size());
    double dmTime = DistanceMatrixBenchmark::benchmarkUsingMapping(trips, *dm, dmDistances, mappingFilePath);

    delete dm;

    std::cout << "Run " << trips.size() << " queries using Distance Matrix in " << dmTime << " seconds." << std::endl;
    std::cout << "That means " << (dmTime / (double) trips.size()) * 1000 << " ms per query." << std::endl;

    if (outputDistances) {
        std::cout << "Now outputting distances to '" << distancesOutputPath << "'." << std::endl;

        std::ofstream output;
        output.open(distancesOutputPath);

        output << queriesFilePath << std::endl;
        for(size_t i = 0; i < trips.size(); ++i) {
            output << dmDistances[i] << std::endl;
        }

        output.close();
    }
    return dmTime;
}

/**
 * TODO
 */
GraphLoader *newGraphLoader(const std::string& inputFormat, const std::string& inputFilePath) {
    if (inputFormat == "xengraph") {
        return new XenGraphLoader(inputFilePath);
    } else if (inputFormat == "dimacs") {
        return new DIMACSLoader(inputFilePath);
    } else if (inputFormat == "adj") {
        return new AdjGraphLoader(inputFilePath);
    } else if (inputFormat == "csv") {
        return new CsvGraphLoader(inputFilePath);
    } else {
        throw input_error(std::string("Unknown input format '") + inputFormat + "'.\n" + INVALID_FORMAT_INFO);
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
int main(int argc, char* argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);

    boost::optional<std::string> command, method, inputFormat, inputPath, outputFormat, outputPath, preprocessingMode,
    inputStructure, querySet, mappingFile;
    boost::optional<unsigned int> tnodesCnt, dmIntSize, precisionLoss;

    // Declare the supported options.
    boost::program_options::options_description allOptions("Allowed options");
    allOptions.add_options()
            ("help", "produce help message")
            ("command", boost::program_options::value(&command))
            ("method,m", boost::program_options::value(&method))
            ("input-format,f", boost::program_options::value(&inputFormat))
            ("input-path,i", boost::program_options::value(&inputPath))
            ("output-format", boost::program_options::value(&outputFormat))
            ("output-path,o", boost::program_options::value(&outputPath))
            ("preprocessing-mode", boost::program_options::value(&preprocessingMode))
            ("int-size", boost::program_options::value(&dmIntSize)->default_value(0))
            ("tnodes-cnt", boost::program_options::value(&tnodesCnt))
            ("precision-loss", boost::program_options::value(&precisionLoss)->default_value(1))
            ("input-structure", boost::program_options::value(&inputStructure))
            ("query-set", boost::program_options::value(&querySet))
            ("mapping-file", boost::program_options::value(&mappingFile));

    boost::program_options::positional_options_description p;
    p.add("command", 1);

    boost::program_options::variables_map vm;
    boost::program_options::store(
            boost::program_options::command_line_parser(argc, argv).options(allOptions).positional(p).run(), vm);
    boost::program_options::notify(vm);

    if (!command) {
        printUsageInfo(argv[0]);
        return 0;
    }

    try {
        if (*command == "create") {
            if (vm.count("help")) {
                printf("To create a data structure, please provide required commands for the Create command:\n"
                       "-m <method>\n-f <input_format>\n-i <input_file>\n-o <output_file>\n");
                return 0;
            }

            if (!method || !inputPath) {
                throw input_error("Missing one or more required options (-m <method> / -i <input_file>) for the Create command.\n");
            }

            if (!inputFormat) {
                auto extension = std::filesystem::path(*inputPath).extension();

                if (extension == ".xeng") inputFormat.emplace("xengraph");
                else if (extension == ".gr") inputFormat.emplace("dimacs");
                else if (extension == ".csv") inputFormat.emplace("adj");
                else if (extension == "") inputFormat.emplace("csv");
                else throw input_error("Unable to detect input file format. Please specify with '-f <format>'.");
            }

            if (!outputPath) {
                outputPath.emplace("out");
            }

            GraphLoader* graphLoader = newGraphLoader(*inputFormat, *inputPath);

            if (*method == "ch") {
                createCH(*graphLoader, *outputPath, *precisionLoss);
            } else if (*method == "tnr") {
                if (!preprocessingMode || !tnodesCnt) {
                    throw input_error("Missing one or more required options (--preprocessing-mode <fast/slow/dm> / --tnodes-cnt <cnt>) for TNR creation.\n");
                }
                createTNR(*preprocessingMode, *tnodesCnt, *dmIntSize, *graphLoader, *outputPath, *precisionLoss);
            } else if (*method == "tnraf") {
                if (!preprocessingMode || !tnodesCnt) {
                    throw input_error("Missing one or more required options (--preprocessing-mode <slow/dm> / --tnodes-cnt <cnt>) for TNRAF creation.\n");
                }
                createTNRAF(*preprocessingMode, *tnodesCnt, *dmIntSize, *graphLoader, *outputPath, *precisionLoss);
            } else if (*method == "dm") {
                if (!preprocessingMode || !outputFormat) {
                    throw input_error("Missing one or more required options (--preprocessing-mode <fast/slow> / --output-format <xdm/csv/hdf>) for DM creation.\n");
                }

                if (*dmIntSize == 16) {
                    createDM<uint_least16_t>(*outputFormat, *preprocessingMode, *graphLoader, *outputPath, boost::numeric_cast<int>(*precisionLoss));
                } else if (*dmIntSize == 32) {
                    createDM<uint_least32_t>(*outputFormat, *preprocessingMode, *graphLoader, *outputPath, boost::numeric_cast<int>(*precisionLoss));
                } else {
                    createDM<dist_t>(*outputFormat, *preprocessingMode, *graphLoader, *outputPath, boost::numeric_cast<int>(*precisionLoss));
                }
            } else {
                throw input_error("Invalid method name '" + *method + "'.\n");
            }
            delete graphLoader;

        } else if (*command == "benchmark") {
            if (vm.count("help")) {
                printf("To run benchmarking, please provide required commands for the Benchmark command:\n"
                       "-m <method>\n--input-structure\n--query-set\n");
                return 0;
            }

            if (!method || !inputStructure || !querySet) {
                throw input_error("Missing one or more required options (-m <method> / --input-structure <path to structure file> / --query-set <path to query set file>) for the Benchmark command.\n");
            }

            std::unordered_map<std::string, std::function<double(std::string, std::string, std::string, std::string, bool)>> benchmarkMapFunctions = {
                    {"dijkstra", benchmarkDijkstraWithMapping},
                    {"astar", benchmarkAstarWithMapping},
                    {"ch", benchmarkCHwithMapping},
                    {"tnr", benchmarkTNRwithMapping},
                    {"tnraf", benchmarkTNRAFwithMapping},
                    {"dm", benchmarkDMwithMapping},
            };

            std::unordered_map<std::string, std::function<double(std::string, std::string, std::string, bool)>> benchmarkFunctions = {
                    {"dijkstra", benchmarkDijkstra},
                    {"astar", benchmarkAstar},
                    {"ch", benchmarkCH},
                    {"tnr", benchmarkTNR},
                    {"tnraf", benchmarkTNRAF},
                    {"dm", benchmarkDM},
            };

            if (!benchmarkFunctions.contains(*method)) {
                throw input_error("Invalid method '" + *method + "' for the Benchmark command.\n");
            }

            auto mem = Memory();
            mem.init();

            double totalTime;
            if (mappingFile) {
                auto func = benchmarkMapFunctions.at(*method);
                if (outputPath) {
                    totalTime = func(*inputStructure, *querySet, *mappingFile, *outputPath, true);
                } else {
                    totalTime = func(*inputStructure, *querySet, *mappingFile, "", false);
                }
            } else {
                auto func = benchmarkFunctions.at(*method);
                if (outputPath) {
                    totalTime = func(*inputStructure, *querySet, *outputPath, true);
                } else {
                    totalTime = func(*inputStructure, *querySet, "", false);
                }
            }

            std::ofstream output;
            output.open("benchmark.txt");
            output << totalTime << std::endl;
            output << mem.get_max_memory_usage() << std::endl;
            output.close();

        } else {
            throw input_error(INVALID_USAGE_INFO);
        }
    }
    catch (input_error& e) {
        std::cout << "Input Error: " << e.what();
        return 1;
    }
    catch (not_implemented_error& e) {
        std::cout << "Not Implemented Error: " << e.what();
        return 1;
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what();
        return 1;
    }

    return 0;
}
