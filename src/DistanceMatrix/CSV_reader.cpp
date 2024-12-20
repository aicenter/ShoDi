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

#if defined(_MSC_VER)
	#pragma warning(disable:4706)
	#define NOMINMAX // prevents the min and max macro definitions from windows.h, which are introduced in p-ranav-csv2
#endif
#include <csv2/reader.hpp>
#include <iostream>
#include <filesystem>

#include "crack_atof.cpp"
#include "../progress_bar.h"

#if defined(_MSC_VER)
	#pragma warning(default:4706)
#endif

#include "CSV_reader.h"

dist_t parse_distance(
	const std::string& str, 
	unsigned int nodeFrom, 
	unsigned int nodeTo, const std::string& inputFile, 
	double scaling_factor) {
	double val;
	try {
		val = crack_atof(str.c_str(), str.c_str() + str.size());
	}
	catch (std::invalid_argument&) {
		std::cerr << "Warning: Found an unexpected value (" << str << ") in '" << inputFile << "'. It will be interpreted as 'no edge' from node " << nodeFrom << " to node " << nodeTo << "." << std::endl;
		return std::numeric_limits<dist_t>::max();
	}
	catch (std::out_of_range&) {
		std::cerr << "Warning: Found an out of range value (" << str << ") in '" << inputFile << "'. It will be interpreted as 'no edge' from node " << nodeFrom << " to node " << nodeTo << "." << std::endl;
		return std::numeric_limits<dist_t>::max();
	}

	if (std::isnan(val)) {
		return std::numeric_limits<dist_t>::max();
	}

	if (val < 0) {
		std::cerr << "Warning: Found a negative value (" << str << ") in '" << inputFile << "'. It will be interpreted as 'no edge' from node " << nodeFrom << " to node " << nodeTo << "." << std::endl;
		return std::numeric_limits<dist_t>::max();
	}

	return static_cast<dist_t>(round(val / (double)scaling_factor));
}

std::pair<std::unique_ptr<dist_t[]>, unsigned> CSV_reader::read_matrix(const std::string& dm_filepath) {
	csv2::Reader<csv2::delimiter<','>, csv2::quote_character<'"'>, csv2::first_row_is_header<false>,
		csv2::trim_policy::trim_characters<' ', '\t', '\r', '\n'>> reader;

	if(!std::filesystem::exists(dm_filepath)) {
		throw std::runtime_error(std::string("Distance matrix file does not exists: ") + dm_filepath + "\n");
	}

	if (!reader.mmap(dm_filepath)) {
		throw std::runtime_error(std::string("Error reading file ") + dm_filepath + " using mmap.\n");
	}

	const auto size = static_cast<unsigned>(reader.cols());
	if (size != reader.rows())
		throw std::runtime_error(dm_filepath + " does not contain a square matrix. Found " +
			std::to_string(reader.rows()) + " rows and " + std::to_string(size) + " cols.\n");
	auto dm = std::make_unique<dist_t[]>(size * size);
	unsigned int i = 0;

	constexpr unsigned int step = 200;

	indicators::ProgressBar bar{
		indicators::option::BarWidth{70},
		indicators::option::PostfixText{"Loading Distance Matrix"},
		indicators::option::MaxProgress{size / step}
	};
	
	for (const auto& row: reader) {
		unsigned int j = 0;
		for (const auto& cell: row) {
			std::string val;
			/*cell.read_value(val);*/
			cell.read_raw_value(val);
			const dist_t dist = parse_distance(val, i, j, dm_filepath, (double)1);
			dm[i * size + j] = dist;
			++j;
		}
		if ((i % step) == 0) {
			bar.tick();
		}
		++i;
	}

	return {std::move(dm), size};
}


