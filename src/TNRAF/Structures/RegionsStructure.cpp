//
// Author: Xenty (Michal Cvach)
// Created on: 02.12.19
//

#include "RegionsStructure.h"

//______________________________________________________________________________________________________________________
RegionsStructure::RegionsStructure(unsigned int nodesCnt, unsigned int regionsCnt) : mapping(nodesCnt), regions(regionsCnt), regionsCnt(regionsCnt) {

}

// Allows us to assign a certain node to a certain region.
//______________________________________________________________________________________________________________________
void RegionsStructure::addNode(unsigned int nodeId, unsigned int regionId) {
    mapping[nodeId] = regionId;
    regions[regionId].push_back(nodeId);
}

// Returns a vector of nodes assigned to a certain region.
//______________________________________________________________________________________________________________________
vector < unsigned int > & RegionsStructure::nodesInRegion(unsigned int regionId) {
    return regions[regionId];
}

// Returns a region for a given node.
//______________________________________________________________________________________________________________________
unsigned int RegionsStructure::getRegion(unsigned int nodeId) {
    return mapping[nodeId];
}

// Returns the amount of regions we are working with.
//______________________________________________________________________________________________________________________
unsigned int RegionsStructure::getRegionsCnt() {
    return regionsCnt;
}