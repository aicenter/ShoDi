//
// Author: Xenty (Michal Cvach)
// Created on: 9.7.19
//

#include "DistanceQueryManagerWithMappingAPI.h"
#include "../GraphBuilding/Loaders/DDSGFLoader.h"

//______________________________________________________________________________________________________________________
void DistanceQueryManagerWithMappingAPI::initializeCH(string chFile, string mappingFile) {
    DDSGFLoader chLoader = DDSGFLoader(chFile;
    graph = chLoader.loadFlagsGraph();
    qm = new FPointCHDistanceQueryManagerWithMapping(*graph, mappingFile);
}

//______________________________________________________________________________________________________________________
double DistanceQueryManagerWithMappingAPI::distanceQuery(unsigned int source, unsigned int target) {
    return qm -> findDistance(source, target);
}

//______________________________________________________________________________________________________________________
void DistanceQueryManagerWithMappingAPI::clearStructures() {
    delete qm;
    delete graph;
}