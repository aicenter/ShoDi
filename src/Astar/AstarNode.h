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
// Author: horakj31 (Jan Horak)
// Created on: 25.07.24
//

#ifndef ASTARNODE_H
#define ASTARNODE_H

/**
 * A simple structure used for the priority queue during the A* runs. We need both the 'nodeID' and 'weight'
 * which is the distance of the node from the source node at the current moment. We order the nodes in an ascending
 * order based on the weight.
 */
class AstarNode {
public:
    /**
     * A simple constructor.
     *
     * @param x[in] The ID of the node.
     * @param y[in] The distance to the node (weight for the priority queue).
     */
    AstarNode(unsigned int x, double y);

    unsigned int ID;
    double weight;

};


#endif //ASTARNODE_H
