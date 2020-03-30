//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#include "../GraphBuilding/Loaders/XenGraphLoader.h"
#include "CHDistanceQueryManagerWithMapping.h"

// Initializes the query manager. Here, the mapping from the original indices to our indices is loaded.
//______________________________________________________________________________________________________________________
CHDistanceQueryManagerWithMapping::CHDistanceQueryManagerWithMapping(FlagsGraph & g, string mappingFilepath) : qm(g) {
    XenGraphLoader mappingLoader(mappingFilepath);
    mappingLoader.loadNodesMapping(mapping);
}

// Used to find the shortest distance from source to target where source and target are the original indices.
//______________________________________________________________________________________________________________________
unsigned int CHDistanceQueryManagerWithMapping::findDistance(const long long unsigned int source, const long long unsigned int target) {
    return qm.findDistance(mapping.at(source), mapping.at(target));
}