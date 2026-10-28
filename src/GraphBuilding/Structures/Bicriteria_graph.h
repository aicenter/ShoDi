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

#include <stdexcept>
#include <vector>

#include <spdlog/fmt/fmt.h>

#include "../../constants.h"

/**
 * Edge with a primary weight, which determines the shortest paths, and a secondary weight, which is only accumulated
 * along them.
 */
struct Bicriteria_edge {
	unsigned int target;
	dist_t primary;
	double secondary;
};

/**
 * Forward adjacency of a graph with two edge weights, used by the Bicriteria_distance_matrix_computor. Parallel edges
 * are kept: the lexicographic Dijkstra of the computor picks the best of them.
 */
class Bicriteria_graph {
public:
	explicit Bicriteria_graph(unsigned int nodes): following_nodes(nodes) {}

	void add_edge(unsigned int from, unsigned int to, dist_t primary, double secondary) {
		if(from >= following_nodes.size() || to >= following_nodes.size()) {
			throw std::out_of_range(fmt::format("Edge {} -> {} out of the node range 0..{}", from, to, following_nodes.size() - 1));
		}
		following_nodes[from].push_back({to, primary, secondary});
	}

	[[nodiscard]] const std::vector<Bicriteria_edge>& outgoing_edges(unsigned int node) const {
		return following_nodes[node];
	}

	[[nodiscard]] unsigned int nodes() const {
		return static_cast<unsigned int>(following_nodes.size());
	}

private:
	std::vector<std::vector<Bicriteria_edge>> following_nodes;
};
