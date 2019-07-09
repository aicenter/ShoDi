//
// Author: Xenty (Michal Cvach)
// Created on: 9.7.19
//

#include "../../GraphBuilding/Loaders/XenGraphLoader.h"
#include "FPointCHDistanceQueryManagerWithMapping.h"

//______________________________________________________________________________________________________________________
FPointCHDistanceQueryManagerWithMapping::FPointCHDistanceQueryManagerWithMapping(FPointFlagsGraph & g, string mappingFilepath) : qm(g) {
    XenGraphLoader mappingLoader(mappingFilepath);
    mappingLoader.loadNodesMapping(mapping);
}

//______________________________________________________________________________________________________________________
double FPointCHDistanceQueryManagerWithMapping::findDistance(const long long unsigned int source, const long long unsigned int target) {
    return qm.findDistance(mapping.at(source), mapping.at(target));
}