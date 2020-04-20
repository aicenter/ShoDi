//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#ifndef TRANSIT_NODE_ROUTING_EDGE_H
#define TRANSIT_NODE_ROUTING_EDGE_H

/**
 * This representation of an edge is used in the Contraction Hierarchies queries. How it works is already described
 * in the 'FlagsGraph.h' file.
 */
class QueryEdge {
public:
    /**
     * A simple constructor.
     *
     * @param a[in] Target node of the edge.
     * @param b[in] Weight of the edge.
     * @param c[in] A forward flag for the edge (denotes if the edge is valid in the forward direction).
     * @param d[in] A backward flag for the edge (denotes if the edge is valid in the backward direction).
     */
    QueryEdge(const unsigned int a, const unsigned int b, const bool c, const bool d);

    /**
     * A constructor used to create a copy of an edge.
     *
     * @param x[in] Another instance of the class we want to create a copy of.
     */
    QueryEdge(const QueryEdge & x);

    unsigned int weight;
    unsigned int targetNode;
    bool forward;
    bool backward;
};


#endif //TRANSIT_NODE_ROUTING_EDGE_H
