//
// Author: Xenty (Michal Cvach)
// Created on: 02.12.19
//

#include "RegionsStructure.h"

//______________________________________________________________________________________________________________________
RegionsStructure::RegionsStructure(unsigned int nodesCnt, unsigned int regionsCnt) : regions(regionsCnt), mapping(nodesCnt), regionsCnt(regionsCnt) {

}

//______________________________________________________________________________________________________________________
void RegionsStructure::addNode(unsigned int nodeId, unsigned int regionId) {
    mapping[nodeId] = regionId;
    regions[regionId].push_back(nodeId);
}

//______________________________________________________________________________________________________________________
std::vector < unsigned int > & RegionsStructure::nodesInRegion(unsigned int regionId) {
    return regions[regionId];
}

//______________________________________________________________________________________________________________________
unsigned int RegionsStructure::getRegion(unsigned int nodeId) {
    return mapping[nodeId];
}

//______________________________________________________________________________________________________________________
unsigned int RegionsStructure::getRegionsCnt() {
    return regionsCnt;
}