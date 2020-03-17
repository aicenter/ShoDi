//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#include "../GraphBuilding/Loaders/XenGraphLoader.h"
#include "CHDistanceQueryManagerWithMapping.h"

//______________________________________________________________________________________________________________________
CHDistanceQueryManagerWithMapping::CHDistanceQueryManagerWithMapping(FlagsGraph & g, string mappingFilepath) : qm(g) {
    XenGraphLoader mappingLoader(mappingFilepath);
    mappingLoader.loadNodesMapping(mapping);
}

//______________________________________________________________________________________________________________________
double CHDistanceQueryManagerWithMapping::findDistance(const long long unsigned int source, const long long unsigned int target) {
    return qm.findDistance(mapping.at(source), mapping.at(target));
}