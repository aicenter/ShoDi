//
// Author: Xenty (Michal Cvach)
// Created on: 07.08.19
//

#ifndef CONTRACTION_HIERARCHIES_QUERYEDGEWITHUNPACKINGDATA_H
#define CONTRACTION_HIERARCHIES_QUERYEDGEWITHUNPACKINGDATA_H


#include <climits>

//______________________________________________________________________________________________________________________
class QueryEdgeWithUnpackingData {
public:
    QueryEdgeWithUnpackingData(const unsigned int a, const long long unsigned int b, const bool c, const bool d, const unsigned int mNode = UINT_MAX);
    QueryEdgeWithUnpackingData(const QueryEdgeWithUnpackingData & x);
    long long unsigned int weight;
    unsigned int targetNode;
    unsigned int middleNode;
    bool forward;
    bool backward;
};

#endif //CONTRACTION_HIERARCHIES_QUERYEDGEWITHUNPACKINGDATA_H
