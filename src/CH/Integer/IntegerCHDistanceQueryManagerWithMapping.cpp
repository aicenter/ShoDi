//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#include "../../GraphBuilding/Loaders/IntegerXenGraphLoader.h"
#include "IntegerCHDistanceQueryManagerWithMapping.h"

//______________________________________________________________________________________________________________________
IntegerCHDistanceQueryManagerWithMapping::IntegerCHDistanceQueryManagerWithMapping(IntegerFlagsGraph & g, string mappingFilepath) : qm(g) {
    IntegerXenGraphLoader mappingLoader(mappingFilepath);
    mappingLoader.loadNodesMapping(mapping);
}

//______________________________________________________________________________________________________________________
double IntegerCHDistanceQueryManagerWithMapping::findDistance(const long long unsigned int source, const long long unsigned int target) {
    return qm.findDistance(mapping.at(source), mapping.at(target));
}