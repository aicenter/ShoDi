//
// Author: Xenty (Michal Cvach)
// Created on: 20.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRPATHQUERYMANAGER_H
#define CONTRACTION_HIERARCHIES_TNRPATHQUERYMANAGER_H

#include "../GraphBuilding/Structures/TransitNodeRoutingGraph.h"
#include "../CH/CHDistanceQueryManager.h"
#include "../CH/CHPathQueryManager.h"
#include "../GraphBuilding/Structures/TransitNodeRoutingGraphForPathQueries.h"
#include "../GraphBuilding/Structures/SimpleEdge.h"

// This class handles Path queries over the TNR data structure (that means queries where we are interested in the actual
// shortest path and not just the shortest distance).
//______________________________________________________________________________________________________________________
class TNRPathQueryManager {
public:
    TNRPathQueryManager(TransitNodeRoutingGraphForPathQueries & graph);
    unsigned int findDistance(const unsigned int source, const unsigned int target);
    unsigned int findPath(const unsigned int source, const unsigned int target, vector<SimpleEdge> & path);
    unsigned int quickFindDistance(const unsigned int source, const unsigned int target);
private:
    TransitNodeRoutingGraphForPathQueries & graph;
    CHPathQueryManager fallbackCHmanager;
};


#endif //CONTRACTION_HIERARCHIES_TNRPATHQUERYMANAGER_H
