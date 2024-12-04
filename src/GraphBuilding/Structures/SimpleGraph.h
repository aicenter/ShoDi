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
// Created on: 20.8.18
//

#ifndef TRANSIT_NODE_ROUTING_SIMPLEGRAPH_H
#define TRANSIT_NODE_ROUTING_SIMPLEGRAPH_H

#include <vector>
#include <map>
#include "../../constants.h"

#include "BaseGraph.h"


/**
 * Class representing a simple graph - Simple graph is a graph where there only exists at most one edge between each
 * pair of nodes. Simple graph doesn't contain multiple edges (parallel edges). In our case, if there are more edges
 * than one between two nodes in the original graph, we only preserve the one with the lowest weight.
 */
class SimpleGraph : public BaseGraph {
private:
    std::vector<std::map<unsigned int, dist_t>> followingNodes;
public:
    /**
     * A simple constructor.
     *
     * @param n[in] The number of nodes in the graph.
     */
    SimpleGraph(unsigned int n);

    /**
     * Tries to add an edge into the graph. If the edge does not exist already, it is added. If the edge exists already,
     * its weight is checked, if the new weight would be lower, the weight is adjusted. This ensures that we always
     * only have one edge for each pair of nodes - we always keep the edge with the lowest weight.
     *
     * @param from[in] The source node of the edge.
     * @param to[in] The target node of the edge.
     * @param weight[in] The weight of the edge.
     */
    bool addEdge(unsigned int from, unsigned int to, dist_t weight) override;

    /**
     * Returns the amount of nodes in the graph.
     *
     * @return The amount of nodes in the graph.
     */
    unsigned int nodes() const override;

    /**
     * Returns all the edges in the graph that have 'x' as their source node.
     *
     * @param x[in] The node we are interested in.
     * @return A map containing all the edges in the graph that have 'x' as their source node along with their weights.
     */
    const std::map<unsigned int, dist_t> & edges(unsigned int x)const;

    bool handlesDuplicateEdges() override {
        return true;
    }

    ~SimpleGraph() = default;
};


#endif //TRANSIT_NODE_ROUTING_SIMPLEGRAPH_H
