//
// Author: Xenty (Michal Cvach)
// Created on: 22.03.20
//

#include "TNRAFDistanceQueryManagerWithMapping.h"
#include "../GraphBuilding/Loaders/XenGraphLoader.h"

//______________________________________________________________________________________________________________________
TNRAFDistanceQueryManagerWithMapping::TNRAFDistanceQueryManagerWithMapping(TransitNodeRoutingArcFlagsGraph & g, string mappingFilepath) : qm(g) {
    XenGraphLoader mappingLoader(mappingFilepath);
    mappingLoader.loadNodesMapping(mapping);
}

//______________________________________________________________________________________________________________________
unsigned int TNRAFDistanceQueryManagerWithMapping::findDistance(const long long unsigned int start, const long long unsigned int goal) {
    return qm.findDistance(mapping.at(start), mapping.at(goal));
}