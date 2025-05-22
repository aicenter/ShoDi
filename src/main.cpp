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

#include <iostream>
#include <initializer_list>
#include <memory>
#include <filesystem>
#include <boost/program_options.hpp>
#include <boost/optional/optional_io.hpp>
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
#include "Benchmarking/memory.h"
#include "Error/Error.h"
#include "GraphBuilding/Loaders/AdjGraphLoader.h"
#include "GraphBuilding/Loaders/CsvGraphLoader.h"
#include "DistanceMatrix/DistanceMatrixComputorSlow.h"
#include "DistanceMatrix/DistanceMatrixOutputter.h"
#include "DistanceMatrix/DistanceMatrixXdmOutputter.h"
#include "DistanceMatrix/DistanceMatrixCsvOutputter.h"
#include "DistanceMatrix/DistanceMatrixHdfOutputter.h"
#include "TNRAF/TNRAFPreprocessingMode.h"

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
	TNRAFPreprocessingMode mode;
    if (preprocessingMode == "slow") {
//        createTNRAFSlow(transitNodeSetSize, graphLoader, outputFilePath, scaling_factor);
		mode = TNRAFPreprocessingMode::SLOW;
    } else if (preprocessingMode == "dm") {
//        createTNRAFUsingDM(transitNodeSetSize, graphLoader, outputFilePath, scaling_factor);
		mode = TNRAFPreprocessingMode::DM;
    } else if (preprocessingMode == "fast") {
        mode = TNRAFPreprocessingMode::FAST;
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
		    num_regions, dmIntSize, mode);
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
	try {
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

		boost::program_options::variables_map vm;
		boost::program_options::store(
				boost::program_options::command_line_parser(argc, argv).options(allOptions).run(), vm);
		boost::program_options::notify(vm);

		try {
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
	catch (...) {
		const std::exception_ptr& eptr = std::current_exception();
		if (!eptr) {
			throw std::bad_exception();
		}

		/*char* message;*/
		std::string message;
		try {
			std::rethrow_exception(eptr);
		}
		catch (const std::exception& e) {
			message = e.what();
		}
		catch (const std::string& e) {
			message = e;
		}
		catch (const char* e) {
			message = e;
		}
		catch (...) {
			message = "Unknown error";
		}

		std::cout << message;

		return -1;
	}
}
