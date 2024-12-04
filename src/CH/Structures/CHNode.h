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

#ifndef TRANSIT_NODE_ROUTING_CHNODE_H
#define TRANSIT_NODE_ROUTING_CHNODE_H

/**
 * Auxiliary node structure which is used in the CHPriorityQueue(). Since the weight in the queue can be less than zero
 * (for example for a node where its degree is higher than the amount of shortcuts added after contracting that node)
 * signed integer is used for the weight here. This is the main difference against DijkstraNode(), where the weight
 * is unsigned because distance during Dijkstra run can only be positive (in our case)
 */
class CHNode {
public:
    /**
     * A simple constructor.

     * @param x[in] The ID of the node.
     * @param y[in] The priority of the node.
     */
    CHNode(const unsigned int x, const int y);

    unsigned int id;
    int weight;
private:
};


#endif //TRANSIT_NODE_ROUTING_CHNODE_H
