//
// Author: Xenty (Michal Cvach)
// Created on: 02.12.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRAFDISTANCEQUERYMANAGER_H
#define CONTRACTION_HIERARCHIES_TNRAFDISTANCEQUERYMANAGER_H

#include "../GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.h"
#include "../TNR/TNRDistanceQueryManager.h"

// This class uses the Transit Node Routing with Arc Flags data structures to answer distance queries. This handles
// queries where we are interested in the distance from some start to some goal, but we are not interested in the
// actual path.
//______________________________________________________________________________________________________________________
class TNRAFDistanceQueryManager {
public:
    TNRAFDistanceQueryManager(TransitNodeRoutingArcFlagsGraph & graph);
    unsigned int findDistance(const unsigned int source, const unsigned int target);
    void printQueriesAnalysis();
private:
    TransitNodeRoutingArcFlagsGraph & graph;
    CHDistanceQueryManager fallbackCHmanager;
    unsigned int globalQueries;
    unsigned int localQueries;
};


#endif //CONTRACTION_HIERARCHIES_TNRAFDISTANCEQUERYMANAGER_H
