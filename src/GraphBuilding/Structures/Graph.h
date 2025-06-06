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
// Created on: 25.7.18
//

#ifndef TRANSIT_NODE_ROUTING_GRAPH_H
#define TRANSIT_NODE_ROUTING_GRAPH_H

#include <utility>
#include <vector>
#include "BaseGraph.h"
#include "SimpleGraph.h"
#include "../../constants.h"


/**
 * Class representing a graph.
 * This simple graph representation is used for the Dijkstra Algorithm. For the Contraction Hierarchies,
 * other representation seem to work better. This representation is also used at some points during the preprocessing
 * process for some methods using some modes.
 */
class Graph : public BaseGraph {
private:
    std::vector<std::vector<std::pair<unsigned int, dist_t>>> followingNodes;
    std::vector<std::vector<std::pair<unsigned int, dist_t>>> previousNodes;
public:
    /**
     * A simple constructor.
     *
     * @param n[in] The number of nodes in the graph.
     */
    Graph(unsigned int n);

    /**
     * A constructor that will initialize a Graph instance based on an instance of the SimpleGraph class.
     * SimpleGraph can be used to ensure we will not end up with a multi graph, but for Dijkstra, Graph is needed.
     *
     * @param x[in] An instance of the SimpleGraph class that will be used to initialize an instance of Graph.
     */
    Graph(SimpleGraph &x);

    /**
     * Adds an edge into the graph. This function does not validate that there is not another edge between the two
     * nodes already. In theory, the Graph class can also represent multi graphs. Since we do not want parallel edges
     * most of the time, it might make sense to start with an instance of SimpleGraph first and then transform it to
     * Graph later.
     *
     * @param from[in] Source node of the edge.
     * @param to[in] Target node of the edge.
     * @param weight[in] The weight of the edge.
     */
    bool addEdge(unsigned int from, unsigned int to, dist_t weight) override;

    /**
     * Returns the number of nodes in the graph.
     *
     * @return The number of nodes in the graph.
     */
    unsigned int nodes() const override;

    /**
     * Returns all the edges with the node 'x' as their target node.
     *
     * @param x[in] The target node we are interested in.
     * @return All the edges in the graph that have 'x' as their target node.
     */
    const std::vector<std::pair<unsigned int, dist_t>> &incomingEdges(const unsigned int x) const;

    /**
     * Returns all the edges with the node 'x' as their source node.
     *
     * @param x[in] The source node we are interested in.
     * @return All the edges in the graph that have 'x' as their source node.
     */
    const std::vector<std::pair<unsigned int, dist_t>> &outgoingEdges(const unsigned int x) const;

    bool handlesDuplicateEdges() override {
        return false;
    }

    ~Graph() = default;
};

#endif //TRANSIT_NODE_ROUTING_GRAPH_H
