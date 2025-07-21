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

#include <fstream>

#include "../Structures/Graph.h"
#include <boost/numeric/conversion/cast.hpp>
#include <limits>
#include <stdexcept>
#include <iostream>
#include <spdlog/spdlog.h>

#include "csv.h"
#include "inout.h"
#include "progress_bar.h"


CsvGraphLoader::CsvGraphLoader(const fs::path& input_path) :
	input_path(input_path),
	nodes_path(check_path(input_path / "nodes.csv")),
	edges_path(check_path(input_path / "edges.csv"))
{};


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
	spdlog::info("Counting nodes");

	// return boost::numeric_cast<unsigned int>(nodeReader.rows() - 1);
	std::ifstream in_stream(nodes_path);
	auto count = std::count_if(std::istreambuf_iterator<char>{in_stream}, {}, [](char c) { return c == '\n'; });
	--count;

	spdlog::info("Found {} nodes", count);

	return static_cast<unsigned>(count);
}

void CsvGraphLoader::loadGraph(BaseGraph& graph, int scaling_factor) {
	spdlog::info("Loading edges from {}", edges_path.string());

	io::CSVReader<3, io::trim_chars<>, io::no_quote_escape<'\t'>> edge_reader(edges_path.string());
	edge_reader.read_header(io::ignore_extra_column, "u", "v", "cost");
	unsigned int from;
	unsigned int to;
	size_t cost;
	unsigned counter = 0;
	constexpr unsigned progress_bar_step = 1'000'000;

	// count edges
	spdlog::info("Counting edges");
	std::ifstream in_stream(edges_path);
	auto count = std::count_if(std::istreambuf_iterator<char>{in_stream}, {}, [](char c) { return c == '\n'; });
	spdlog::info("Found {} edges", count);

	indicators::ProgressBar progress_bar{
		indicators::option::BarWidth{70},
		indicators::option::PostfixText{"Loading edges"},
		indicators::option::MaxProgress{count / progress_bar_step}
	};
	while(edge_reader.read_row(from, to, cost)){
		// 	dist = parse_distance(cost_val, from, to, scaling_factor);
		auto dist = (dist_t) std::floor(cost / scaling_factor);
		graph.addEdge(from, to, dist);
		++counter;
		if(counter % progress_bar_step == 0) {
			progress_bar.tick();
		}
	}
}

void CsvGraphLoader::loadLocations(std::vector<std::pair<double, double>>& locations) {
	io::CSVReader<3, io::trim_chars<>, io::no_quote_escape<'\t'>> node_reader(edges_path.string());
	node_reader.read_header(io::ignore_extra_column, "id", "x", "y");
	unsigned int id;
	double x;
	double y;
	unsigned counter = 0;
	constexpr unsigned progress_bar_step = 1'000'000;

	// count edges
	std::ifstream in_stream(edges_path);
	auto count = std::count_if(std::istreambuf_iterator<char>{in_stream}, {}, [](char c) { return c == '\n'; });

	indicators::ProgressBar progress_bar{
		indicators::option::BarWidth{70},
		indicators::option::PostfixText{"Loading nodes"},
		indicators::option::MaxProgress{count / progress_bar_step}
	};
	while(node_reader.read_row(id, x, y)){
		locations[id] = {x, y};
		++counter;
		if(counter % progress_bar_step == 0) {
			progress_bar.tick();
		}
    }
}
