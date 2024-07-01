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
#include "GraphBuilding/Loaders/DistanceMatrixLoader.h"
#include "GraphBuilding/Loaders/TGAFLoader.h"
#include "Benchmarking/DijkstraBenchmark.h"
#include "Error/Error.h"
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
        unsigned int precisionLoss) {
    Timer timer("Contraction Hierarchies from DIMACS preprocessing");

    UpdateableGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, precisionLoss);

    timer.begin();
    CHPreprocessor::preprocessForDDSG(graph);
    timer.finish();

    graphLoader.loadGraph(graph, precisionLoss);
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
        unsigned int precisionLoss) {
    Timer timer("Transit Node Routing preprocessing (fast mode)");

    UpdateableGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, precisionLoss);

    timer.begin();
    CHPreprocessor::preprocessForDDSG(graph);
    timer.finish();

    graphLoader.loadGraph(graph, precisionLoss);

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
        unsigned int precisionLoss) {
    Timer timer("Transit Node Routing preprocessing (slow mode)");

    UpdateableGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, precisionLoss);
    Graph* originalGraph = graph.createCopy();

    timer.begin();
    CHPreprocessor::preprocessForDDSG(graph);
    timer.finish();

    graphLoader.loadGraph(graph, precisionLoss);

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
        GraphLoader& graphLoader,
        const std::string& outputFilePath,
        unsigned int precisionLoss) {
    Timer timer("Transit Node Routing preprocessing (using distance matrix)");

    UpdateableGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, precisionLoss);
    Graph* originalGraph = graph.createCopy();

    timer.begin();
    CHPreprocessor::preprocessForDDSG(graph);
    timer.finish();

    graphLoader.loadGraph(graph, precisionLoss);

    timer.begin();
    TNRPreprocessor::preprocessWithDMvalidation(
        graph, *originalGraph, outputFilePath, transitNodeSetSize);
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
        GraphLoader& graphLoader,
        const std::string& outputFilePath,
        unsigned int precisionLoss) {
    if (preprocessingMode == "fast") {
        createTNRFast(transitNodeSetSize, graphLoader, outputFilePath, precisionLoss);
    } else if (preprocessingMode == "slow") {
        createTNRSlow(transitNodeSetSize, graphLoader, outputFilePath, precisionLoss);
    } else if (preprocessingMode == "dm") {
        createTNRUsingDM(transitNodeSetSize, graphLoader, outputFilePath, precisionLoss);
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
        GraphLoader& graphLoader,
        const std::string& outputFilePath,
        unsigned int precisionLoss) {
	bool dm_mode;
    if (preprocessingMode == "slow") {
//        createTNRAFSlow(transitNodeSetSize, graphLoader, outputFilePath, precisionLoss);
		dm_mode = false;
    } else if (preprocessingMode == "dm") {
//        createTNRAFUsingDM(transitNodeSetSize, graphLoader, outputFilePath, precisionLoss);
		dm_mode = true;
    } else {
        throw input_error(std::string("Unknown preprocessing mode '") + preprocessingMode +
                          "' for Transit Node Routing with Arc Flags preprocessing.\n" + INVALID_FORMAT_INFO);
    }

	Timer timer("Transit Node Routing with Arc Flags preprocessing");

	UpdateableGraph graph(graphLoader.nodes());
	graphLoader.loadGraph(graph, precisionLoss);
	Graph* originalGraph = graph.createCopy();

	timer.begin();
	CHPreprocessor::preprocessForDDSG(graph);
	timer.finish();

	graphLoader.loadGraph(graph, precisionLoss);

	auto num_regions = std::min(graph.nodes(), 32u);

	timer.begin();
	TNRAFPreprocessor::preprocessUsingCH(
		graph, *originalGraph, outputFilePath, transitNodeSetSize,
		    num_regions, dm_mode);
	timer.finish();

	timer.printMeasuredTime();
}

template<typename ComputorType>
Distance_matrix_travel_time_provider* computeDistanceMatrix(GraphLoader &graphLoader, unsigned int precisionLoss, std::string timerName) {
    ComputorType computor;
    auto graph = computor.loadGraph(graphLoader, precisionLoss);

    Timer timer(timerName);

    timer.begin();
    computor.computeDistanceMatrix(graph);
    timer.finish();

    timer.printMeasuredTime();

    return computor.getDistanceMatrixInstance();
}

void createDM(
        const std::string& outputFormat,
        const std::string& preprocessingMode,
        GraphLoader& graphLoader,
        const std::string& outputFilePath,
        unsigned int precisionLoss) {
    std::function<Distance_matrix_travel_time_provider* (GraphLoader&, unsigned int, std::string)> computor;

    std::unique_ptr<DistanceMatrixOutputter> outputter{nullptr};
    std::unique_ptr<Distance_matrix_travel_time_provider> dm{nullptr};

    if (preprocessingMode == "slow") {
        computor = computeDistanceMatrix<DistanceMatrixComputorSlow>;
    } else if (preprocessingMode == "fast") {
        computor = computeDistanceMatrix<DistanceMatrixComputorFast>;
    } else {
        throw input_error(std::string("Unknown preprocessing mode '") + preprocessingMode +
                          "' for Distance Matrix preprocessing.\n" + INVALID_FORMAT_INFO);
    }

    if (outputFormat == "xdm") {
        outputter = std::unique_ptr<DistanceMatrixXdmOutputter> { new DistanceMatrixXdmOutputter()};
    } else if (outputFormat == "csv") {
        outputter = std::unique_ptr<DistanceMatrixCsvOutputter> { new DistanceMatrixCsvOutputter()};
    } else if (outputFormat == "hdf") {
        outputter = std::unique_ptr<DistanceMatrixHdfOutputter>{ new DistanceMatrixHdfOutputter() };
    } else {
        throw input_error(std::string("Unknown output type '") + outputFormat +
                          "' for Distance Matrix preprocessing.\n" + INVALID_FORMAT_INFO);
    }

    dm = std::unique_ptr<Distance_matrix_travel_time_provider> {computor(graphLoader, precisionLoss, "Distance Matrix preprocessing")};
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
 */
void benchmarkDijkstra(
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        char* distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    XenGraphLoader dijkstraGraphLoader(inputFilePath);
    Graph dijkstraGraph(dijkstraGraphLoader.nodes());
    dijkstraGraphLoader.loadGraph(dijkstraGraph, 1);

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
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& mappingFilePath,
        char* distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    XenGraphLoader dijkstraGraphLoader(inputFilePath);
    Graph dijkstraGraph(dijkstraGraphLoader.nodes());
    dijkstraGraphLoader.loadGraph(dijkstraGraph, 1);

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
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        char* distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader(inputFilePath);
    FlagsGraph* ch = chLoader.loadFlagsGraph();

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
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& mappingFilePath,
        char* distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    DDSGLoader chLoader = DDSGLoader(inputFilePath);
    FlagsGraph* ch = chLoader.loadFlagsGraph();

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
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        char* distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<unsigned int, unsigned int> > trips;
    tripsLoader.loadTrips(trips);

    TNRGLoader tnrLoader = TNRGLoader(inputFilePath);
    TransitNodeRoutingGraph* tnrGraph = tnrLoader.loadTNRforDistanceQueries();

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
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& mappingFilePath,
        char* distancesOutputPath = nullptr,
        bool outputDistances = false) {
    TripsLoader tripsLoader = TripsLoader(queriesFilePath);
    std::vector<std::pair<long long unsigned int, long long unsigned int> > trips;
    tripsLoader.loadLongLongTrips(trips);

    TNRGLoader tnrLoader = TNRGLoader(inputFilePath);
    TransitNodeRoutingGraph* tnrGraph = tnrLoader.loadTNRforDistanceQueries();

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
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        char* distancesOutputPath = nullptr,
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
        const std::string& inputFilePath,
        const std::string& queriesFilePath,
        const std::string& mappingFilePath,
        char* distancesOutputPath = nullptr,
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
}

/**
 * TODO
 */
GraphLoader *newGraphLoader(const std::string& inputFormat, const std::string& inputFilePath) {
    if (inputFormat == "xengraph") {
        return new XenGraphLoader(inputFilePath);
    } else if (inputFormat == "dimacs") {
        return new DIMACSLoader(inputFilePath);
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

    boost::optional<std::string> command, method, inputFormat, inputFile, outputFormat, outputFile, preprocessingMode,
    inputStructure, querySet, mappingFile;
    boost::optional<unsigned int> tnodesCnt, precisionLoss;

    // Declare the supported options.
    boost::program_options::options_description allOptions("Allowed options");
    allOptions.add_options()
            ("help", "produce help message")
            ("command", boost::program_options::value(&command))
            ("method,m", boost::program_options::value(&method))
            ("input-format,f", boost::program_options::value(&inputFormat))
            ("input-file,i", boost::program_options::value(&inputFile))
            ("output-format", boost::program_options::value(&outputFormat))
            ("output-file,o", boost::program_options::value(&outputFile))
            ("preprocessing-mode", boost::program_options::value(&preprocessingMode))
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

            if (!method || !inputFile) {
                throw input_error("Missing one or more required options (-m <method> / -i <input_file>) for the Create command.\n");
            }

            if (!inputFormat) {
                auto extension = std::filesystem::path(*inputFile).extension();

                if (extension == ".xeng") inputFormat.emplace("xengraph");
                else if (extension == ".gr") inputFormat.emplace("dimacs");
                else if (extension == ".csv") inputFormat.emplace("csv");
                else throw input_error("Unable to detect input file format. Please specify with '-f <format>'.");
            }

            if (!outputFile) {
                outputFile.emplace("out");
            }

            GraphLoader* graphLoader = newGraphLoader(*inputFormat, *inputFile);

            if (*method == "ch") {
                createCH(*graphLoader, *outputFile, *precisionLoss);
            } else if (*method == "tnr") {
                if (!preprocessingMode || !tnodesCnt) {
                    throw input_error("Missing one or more required options (--preprocessing-mode <fast/slow/dm> / --tnodes-cnt <cnt>) for TNR creation.\n");
                }
                createTNR(*preprocessingMode, *tnodesCnt, *graphLoader, *outputFile, *precisionLoss);
            } else if (*method == "tnraf") {
                if (!preprocessingMode || !tnodesCnt) {
                    throw input_error("Missing one or more required options (--preprocessing-mode <slow/dm> / --tnodes-cnt <cnt>) for TNRAF creation.\n");
                }
                createTNRAF(*preprocessingMode, *tnodesCnt, *graphLoader, *outputFile, *precisionLoss);
            } else if (*method == "dm") {
                if (!preprocessingMode || !outputFormat) {
                    throw input_error("Missing one or more required options (--preprocessing-mode <fast/slow> / --output-format <xdm/csv/hdf>) for DM creation.\n");
                }
                createDM(*outputFormat, *preprocessingMode, *graphLoader, *outputFile, *precisionLoss);
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

            std::unordered_map<std::string, std::function<void(std::string, std::string, std::string, char*, bool)>> benchmarkMapFunctions = {
                    {"dijkstra", benchmarkDijkstraWithMapping},
                    {"ch", benchmarkCHwithMapping},
                    {"tnr", benchmarkTNRwithMapping},
                    {"tnraf", benchmarkTNRAFwithMapping},
            };

            std::unordered_map<std::string, std::function<void(std::string, std::string, char*, bool)>> benchmarkFunctions = {
                    {"dijkstra", benchmarkDijkstra},
                    {"ch", benchmarkCH},
                    {"tnr", benchmarkTNR},
                    {"tnraf", benchmarkTNRAF},
            };

            if (!benchmarkFunctions.contains(*method)) {
                throw input_error("Invalid method '" + *method + "' for the Benchmark command.\n");
            }

            if (mappingFile) {
                auto func = benchmarkMapFunctions.at(*method);
                if (outputFile) {
                    func(*inputStructure, *querySet, *mappingFile, reinterpret_cast<char *>(*outputFile->c_str()), true);
                } else {
                    func(*inputStructure, *querySet, *mappingFile, nullptr, false);
                }
            } else {
                auto func = benchmarkFunctions.at(*method);
                if (outputFile) {
                    func(*inputStructure, *querySet, reinterpret_cast<char *>(*outputFile->c_str()), true);
                } else {
                    func(*inputStructure, *querySet, nullptr, false);
                }
            }
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
