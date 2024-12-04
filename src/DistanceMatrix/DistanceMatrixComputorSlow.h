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

//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#ifndef CONTRACTION_HIERARCHIES_DISTANCEMATRIXCOMPUTOR_H
#define CONTRACTION_HIERARCHIES_DISTANCEMATRIXCOMPUTOR_H

#include "../GraphBuilding/Structures/Graph.h"
#include "DistanceMatrixComputor.h"
#include <cstddef>


template <class IntType>
class DistanceMatrixComputorSlow : public DistanceMatrixComputor<IntType> {
public:

    Graph loadGraph(GraphLoader& graphLoader, int scaling_factor);

    std::unique_ptr<IntType[]> compute_and_get_distance_matrix(GraphLoader& graphLoader, int scaling_factor) override;

    /**
     * this function will compute the full distance matrix for the given graph.
     * the matrix will be stored in the instance of the DistanceMatrixComputor class,
     * so that it can be output or used further.
     *
     * @param graph[in] the graph for which we want to compute the distance matrix.
     */
    void computeDistanceMatrix(const Graph& graph);

    /**
     * This function will compute the full distance matrix for the given graph as if the directions for all the edges
     * were switched. This is useful during the preprocessing phase of some of the methods.
     *
     * @param graph[in] The graph for which we want to compute the distance matrix. (The graph should be as normal,
     * not reversed, the 'reversion' is done inside this function.)
     */
    void computeDistanceMatrixInReversedGraph(const Graph& graph);

private:
    /**
     * // This function will compute one row of the full distance matrix. This is done by running a simple Dijkstra from the
// node corresponding to the row, which is not stopped until all reachable nodes have been visited. The distances
// to all the other nodes found by this Dijkstra run are then used as values for the row.
     *
     * @param rowID
     * @param graph
     * @param useReversedGraph
     */
    void fillDistanceMatrixRow(unsigned int rowID, const Graph& graph, bool useReversedGraph = false);

};

#include "DistanceMatrixComputorSlow.tpp"

#endif //CONTRACTION_HIERARCHIES_DISTANCEMATRIXCOMPUTOR_H
