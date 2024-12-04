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
// Created on: 20.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRPATHQUERYMANAGER_H
#define CONTRACTION_HIERARCHIES_TNRPATHQUERYMANAGER_H

#include "../GraphBuilding/Structures/TransitNodeRoutingGraph.h"
#include "../CH/CHDistanceQueryManager.h"
#include "../CH/CHPathQueryManager.h"
#include "../GraphBuilding/Structures/TransitNodeRoutingGraphForPathQueries.h"
#include "../GraphBuilding/Structures/SimpleEdge.h"

/**
 * This class handles Path queries over the TNR data structure (that means queries where we are interested in the actual
 * shortest path and not just the shortest distance).
 */
class TNRPathQueryManager {
public:
    /**
     *
     * @param graph
     */
    TNRPathQueryManager(
            TransitNodeRoutingGraphForPathQueries & graph);

    /**
     * Actually finds the distance between two targets. If start != goal, this function first invokes the locality
     * filter to determine whether the query is local. If it is local, we fallback to the Contraction Hierarchies query
     * algorithm. This is slightly slower, but the local queries already guarantee that the start and goal nodes will
     * be reasonably close to each other and in that case the query should take relatively short time.
     * If the query is not local, Transit Node Routing structure can be used to determine the shortest distance,
     * and in that case it is invoked to do so.
     *
     * @param start
     * @param goal
     * @return
     */
    unsigned int findDistance(
            const unsigned int start,
            const unsigned int goal);

    /**
     *
     * @param start
     * @param goal
     * @param path
     * @return
     */
    unsigned int findPath(
            const unsigned int start,
            const unsigned int goal,
            std::vector<SimpleEdge> & path);

    /**
     *
     * @param start
     * @param goal
     * @return
     */
    unsigned int quickFindDistance(
            const unsigned int start,
            const unsigned int goal);
private:
    TransitNodeRoutingGraphForPathQueries & graph;
    CHPathQueryManager fallbackCHmanager;
};


#endif //CONTRACTION_HIERARCHIES_TNRPATHQUERYMANAGER_H
