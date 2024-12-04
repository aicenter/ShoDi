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
// Created on: 29.8.18
//

#ifndef TRANSIT_NODE_ROUTING_OUTPUTEDGE_H
#define TRANSIT_NODE_ROUTING_OUTPUTEDGE_H

/**
 * Simple structure used for outputing edges into the binary .ch format. Each instance is one edge with all it's
 * necessary information.
 */
class OutputEdge {
public:
    /**
     * A simple constructor.
     *
     * @param a[in] Source node for the edge.
     * @param b[in] Target node for the edge.
     * @param c[in] Weight of the edge.
     * @param d[in] Flags for the edge.
     */
    OutputEdge(unsigned int a, unsigned int b, unsigned int c, unsigned int d);

    unsigned int sourceNode;
    unsigned int targetNode;
    unsigned int weight;
    unsigned int flags;
};


#endif //TRANSIT_NODE_ROUTING_OUTPUTEDGE_H
