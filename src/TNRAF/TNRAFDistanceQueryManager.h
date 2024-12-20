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
// Created on: 02.12.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRAFDISTANCEQUERYMANAGER_H
#define CONTRACTION_HIERARCHIES_TNRAFDISTANCEQUERYMANAGER_H

#include "../GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.h"
#include "../TNR/TNRDistanceQueryManager.h"

/**
 * This class uses the Transit Node Routing with Arc Flags data structures to answer distance queries. This handles
 * queries where we are interested in the distance from some start to some goal, but we are not interested in the
 * actual path.
 */
class TNRAFDistanceQueryManager {
public:
    /**
     * Initializes the query manager.
     *
     * @param graph[in] The Transit Node Routing with Arc Flags data structure that will be used to answer queries.
     */
    explicit TNRAFDistanceQueryManager(
            TransitNodeRoutingArcFlagsGraph& graph);

    /**
     * Actually finds the distance between two targets. If start != goal, this function first invokes the locality
     * filter to determine whether the query is local. If it is local, we fallback to the Contraction Hierarchies query
     * algorithm. This is slightly slower, but the local queries already guarantee that the start and goal nodes will
     * be reasonably close to each other and in that case the query should take relatively short time. If the query
     * is not local, Transit Node Routing with Arc Flags data structure can be used to determine the shortest distance,
     * and in that case it is invoked to do so.
     *
     * @param start[in] The ID of the start node of the query.
     * @param goal[in] The ID of the goal node of the query.
     * @return Returns the shortest distance from start to goal in the graph or UINT_MAX if goal can not be reached
     * from start.
     */
    unsigned int findDistance(
            const unsigned int start,
            const unsigned int goal);

private:
    TransitNodeRoutingArcFlagsGraph& graph;
    CHDistanceQueryManager<NodeDataRegions> fallbackCHmanager;
};


#endif //CONTRACTION_HIERARCHIES_TNRAFDISTANCEQUERYMANAGER_H
