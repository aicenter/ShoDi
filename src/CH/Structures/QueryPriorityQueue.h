//
// Author: Xenty (Michal Cvach)
// Created on: 27.8.18
//

#ifndef TRANSIT_NODE_ROUTING_QUERYPRIORITYQUEUE_H
#define TRANSIT_NODE_ROUTING_QUERYPRIORITYQUEUE_H

#include <vector>
#include <unordered_map>
#include "../../Dijkstra/DijkstraNode.h"

using namespace std;

class QueryPriorityQueue {
    public:
        //QueryPriorityQueue(const unsigned int n);
        QueryPriorityQueue();
        void push(const unsigned int x, const long long unsigned int y);
        void decreaseKey(const unsigned int x, const long long unsigned int y);
        DijkstraNode & top();
        void pop();
        bool empty();
        void clean();
        long unsigned int size();
        void printHeap();
    private:
        void bubbleDown(const unsigned int i);
        void bubbleUp(const unsigned int i);
        void swap(const unsigned int x, const unsigned int y);
        vector<DijkstraNode> content;
        //vector<unsigned int> mapping;
        unordered_map<unsigned int, unsigned int> mapping;
};


#endif //TRANSIT_NODE_ROUTING_QUERYPRIORITYQUEUE_H
