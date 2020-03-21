//
// Author: Xenty (Michal Cvach)
// Created on: 07.08.19
//

#ifndef CONTRACTION_HIERARCHIES_QUERYEDGEWITHUNPACKINGDATA_H
#define CONTRACTION_HIERARCHIES_QUERYEDGEWITHUNPACKINGDATA_H


#include <climits>

// Represents an edge in the graph in a way that makes sense for the Contraction Hierarchies query algorithm.
// Similar to QueryEdge, but this class also contains additional information that can be utilised when searching for the
// actual shortest path and not just the shortest distance.
//______________________________________________________________________________________________________________________
class QueryEdgeWithUnpackingData {
public:
    QueryEdgeWithUnpackingData(const unsigned int a, const unsigned int b, const bool c, const bool d, const unsigned int mNode = UINT_MAX);
    QueryEdgeWithUnpackingData(const QueryEdgeWithUnpackingData & x);
    unsigned int weight;
    unsigned int targetNode;
    unsigned int middleNode;
    bool forward;
    bool backward;
};

#endif //CONTRACTION_HIERARCHIES_QUERYEDGEWITHUNPACKINGDATA_H
