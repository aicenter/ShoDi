//
// Author: Xenty (Michal Cvach)
// Created on: 2.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPRIORITYQUEUE_H
#define TRANSIT_NODE_ROUTING_CHPRIORITYQUEUE_H

#include <vector>
#include "CHNode.h"

using namespace std;

class CHpriorityQueue {
public:
    void printSomeInfo();
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
