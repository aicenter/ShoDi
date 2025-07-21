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
 * File:             AdjGraphLoader.cpp
 *
 * Author:           Jan Neumann (neumaja5@fel.cvut.cz)
 * Created:          07/27/20
 *****************************************************************************/

#include "AdjGraphLoader.h"
#include "../../progress_bar.h"
#include "../Structures/Graph.h"
#include <boost/numeric/conversion/cast.hpp>
#include <limits>
#include <stdexcept>
#include <iostream>


AdjGraphLoader::AdjGraphLoader(const std::string& inputFile) : inputFile(inputFile) {
	if (!reader.mmap(inputFile)) {
		throw std::runtime_error(std::string("Error reading file ") + this->inputFile + " using mmap.\n");
	}
}

inline dist_t parse_distance(
	const std::string& str, unsigned int nodeFrom, unsigned int nodeTo, const std::string& inputFile, int scaling_factor
) {
	double val;
	try {
		val = stod(str);
	}
	catch (std::invalid_argument&) {
		std::cerr << "Warning: Found an unexpected value (" << str << ") in '" << inputFile
				  << "'. It will be interpreted as 'no edge' from node " << nodeFrom << " to node " << nodeTo << "."
				  << std::endl;
		return std::numeric_limits<dist_t>::max();
	}
	catch (std::out_of_range&) {
		std::cerr << "Warning: Found an out of range value (" << str << ") in '" << inputFile
				  << "'. It will be interpreted as 'no edge' from node " << nodeFrom << " to node " << nodeTo << "."
				  << std::endl;
		return std::numeric_limits<dist_t>::max();
	}

	if (std::isnan(val)) {
		return std::numeric_limits<dist_t>::max();
	}

	if (val < 0) {
		std::cerr << "Warning: Found a negative value (" << str << ") in '" << inputFile
				  << "'. It will be interpreted as 'no edge' from node " << nodeFrom << " to node " << nodeTo << "."
				  << std::endl;
		return std::numeric_limits<dist_t>::max();
	}

    return (dist_t) std::floor(val / scaling_factor);
}

unsigned int AdjGraphLoader::nodes() {
	return boost::numeric_cast<unsigned int>(reader.cols());
}

void AdjGraphLoader::loadGraph(BaseGraph& graph, int scaling_factor) {
	const unsigned int size = nodes();

	if (size != reader.rows())
		throw std::runtime_error(this->inputFile +
								 " does not contain a square matrix. Found " +
								 std::to_string(reader.rows()) + " rows and " +
								 std::to_string(size) + " cols.\n");

	const dist_t max = std::numeric_limits<dist_t>::max();
	unsigned counter = 0;
	constexpr unsigned progress_bar_step = 1000;
	
	indicators::ProgressBar progress_bar{
		indicators::option::BarWidth{70},
		indicators::option::PostfixText{"Loading CSV file"},
		indicators::option::MaxProgress{size / progress_bar_step}
	};

	unsigned int i = 0;
	//const double multiplier = std::pow(10, scaling_factor);
	for (const auto& row: reader) {
		unsigned int j = 0;
		for (const auto& cell: row) {
			std::string val;
			cell.read_value(val);
			const dist_t dist = parse_distance(val, i, j, inputFile, scaling_factor);
			if (dist != max)
				graph.addEdge(i, j, dist);
			++j;
		}
		++counter;
		if(counter % progress_bar_step == 0) {
			progress_bar.tick();
		}
		++i;
	}
}
