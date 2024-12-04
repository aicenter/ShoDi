/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

//
// Author: Xenty (Michal Cvach)
// Created on: 01.08.19
//

#include <fstream>
#include <iostream>
#include <functional>
#include <initializer_list>
#include <filesystem>
#include <boost/program_options.hpp>
#include <boost/optional/optional_io.hpp>
#include <tuple>
#include "DistanceMatrix/Distance_matrix_travel_time_provider.h"
#include "GraphBuilding/Loaders/TNRGLoader.h"
#include "GraphBuilding/Structures/UpdateableGraph.h"
#include "TNRAF/TNRAFPreprocessor.h"
#include "GraphBuilding/Loaders/TripsLoader.h"
#include "GraphBuilding/Loaders/DDSGLoader.h"
#include "Benchmarking/CHBenchmark.h"
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
#include "GraphBuilding/Loaders/CsvGraphLoader.h"
#include "DistanceMatrix/DistanceMatrixComputorSlow.h"


constexpr auto INVALID_FORMAT_INFO = "Please, make sure that your call has the right format. If not sure,\n"
									 "refer to 'README.md' for a complete overview of use cases for this application\n"
									 "with examples.\n";

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

	boost::optional<std::string> method, inputFormat, inputPath, outputFormat, outputPath, preprocessingMode,
		inputStructure, querySet, mappingFile;
	boost::optional<unsigned int> tnodesCnt, dmIntSize, precisionLoss;

	// Declare the supported options.
	boost::program_options::options_description allOptions("Allowed options");
	allOptions.add_options()
		("help", "produce help message")
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

	boost::program_options::variables_map vm;
	boost::program_options::store(
		boost::program_options::command_line_parser(argc, argv).options(allOptions).positional(p).run(), vm);
	boost::program_options::notify(vm);

	try {
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
