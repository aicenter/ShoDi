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
 * File:             AdjMatrixGraph.cpp
 *
 * Author:           Jan Neumann (neumaja5@fel.cvut.cz)
 * Created:          09/11/20
 *****************************************************************************/

#include "AdjMatrixGraph.h"
#include "../../constants.h"
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

std::vector<dist_t> AdjMatrixGraph::matrix() { return std::move(mtx); }
