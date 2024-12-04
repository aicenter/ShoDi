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
// Created on: 28.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHFLAGSGRAPHQUERYMANAGER_H
#define TRANSIT_NODE_ROUTING_CHFLAGSGRAPHQUERYMANAGER_H


#include <vector>
#include <queue>
#include "../GraphBuilding/Structures/FlagsGraph.h"



/**
 * This class is responsible for the Contraction Hierarchies 'distance' queries - when we only require the 'distance'
 * between two points and do not care about the actual path.
 */
template <class T = NodeData>
class CHDistanceQueryManager {
public:
    /**
     * A simple constructor.
     *
     * @param g[in] The FlagsGraph instance we will be using to answer shortest distance queries.
     */
    CHDistanceQueryManager(FlagsGraph<T>& g);

    /**
     * We use the query algorithm that was described in the "Contraction Hierarchies: Faster and Simpler Hierarchical
     * Routing in Road Networks" article by Robert Geisberger, Peter Sanders, Dominik Schultes, and Daniel Delling.
     * Basically, the query is a modified bidirectional Dijkstra query, where from the start node we only expand
     * following nodes with higher contraction rank than the current node and from the goal we only expand previous
     * nodes with higher contraction rank than the current node. Both scopes will eventually meet in the node with the
     * highest contraction rank from all nodes in the path.
     *
     * @param start[in] The start node for the query.
     * @param goal[in] The goal node for the query.
     * @return Returns the shortest distance from start to goal or 'UINT_MAX' if goal is not reachable from start.
     */
    unsigned int findDistance(const unsigned int start, const unsigned int goal);

protected:
    /**
     * Reset information for the nodes that were changed in the current query. This is required so that following
     * queries are not influenced by the current query. Using this function ensures that following queries also
     * give correct results.
     */
    void prepareStructuresForNextQuery();

    FlagsGraph<T>& graph;
    unsigned int upperbound;
    std::vector<unsigned int> forwardChanged;
    std::vector<unsigned int> backwardChanged;
    std::vector<unsigned int> forwardStallChanged;
    std::vector<unsigned int> backwardStallChanged;
};

#include "CHDistanceQueryManager.tpp"

#endif //TRANSIT_NODE_ROUTING_CHFLAGSGRAPHQUERYMANAGER_H
