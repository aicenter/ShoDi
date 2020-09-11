/******************************************************************************
 * File:             AdjMatrixGraph.h
 *
 * Author:           Jan Neumann (neumaja5@fel.cvut.cz)
 * Created:          09/11/20
 *****************************************************************************/

#ifndef ADJMATRIXGRAPH_H_8R9FHUCS
#define ADJMATRIXGRAPH_H_8R9FHUCS

#include "GraphBuilding/Structures/BaseGraph.h"
#include "constants.h"

#include <vector>

class AdjMatrixGraph : public BaseGraph {
private:
  std::vector<dist_t> mtx;
  unsigned int nodesAmount;

public:
  AdjMatrixGraph(unsigned int nodes);

  bool addEdge(unsigned int from, unsigned int to, dist_t weight) override;
  unsigned int nodes() const override;

  /**
   * Retrieve the adjacency matrix itself
   */
  std::vector<dist_t> matrix();

  ~AdjMatrixGraph() = default;
};

#endif /* end of include guard: ADJMATRIXGRAPH_H_8R9FHUCS */
