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
// Created on: 1.8.18
//

#ifndef TRANSIT_NODE_ROUTING_EDGEDIFFERENCEMANAGER_H
#define TRANSIT_NODE_ROUTING_EDGEDIFFERENCEMANAGER_H

#include <vector>



/**
 * Auxiliary class than can compute the weight of a node for the contraction order. There are multiple terms that we
 * take into account when computing difference. The most important is edge difference, which means how many shortcuts
 * will be added into the graph after contracting a node minus the node's degree - we prefer nodes, where we add less
 * shortcuts than was the original degree of the node. We also take contracted neighbours into consideration, a node
 * which has less contracted neighbours is more likely to be contracted soon than a node with a lot of contracted
 * neighbours. This slightly helps with uniformity. Other terms can be added to the weight computation in order to get
 * better preprocessing times or even better query times. Things like 'Voronoi Regions', 'Cost of Contraction' or 'Cost
 * of Queries' are all suggested as possible additional terms in the original article.
 */
class EdgeDifferenceManager {
public:
    /**
     * Initializes the manager. Does resize the internal structures in order to be able to compute differences later.
     *
     * @param nodes[in] The number of nodes in the graph.
     */
    static void init(
            const unsigned int nodes);

    /**
     * Computes the edge difference for a given node.
     *
     * @param contractedNode[in] The last contracted node.
     * @param x[in] The node we want to compute the difference for.
     * @param possibleShortcuts[in] The number of shortcuts that could be inserted into the graph if we were to contract
     * 'x' now.
     * @param degree[in] The current degree of 'x'.
     * @return Returns the edge difference for the node 'x'.
     */
    static int difference(
            const unsigned int contractedNode,
            const unsigned int x,
            const unsigned int possibleShortcuts,
            const unsigned int degree);

private:
    static std::vector<unsigned int> neighboursContracted;
    static std::vector<unsigned int> previousContracted;
};


#endif //TRANSIT_NODE_ROUTING_EDGEDIFFERENCEMANAGER_H
