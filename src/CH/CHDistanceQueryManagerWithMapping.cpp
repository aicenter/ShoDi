//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#include "../GraphBuilding/Loaders/XenGraphLoader.h"
#include "CHDistanceQueryManagerWithMapping.h"

// Initializes the query manager. Here, the mapping from the original indices to our indices is loaded.
//______________________________________________________________________________________________________________________
CHDistanceQueryManagerWithMapping::CHDistanceQueryManagerWithMapping(FlagsGraph & g, std::string mappingFilepath) : qm(g) {
    XenGraphLoader mappingLoader(mappingFilepath);
    mappingLoader.loadNodesMapping(mapping);
}

// Used to find the shortest distance from start to goal where start and goal are the original indices.
//______________________________________________________________________________________________________________________
unsigned int CHDistanceQueryManagerWithMapping::findDistance(const long long unsigned int start, const long long unsigned int goal) {
    return qm.findDistance(mapping.at(start), mapping.at(goal));
}