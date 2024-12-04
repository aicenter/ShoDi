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
// Created on: 2.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPRIORITYQUEUE_H
#define TRANSIT_NODE_ROUTING_CHPRIORITYQUEUE_H

#include <vector>
#include "CHNode.h"



/**
 * A simple implementation of a Min Binary Heap which is used as a priority queue for node contraction. This heap
 * implementation additionally supports changeValue() - a weight of a certain node can change after update (for example
 * when its neighbour was contracted) and the heap must be able to handle that. Faster heap could possibly improve the
 * performance of the CH preprocessing, for example Fibonacci Heap. The priority queue does not seem to be the
 * bottleneck currently though, as many to many shortest path finding takes much more time than the priority
 * queue management.
 */
class CHpriorityQueue {
public:
    /**
     * Constructor for the queue.
     *
     * @param nodes[in] The number of nodes in the graph.
     */
    CHpriorityQueue(const unsigned int nodes);

    /**
     * Inserts node 'x' into the queue with the priority 'y'.
     *
     * @param x[in] The node we want to insert.
     * @param y[in] The priority of the node we want to insert.
     */
    void insert(const unsigned int x, const int y);

    /**
     * Puts a node at the end of the queue. This does not ensure that the node will be at its correct position.
     * The function 'buildProperHeap' however ensures that after it all nodes will be at their correct positions.
     * We can therefore insert multiple nodes into the priority queue using 'pushOnly' and then only call
     * 'buildProperHeap' to make sure the order of nodes remain collect. This is faster than doing multiple inserts.
     *
     * @param x[in] The node we want to insert.
     * @param y[in] The priority of the node we want to insert.
     */
    void pushOnly(const unsigned int x, const int y);

    /**
     * Changes the priority of node 'x' to the value 'y'. If the priority changed causes the node to not be at its
     * correct position in the queue anymore, the node is automatically moved to its new correct position.
     *
     * @param x[in] The node whose priority we want to change.
     * @param y[in] The new priority of the node.
     */
    void changeValue(const unsigned int x, const int y);

    /**
     * Returns the element at the front of the queue (the node with the lowest priority).
     *
     * @return The element with the lowest priority in the queue.
     */
    CHNode front();

    /**
     * Removes the element at the front of the queue from the queue.
     */
    void pop();

    /**
     * Checks if the queue is empty.
     *
     * @return Returns 'true' if the queue is empty, 'false' if it contains at least one element.
     */
    bool empty();

    /**
     * Auxiliary function that will ensure that all elements in the queue are at their correct positions. This can be
     * used after multiple calls of 'pushOnly' to ensure that the newly added elements are moved to their correct
     * positions.
     */
    void buildProperHeap();

private:
    /**
     * Auxiliary function that bubbles down an element if its priority is too high for its current position.
     *
     * @param i[in] The node we want to bubble down.
     */
    void bubbleDown(const unsigned int i);

    /**
     * Auxiliary function that bubbles up an element if its priority is too low for its current position.
     *
     * @param i[in] The node we want to bubble up.
     */
    void bubbleUp(const unsigned int i);

    /**
     * Swaps the positions of two elements in the queue.
     *
     * @param x[in] Element 1 we are swapping.
     * @param y[in] Element 2 we are swapping.
     */
    void swap(const unsigned int x, const unsigned int y);

    std::vector<CHNode> content;
    std::vector<unsigned int> mapping;
};


#endif //TRANSIT_NODE_ROUTING_CHPRIORITYQUEUE_H
