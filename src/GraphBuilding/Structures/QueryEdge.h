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

#ifndef TRANSIT_NODE_ROUTING_EDGE_H
#define TRANSIT_NODE_ROUTING_EDGE_H

/**
 * This representation of an edge is used in the Contraction Hierarchies queries. How it works is already described
 * in the 'FlagsGraph.h' file.
 */
class QueryEdge {
public:
    /**
     * A simple constructor.
     *
     * @param a[in] Target node of the edge.
     * @param b[in] Weight of the edge.
     * @param c[in] A forward flag for the edge (denotes if the edge is valid in the forward direction).
     * @param d[in] A backward flag for the edge (denotes if the edge is valid in the backward direction).
     */
    QueryEdge(const unsigned int a, const unsigned int b, const bool c, const bool d);

    /**
     * A constructor used to create a copy of an edge.
     *
     * @param x[in] Another instance of the class we want to create a copy of.
     */
    QueryEdge(const QueryEdge & x);

    unsigned int weight;
    unsigned int targetNode;
    bool forward;
    bool backward;
};


#endif //TRANSIT_NODE_ROUTING_EDGE_H
