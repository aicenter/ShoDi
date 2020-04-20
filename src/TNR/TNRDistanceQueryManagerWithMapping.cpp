//
// Author: Xenty (Michal Cvach)
// Created on: 22.03.20
//

#include "TNRDistanceQueryManagerWithMapping.h"
#include "../GraphBuilding/Loaders/XenGraphLoader.h"

//______________________________________________________________________________________________________________________
TNRDistanceQueryManagerWithMapping::TNRDistanceQueryManagerWithMapping(TransitNodeRoutingGraph & g, string mappingFilepath) : qm(g) {
    XenGraphLoader mappingLoader(mappingFilepath);
    mappingLoader.loadNodesMapping(mapping);
}

//______________________________________________________________________________________________________________________
unsigned int TNRDistanceQueryManagerWithMapping::findDistance(const long long unsigned int start, const long long unsigned int goal) {
    return qm.findDistance(mapping.at(start), mapping.at(goal));
}