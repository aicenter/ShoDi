/******************************************************************************
 * File:             CsvGraphLoader.cpp
 *
 * Author:           Jan Horak (horakj31@fel.cvut.cz)
 * Created:          07/18/24
 *****************************************************************************/

#include "CsvGraphLoader.h"
#include "CLI/ProgressBar.hpp"
#include "GraphBuilding/Structures/Graph.h"
#include <boost/numeric/conversion/cast.hpp>
#include <limits>
#include <stdexcept>
#include <iostream>


CsvGraphLoader::CsvGraphLoader(const std::string& inputPath) : inputPath(inputPath) {
	if (!nodeReader.mmap(inputPath + "/nodes.csv")) {
		throw std::runtime_error(std::string("Error reading file ") + this->inputPath
            + "/nodes.csv using mmap.\n");
	}

    if (!edgeReader.mmap(inputPath + "/edges.csv")) {
        throw std::runtime_error(std::string("Error reading file ") + this->inputPath
            + "/edges.csv using mmap.\n");
    }
}

inline dist_t parse_distance(
	const std::string& str, unsigned int nodeFrom, unsigned int nodeTo, int scaling_factor
) {
	double val;
	try {
		val = stod(str);
	}
	catch (std::invalid_argument&) {
		std::cerr << "Warning: Found an unexpected value (" << str << ") in 'edges.csv'. It will be interpreted"
                  << "as 'no edge' from node " << nodeFrom << " to node " << nodeTo << "." << std::endl;
		return std::numeric_limits<dist_t>::max();
	}
	catch (std::out_of_range&) {
		std::cerr << "Warning: Found an out of range value (" << str << ") in 'edges.csv'. It will be interpreted"
                  << "as 'no edge' from node " << nodeFrom << " to node " << nodeTo << "." << std::endl;
		return std::numeric_limits<dist_t>::max();
	}

	if (std::isnan(val)) {
		return std::numeric_limits<dist_t>::max();
	}

	if (val < 0) {
		std::cerr << "Warning: Found a negative value (" << str << ") in 'edges.csv'. It will be interpreted"
                  << "as 'no edge' from node " << nodeFrom << " to node " << nodeTo << "." << std::endl;
		return std::numeric_limits<dist_t>::max();
	}

    return (dist_t) std::floor(val / scaling_factor);
}

unsigned int CsvGraphLoader::nodes() {
	return boost::numeric_cast<unsigned int>(nodeReader.rows() - 1);
}

void CsvGraphLoader::loadGraph(BaseGraph& graph, int scaling_factor) {
	const dist_t max = std::numeric_limits<dist_t>::max();
	ProgressBar progress(edgeReader.rows(), "Loading CSV file:");

    size_t from_col = 0, to_col = 0, dist_col = 0;
    auto header = edgeReader.header();
    size_t c = 0;
    for (const auto& cell : header) {
        std::string column_name;
        cell.read_value(column_name);
        if (column_name == "u") {
            from_col = c;
        } else if (column_name == "v") {
            to_col = c;
        } else if (column_name == "length") {
            dist_col = c;
        }
        ++c;
    }

	for (const auto& row: edgeReader) {
		unsigned int j = 0;
        unsigned int from, to;
        dist_t dist;
		for (const auto& cell: row) {
			std::string val;
			cell.read_value(val);

            if (j == from_col) from = boost::numeric_cast<unsigned int>(std::stoul(val));
            else if (j == to_col) to = boost::numeric_cast<unsigned int>(std::stoul(val));
            else if (j == dist_col) dist = parse_distance(val, from, to, scaling_factor);
            ++j;
		}

        if (dist != max)
            graph.addEdge(from, to, dist);

		++progress;
	}
}
