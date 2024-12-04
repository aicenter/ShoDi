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
// Created on: 6.8.18
//

#ifndef TRANSIT_NODE_ROUTING_HOPSDIJKSTRANODE_H
#define TRANSIT_NODE_ROUTING_HOPSDIJKSTRANODE_H

#include "../../Dijkstra/DijkstraNode.h"

/**
 * This structure is used during preprocessing for the Contraction Hierarchies. In addition to the classic 'nodeID' and
 * 'weight' we want to store 'hops' in the current path to the node. This is used to reduce search space during
 * many-to-many queries. We usually do not consider paths with more than 5 hops during the preprocessing phase.
 */
class HopsDijkstraNode : public DijkstraNode {
public:
    /**
     * A simple constructor.
     *
     * @param x[in] The ID of the node.
     * @param y[in] The distance to the node (weight for the priority queue).
     * @param z[in] The number of hops so far.
     */
    HopsDijkstraNode(unsigned int x, unsigned int y, unsigned int z);

    unsigned int hops;
};


#endif //TRANSIT_NODE_ROUTING_HOPSDIJKSTRANODE_H
