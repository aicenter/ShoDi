//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRDISTANCEQUERYMANAGER_H
#define CONTRACTION_HIERARCHIES_TNRDISTANCEQUERYMANAGER_H


#include "../GraphBuilding/Structures/IntegerStructures/TransitNodeRoutingGraph.h"
#include "../CH/Integer/IntegerCHDistanceQueryManager.h"

// This class handles distance queries over the TNR data structure (that means queries where we don't want to find the
// actual shortest path, but instead we are only interested in its distance).
//______________________________________________________________________________________________________________________
class TNRDistanceQueryManager {
public:
    TNRDistanceQueryManager(TransitNodeRoutingGraph & graph);
    unsigned int findDistance(const unsigned int source, const unsigned int target);
    void printQueriesAnalysis();
protected:
    TransitNodeRoutingGraph & graph;
    IntegerCHDistanceQueryManager fallbackCHmanager;
    unsigned int globalQueries;
    unsigned int localQueries;
};


#endif //CONTRACTION_HIERARCHIES_TNRDISTANCEQUERYMANAGER_H
