//
// Author: Xenty (Michal Cvach)
// Created on: 22.03.20
//

#include "TNRDistanceQueryManagerWithMapping.h"
#include "../GraphBuilding/Loaders/XenGraphLoader.h"

// Initializes the query manager. Here, the mapping from the original indices to our indices is loaded.
//______________________________________________________________________________________________________________________
TNRDistanceQueryManagerWithMapping::TNRDistanceQueryManagerWithMapping(TransitNodeRoutingGraph & g, string mappingFilepath) : qm(g) {
    XenGraphLoader mappingLoader(mappingFilepath);
    mappingLoader.loadNodesMapping(mapping);
}

// Used to find the shortest distance from source to target where source and target are the original indices.
//______________________________________________________________________________________________________________________
double TNRDistanceQueryManagerWithMapping::findDistance(const long long unsigned int source, const long long unsigned int target) {
    return qm.findDistance(mapping.at(source), mapping.at(target));
}