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
#pragma once
#include "../constants.h"
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <cstddef>
#include <vector>

namespace johnson {

  typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS,
                                boost::no_property,
                                boost::property<boost::edge_weight_t, dist_t>>
      JGraph;
  typedef boost::graph_traits<JGraph>::vertex_descriptor Vertex;
  typedef std::pair<unsigned int, unsigned int> Edge;

  typedef struct graph {
    size_t V;
    size_t E;
    std::vector<Edge> edge_array;
    std::vector<dist_t> weights;
  } graph_t;

  graph_t *johnson_init(const std::vector<dist_t>&);

  graph_t *johnson_init2(const unsigned int n, const double p,
                         const unsigned long seed);

  typedef struct edge {
    unsigned int u;
    unsigned int v;
  } edge_t;

  void free_graph(graph_t *g);

  template<class IntType>
  void johnson_parallel(graph_t *gr, IntType *output);

} // namespace johnson

#include "johnson.tpp"
