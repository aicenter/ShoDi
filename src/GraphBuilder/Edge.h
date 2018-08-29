//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#ifndef TRANSIT_NODE_ROUTING_EDGE_H
#define TRANSIT_NODE_ROUTING_EDGE_H


class Edge {
public:
    Edge(const unsigned int a, const long long unsigned int b, const bool c, const bool d);
    unsigned int targetNode;
    long long unsigned int weight;
    bool forward;
    bool backward;
};


#endif //TRANSIT_NODE_ROUTING_EDGE_H
