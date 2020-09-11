/******************************************************************************
 * File:             BaseGraph.h
 *
 * Author:           Jan Neumann (neumaja5@fel.cvut.cz)
 * Created:          09/11/20
 *****************************************************************************/

#ifndef BASEGRAPH_H_IQX7DWCB
#define BASEGRAPH_H_IQX7DWCB

#include "constants.h"

/**
 * Base class for all Graph classes.
 * Allows us to not have to write the same functions over and over.
 */
class BaseGraph {
public:
  /**
   * Adds an edge into the graph.
   *
   * @param from[in] Source node of the edge.
   * @param to[in] Target node of the edge.
   * @param weight[in] The weight of the edge.
   * @return Returns true if the edge was actually inserted into the graph.
   * This is only applicable in some cases. In other, the result is always true.
   */
  virtual bool addEdge(unsigned int from, unsigned int to, dist_t weight) = 0;

  /**
   * Returns the amount of nodes in the graph.
   *
   * @return The amount of nodes in the graph.
   */
  virtual unsigned int nodes() const = 0;

  virtual ~BaseGraph() = default;
};

#endif /* end of include guard: BASEGRAPH_H_IQX7DWCB */
