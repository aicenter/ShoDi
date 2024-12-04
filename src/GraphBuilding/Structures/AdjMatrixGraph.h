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
 * File:             AdjMatrixGraph.h
 *
 * Author:           Jan Neumann (neumaja5@fel.cvut.cz)
 * Created:          09/11/20
 *****************************************************************************/

#ifndef ADJMATRIXGRAPH_H_8R9FHUCS
#define ADJMATRIXGRAPH_H_8R9FHUCS

#include "BaseGraph.h"
#include "../../constants.h"

#include <vector>

class AdjMatrixGraph : public BaseGraph {
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

  bool handlesDuplicateEdges() override {
    return true;
  }

  ~AdjMatrixGraph() = default;
};

#endif /* end of include guard: ADJMATRIXGRAPH_H_8R9FHUCS */
