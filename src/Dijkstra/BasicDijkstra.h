//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_BASICDIJKSTRA_H
#define TRANSIT_NODE_ROUTING_BASICDIJKSTRA_H


#include "../GraphBuilder/Graph.h"

class BasicDijkstra {
public:
    static long long unsigned int run(const unsigned int source, const unsigned int target, const Graph & graph);
};


#endif //TRANSIT_NODE_ROUTING_BASICDIJKSTRA_H
