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

/******************************************************************************
 * File:             CsvGraphLoader.cpp
 *
 * Author:           Jan Horak (horakj31@fel.cvut.cz)
 * Created:          07/18/24
 *****************************************************************************/

#include "CsvGraphLoader.h"
#include "../../CLI/ProgressBar.hpp"
#include "../Structures/Graph.h"
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

    size_t from_col = SIZE_MAX, to_col = SIZE_MAX, cost_col = SIZE_MAX;
    auto header = edgeReader.header();
    size_t c = 0;
    for (const auto& cell : header) {
        std::string column_name;
        cell.read_value(column_name);
        if (column_name == "u") {
            from_col = c;
        } else if (column_name == "v") {
            to_col = c;
        } else if (column_name == "cost") {
            cost_col = c;
        }
        ++c;
    }

	if (from_col == SIZE_MAX || to_col == SIZE_MAX || cost_col == SIZE_MAX) {
		throw std::runtime_error("the 'edges.csv' file does not contain all columns 'u', 'v' and 'cost'.");
	}

	for (const auto& row: edgeReader) {
		unsigned int j = 0;
        unsigned int from = 0, to = 0;
		std::string cost_val;
        dist_t dist;

		for (const auto& cell: row) {
			std::string val;
			cell.read_value(val);

            if (j == from_col) from = boost::numeric_cast<unsigned int>(std::stoul(val));
            else if (j == to_col) to = boost::numeric_cast<unsigned int>(std::stoul(val));
            else if (j == cost_col) cost_val = val;
            ++j;
		}

		++progress;
		if (j == 0) continue; // empty row

		dist = parse_distance(cost_val, from, to, scaling_factor);

        if (dist != max)
            graph.addEdge(from, to, dist);
	}
}

void CsvGraphLoader::loadLocations(std::vector<std::pair<double, double>>& locations) {
    size_t id_col = 0, x_col = 0, y_col = 0;
    auto header = nodeReader.header();
    size_t c = 0;
    for (const auto& cell : header) {
        std::string column_name;
        cell.read_value(column_name);
        if (column_name == "id") {
            id_col = c;
        } else if (column_name == "x") {
            x_col = c;
        } else if (column_name == "y") {
            y_col = c;
        }
        ++c;
    }

    for (const auto& row: nodeReader) {
        unsigned int j = 0;
        unsigned int id = 0;
        double x = 0.0, y = 0.0;

        for (const auto& cell: row) {
            std::string val;
            cell.read_value(val);

            if (j == id_col) id = boost::numeric_cast<unsigned int>(std::stoul(val));
            else if (j == x_col) x = stod(val);
            else if (j == y_col) y = stod(val);
            ++j;
        }

        if (j == 0) continue; // empty row

        locations[id] = {x, y};
    }
}
