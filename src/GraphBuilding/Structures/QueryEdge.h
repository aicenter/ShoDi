//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#ifndef TRANSIT_NODE_ROUTING_EDGE_H
#define TRANSIT_NODE_ROUTING_EDGE_H

// This representation of an edge is used in the Contraction Hierarchies queries. How it works is already described
// in the 'FlagsGraph.h' file.
//______________________________________________________________________________________________________________________
class QueryEdge {
public:
    QueryEdge(const unsigned int a, const unsigned int b, const bool c, const bool d);
    QueryEdge(const QueryEdge & x);
    unsigned int weight;
    unsigned int targetNode;
    bool forward;
    bool backward;
};


#endif //TRANSIT_NODE_ROUTING_EDGE_H
