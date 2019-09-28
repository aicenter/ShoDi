//
// Author: Xenty (Michal Cvach)
// Created on: 20.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRPATHQUERYMANAGER_H
#define CONTRACTION_HIERARCHIES_TNRPATHQUERYMANAGER_H

#include "../GraphBuilding/Structures/IntegerStructures/TransitNodeRoutingGraph.h"
#include "../CH/Integer/IntegerCHDistanceQueryManager.h"
#include "../CH/Integer/IntegerCHPathQueryManager.h"

// This class handles Path queries over the TNR data structure (that means queries where we are interested in the actual
// shortest path and not just the shortest distance).
//______________________________________________________________________________________________________________________
class TNRPathQueryManager {
public:
    TNRPathQueryManager(TransitNodeRoutingGraph & graph);
    unsigned int findDistance(const unsigned int source, const unsigned int target);
    unsigned int quickFindDistance(const unsigned int source, const unsigned int target);
private:
    TransitNodeRoutingGraph & graph;
    IntegerCHDistanceQueryManager fallbackCHmanager;
};


#endif //CONTRACTION_HIERARCHIES_TNRPATHQUERYMANAGER_H
