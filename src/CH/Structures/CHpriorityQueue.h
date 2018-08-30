//
// Author: Xenty (Michal Cvach)
// Created on: 2.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPRIORITYQUEUE_H
#define TRANSIT_NODE_ROUTING_CHPRIORITYQUEUE_H

#include <vector>
#include "CHNode.h"

using namespace std;

// A simple implementation of a Min Binary Heap which is used as a priority queue for node contraction. This heap
// implementation additionally supports changeValue() - a weight of a certain node can change after update (for example
// when it's neighbourg was contracted) and the heap must be able to handle that. Faster heap could possibly improve the
// performance of the CH preprocessing, for example Fibonacci Heap. The priority queue doesn't seem to be the bottleneck
// currently though, as many to many shortest path finding takes much more time than the priority queue management.
//______________________________________________________________________________________________________________________
class CHpriorityQueue {
public:
    CHpriorityQueue(const unsigned int nodes);
    void insert(const unsigned int x, const int y);
    void pushOnly(const unsigned int x, const int y);
    void changeValue(const unsigned int x, const int y);
    CHNode front();
    void pop();
    bool empty();
    void buildProperHeap();
private:
    void bubbleDown(const unsigned int i);
    void bubbleUp(const unsigned int i);
    void swap(const unsigned int x, const unsigned int y);
    vector<CHNode> content;
    vector<unsigned int> mapping;
};


#endif //TRANSIT_NODE_ROUTING_CHPRIORITYQUEUE_H
