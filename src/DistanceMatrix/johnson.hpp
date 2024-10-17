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
