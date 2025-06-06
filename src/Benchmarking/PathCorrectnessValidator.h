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
// Created on: 28.10.19
//

#ifndef CONTRACTION_HIERARCHIES_PATHCORRECTNESSVALIDATOR_H
#define CONTRACTION_HIERARCHIES_PATHCORRECTNESSVALIDATOR_H

#include "../GraphBuilding/Structures/Graph.h"
#include "../TNR/TNRPathQueryManager.h"



/**
 * This class can check whether a path found using some more complex method is valid in the original graph.
 * Since Transit Node Routing or Contraction Hierarchies work with a graph that contains shortcut edges, we need to
 * ensure that those methods can also reconstruct the path in the original graph. This is only required if we are
 * interested in the actual shortest paths. Often we only need to know the shortest distance.
 */
class PathCorrectnessValidator {
public:
    /**
     * Validates that the paths reconstructed by the Transit Node Routing query algorithm are also valid in the original
     * graph.
     *
     * @param originalGraph[in] Instance of the graph that will be used for the validation.
     * @param tnrQueryManager[in] The Transit Node Routing query manager that will be used to obtain the paths that
     * will then be validated.
     * @param trips[in] The set of queries that will be used for the validation.
     */
    static void validateTNRPaths(
            Graph * originalGraph,
            TNRPathQueryManager & tnrQueryManager,
            std::vector< std::pair< unsigned int, unsigned int > > & trips);

    /**
     * Validates that the paths reconstructed by the Contraction Hierarchies query algorithm are also valid in the
     * original graph.
     *
     * @param originalGraph[in] Instance of the graph that will be used for the validation.
     * @param chQueryManager The Contraction Hierarchies query manager that will be used to obtain the paths that
     * will then be validated.
     * @param trips[in] The set of queries that will be used for the validation.
     */
    static void validateCHPaths(
            Graph * originalGraph,
            CHPathQueryManager & chQueryManager,
            std::vector< std::pair< unsigned int, unsigned int > > & trips);

private:
    /**
     * Auxiliary function used to validate one path reconstructed by some query algorithm.
     *
     * @param originalGraph[in] Instance of the graph that will be used for the validation.
     * @param distance[in] The distance returned by the query manager. Should be similar to the distance returned
     * by Dijkstra.
     * @param path[in] The path reconstructed by the complex method that needs to be validated. The path is represented
     * as a std::vector of edges.
     * @return Returns 'true' if the reconstructed path is also a valid path in the original graph, 'false' otherwise.
     */
    static bool validatePath(
            Graph * originalGraph,
            const dist_t distance,
            std::vector<SimpleEdge> & path);

    /**
     * Auxiliary function that checks if an edge exists in the original graph. This is used to check if edges obtained
     * during the reconstruction are actually valid in the original graph.
     *
     * @param from[in] The source node of the edge we are checking.
     * @param to[in] The target node of the edge we are checking.
     * @param originalGraph[in] The graph we are using for the check.
     * @return Returns 'true' if the edge also exists in the original graph, 'false' otherwise.
     */
    static unsigned int checkIfEdgeExists(
            const unsigned int from,
            const unsigned int to,
            Graph * originalGraph);
};


#endif //CONTRACTION_HIERARCHIES_PATHCORRECTNESSVALIDATOR_H
