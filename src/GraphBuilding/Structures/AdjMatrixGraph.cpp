/******************************************************************************
 * File:             AdjMatrixGraph.cpp
 *
 * Author:           Jan Neumann (neumaja5@fel.cvut.cz)
 * Created:          09/11/20
 *****************************************************************************/

#include "AdjMatrixGraph.h"
#include "constants.h"
#include <algorithm>
#include <limits>
#include <vector>

AdjMatrixGraph::AdjMatrixGraph(unsigned int nodes)
    : mtx((size_t)nodes * (size_t)nodes, std::numeric_limits<dist_t>::max()),
      nodesAmount(nodes) {}

bool AdjMatrixGraph::addEdge(unsigned int from, unsigned int to,
                             dist_t weight) {
  const size_t index = (size_t)from * (size_t)nodesAmount + (size_t)to;

  if (weight < mtx[index]) {
    mtx[index] = weight;
    return true;
  }

  return false;
}

unsigned int AdjMatrixGraph::nodes() const { return nodesAmount; }

std::vector<unsigned int> AdjMatrixGraph::matrix() { return std::move(mtx); }
