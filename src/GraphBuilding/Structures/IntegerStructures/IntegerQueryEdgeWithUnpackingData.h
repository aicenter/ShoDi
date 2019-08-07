//
// Author: Xenty (Michal Cvach)
// Created on: 07.08.19
//

#ifndef CONTRACTION_HIERARCHIES_INTEGERQUERYEDGEWITHUNPACKINGDATA_H
#define CONTRACTION_HIERARCHIES_INTEGERQUERYEDGEWITHUNPACKINGDATA_H


#include <climits>

//______________________________________________________________________________________________________________________
class IntegerQueryEdgeWithUnpackingData {
public:
    IntegerQueryEdgeWithUnpackingData(const unsigned int a, const long long unsigned int b, const bool c, const bool d, const unsigned int mNode = UINT_MAX);
    IntegerQueryEdgeWithUnpackingData(const IntegerQueryEdgeWithUnpackingData & x);
    long long unsigned int weight;
    unsigned int targetNode;
    unsigned int middleNode;
    bool forward;
    bool backward;
};

#endif //CONTRACTION_HIERARCHIES_INTEGERQUERYEDGEWITHUNPACKINGDATA_H
