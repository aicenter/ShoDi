//
// Author: Xenty (Michal Cvach)
// Created on: 07.08.19
//

#ifndef CONTRACTION_HIERARCHIES_QUERYEDGEWITHUNPACKINGDATA_H
#define CONTRACTION_HIERARCHIES_QUERYEDGEWITHUNPACKINGDATA_H


#include <climits>

/**
 * Represents an edge in the graph in a way that makes sense for the Contraction Hierarchies query algorithm.
 * Similar to QueryEdge, but this class also contains additional information that can be utilised when searching for the
 * actual shortest path and not just the shortest distance.
 */
class QueryEdgeWithUnpackingData {
public:
    /**
     * A simple constructor.
     *
     * @param a[in] Target node of the edge.
     * @param b[in] Weight of the edge.
     * @param c[in] A forward flag for the edge (denotes if the edge is valid in the forward direction).
     * @param d[in] A backward flag for the edge (denotes if the edge is valid in the backward direction).
     * @param mNode[in] A middle node for the edge. If the edge is not a shortcut edge, it will be set to 'UINT_MAX'.
     */
    QueryEdgeWithUnpackingData(
            unsigned int a,
            unsigned int b,
            bool c,
            bool d,
            unsigned int mNode = UINT_MAX);

    /**
     * A constructor used to create a copy of an edge.
     *
     * @param x[in] Another instance of the class we want to create a copy of.
     */
    QueryEdgeWithUnpackingData(
            const QueryEdgeWithUnpackingData & x);

    unsigned int weight;
    unsigned int targetNode;
    unsigned int middleNode;
    bool forward;
    bool backward;
};

#endif //CONTRACTION_HIERARCHIES_QUERYEDGEWITHUNPACKINGDATA_H
