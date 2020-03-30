//
// Author: Xenty (Michal Cvach)
// Created on: 22.03.20
//

#ifndef CONTRACTION_HIERARCHIES_TNRAFDISTANCEQUERYMANAGERWITHMAPPING_H
#define CONTRACTION_HIERARCHIES_TNRAFDISTANCEQUERYMANAGERWITHMAPPING_H

#include "TNRAFDistanceQueryManager.h"

using namespace std;

// Allows us to answer distance queries using the original indices of the nodes. Internally, nodes are indexed from
// 0 to n-1. Since in the original graph, the nodes indices might be any numbers, this class allows us to load a mapping
// from the original indices to our indices and then answer queries using the original indices. This uses the standard
// TNRDistanceQueryManager internally.
//______________________________________________________________________________________________________________________
class TNRAFDistanceQueryManagerWithMapping {
public:
    TNRAFDistanceQueryManagerWithMapping(TransitNodeRoutingArcFlagsGraph & g, string mappingFilepath);
    unsigned int findDistance(const long long unsigned int source, const long long unsigned int target);
private:
    TNRAFDistanceQueryManager qm;
    unordered_map<long long unsigned int, unsigned int> mapping;
};

#endif //CONTRACTION_HIERARCHIES_TNRAFDISTANCEQUERYMANAGERWITHMAPPING_H
