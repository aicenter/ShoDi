/*
 * MIT License
 *
 * Copyright (c) 2026 Czech Technical University in Prague
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
#pragma once

#include <atomic>
#include <cmath>
#include <cstdint>
#include <exception>
#include <limits>
#include <memory>
#include <queue>
#include <vector>

#include <boost/numeric/conversion/cast.hpp>
#include <spdlog/spdlog.h>

#include "../GraphBuilding/Structures/Bicriteria_graph.h"
#include "../progress_bar.h"

/** Element type of the secondary distance matrix. */
using secondary_dist_t = uint_least32_t;

/**
 * Computes two distance matrices in one pass: the primary matrix of the shortest paths by the primary edge weight, and
 * the secondary matrix of the secondary weight accumulated along those paths. Ties in the primary weight are broken by
 * the smaller secondary weight. The secondary weight is accumulated in double precision and rounded once per matrix
 * cell. Unreachable cells hold the maximum of the element type, as in DistanceMatrixComputorSlow.
 *
 * Each row is one Dijkstra run with lexicographic (primary, secondary) labels; the rows are computed in parallel.
 */
template<class IntType>
class Bicriteria_distance_matrix_computor {
public:
	void compute(const Bicriteria_graph& graph) {
		size = graph.nodes();
		const auto cells = static_cast<size_t>(size) * size;
		primary_table = std::make_unique<IntType[]>(cells);
		secondary_table = std::make_unique<secondary_dist_t[]>(cells);

		spdlog::info("Computing the primary and secondary distance matrices for {} nodes", size);
		constexpr unsigned progress_bar_step = 1000;
		indicators::ProgressBar progress_bar{
			indicators::option::BarWidth{70},
			indicators::option::PostfixText{"Computing distance matrix rows"},
			indicators::option::MaxProgress{size / progress_bar_step}
		};
		std::atomic<unsigned> finished_rows{0};
		// exceptions cannot leave an OpenMP region: the first one is rethrown after the loop
		std::exception_ptr exception;

		#pragma omp parallel for schedule(dynamic)
		for(int row = 0; row < static_cast<int>(size); ++row) {
			try {
				fill_row(static_cast<unsigned>(row), graph);
			}
			catch(...) {
				#pragma omp critical
				if(!exception) {
					exception = std::current_exception();
				}
			}
			if(++finished_rows % progress_bar_step == 0) {
				#pragma omp critical
				progress_bar.tick();
			}
		}
		if(exception) {
			std::rethrow_exception(exception);
		}
		spdlog::info("Distance matrices computed");
	}

	std::unique_ptr<IntType[]> get_primary_matrix() {
		return std::move(primary_table);
	}

	std::unique_ptr<secondary_dist_t[]> get_secondary_matrix() {
		return std::move(secondary_table);
	}

	[[nodiscard]] unsigned int get_size() const {
		return size;
	}

private:
	struct Label {
		unsigned int node;
		uint64_t primary;
		double secondary;
	};

	std::unique_ptr<IntType[]> primary_table;
	std::unique_ptr<secondary_dist_t[]> secondary_table;
	unsigned int size = 0;

	void fill_row(unsigned int row, const Bicriteria_graph& graph) const {
		constexpr uint64_t unreached = std::numeric_limits<uint64_t>::max();
		std::vector<uint64_t> primary(size, unreached);
		std::vector<double> secondary(size, 0);

		auto worse = [](const Label& a, const Label& b) {
			return a.primary > b.primary || (a.primary == b.primary && a.secondary > b.secondary);
		};
		std::priority_queue<Label, std::vector<Label>, decltype(worse)> queue(worse);
		primary[row] = 0;
		queue.push({row, 0, 0});

		while(!queue.empty()) {
			const Label current = queue.top();
			queue.pop();
			if(current.primary != primary[current.node] || current.secondary != secondary[current.node]) {
				continue; // stale entry
			}
			for(const auto& edge: graph.outgoing_edges(current.node)) {
				const uint64_t new_primary = current.primary + edge.primary;
				const double new_secondary = current.secondary + edge.secondary;
				if(new_primary < primary[edge.target]
					|| (new_primary == primary[edge.target] && new_secondary < secondary[edge.target])) {
					primary[edge.target] = new_primary;
					secondary[edge.target] = new_secondary;
					queue.push({edge.target, new_primary, new_secondary});
				}
			}
		}

		const size_t offset = static_cast<size_t>(row) * size;
		for(size_t i = 0; i < size; ++i) {
			if(primary[i] == unreached) {
				primary_table[offset + i] = std::numeric_limits<IntType>::max();
				secondary_table[offset + i] = std::numeric_limits<secondary_dist_t>::max();
			}
			else {
				primary_table[offset + i] = boost::numeric_cast<IntType>(primary[i]);
				secondary_table[offset + i] = boost::numeric_cast<secondary_dist_t>(std::llround(secondary[i]));
			}
		}
	}
};
