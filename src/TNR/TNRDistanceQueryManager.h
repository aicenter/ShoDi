//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRDISTANCEQUERYMANAGER_H
#define CONTRACTION_HIERARCHIES_TNRDISTANCEQUERYMANAGER_H


#include "../GraphBuilding/Structures/IntegerStructures/TransitNodeRoutingGraph.h"
#include "../CH/Integer/IntegerCHDistanceQueryManager.h"

class TNRDistanceQueryManager {
public:
    TNRDistanceQueryManager(TransitNodeRoutingGraph & graph);
    unsigned int findDistance(const unsigned int source, const unsigned int target);
private:
    TransitNodeRoutingGraph & graph;
    IntegerCHDistanceQueryManager fallbackCHmanager;
};


#endif //CONTRACTION_HIERARCHIES_TNRDISTANCEQUERYMANAGER_H
