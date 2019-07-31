//
// Author: Xenty (Michal Cvach)
// Created on: 9.7.19
//

#include "FPointDistanceQueryManagerWithMappingAPI.h"
#include "../GraphBuilding/Loaders/DDSGFLoader.h"

//______________________________________________________________________________________________________________________
void FPointDistanceQueryManagerWithMappingAPI::initializeCH(string chFile, string mappingFile) {
    DDSGFLoader chLoader = DDSGFLoader(chFile);
    graph = chLoader.loadFlagsGraph();
    qm = new FPointCHDistanceQueryManagerWithMapping(*graph, mappingFile);
}

//______________________________________________________________________________________________________________________
double FPointDistanceQueryManagerWithMappingAPI::distanceQuery(long long unsigned int source, long long unsigned int target) {
    return qm -> findDistance(source, target);
}

//______________________________________________________________________________________________________________________
void FPointDistanceQueryManagerWithMappingAPI::clearStructures() {
    delete qm;
    delete graph;
}
