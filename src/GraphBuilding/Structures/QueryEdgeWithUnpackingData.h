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
// Created on: 07.08.19
//

#ifndef CONTRACTION_HIERARCHIES_QUERYEDGEWITHUNPACKINGDATA_H
#define CONTRACTION_HIERARCHIES_QUERYEDGEWITHUNPACKINGDATA_H


#include <climits>

/**
 * Represents an edge in the graph in a way that makes sense for the Contraction Hierarchies query algorithm.
 * Similar to QueryEdge, but this class also contains additional information that can be utilised when searching for the
 * actual shortest path and not just the shortest distance.
 */
class QueryEdgeWithUnpackingData {
public:
    /**
     * A simple constructor.
     *
     * @param a[in] Target node of the edge.
     * @param b[in] Weight of the edge.
     * @param c[in] A forward flag for the edge (denotes if the edge is valid in the forward direction).
     * @param d[in] A backward flag for the edge (denotes if the edge is valid in the backward direction).
     * @param mNode[in] A middle node for the edge. If the edge is not a shortcut edge, it will be set to 'UINT_MAX'.
     */
    QueryEdgeWithUnpackingData(
            unsigned int a,
            unsigned int b,
            bool c,
            bool d,
            unsigned int mNode = UINT_MAX);

    /**
     * A constructor used to create a copy of an edge.
     *
     * @param x[in] Another instance of the class we want to create a copy of.
     */
    QueryEdgeWithUnpackingData(
            const QueryEdgeWithUnpackingData & x);

    unsigned int weight;
    unsigned int targetNode;
    unsigned int middleNode;
    bool forward;
    bool backward;
};

#endif //CONTRACTION_HIERARCHIES_QUERYEDGEWITHUNPACKINGDATA_H
