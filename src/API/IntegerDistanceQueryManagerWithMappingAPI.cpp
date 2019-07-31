//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#include "IntegerDistanceQueryManagerWithMappingAPI.h"
#include "../GraphBuilding/Loaders/DDSGLoader.h"

//______________________________________________________________________________________________________________________
void IntegerDistanceQueryManagerWithMappingAPI::initializeCH(string chFile, string mappingFile) {
    DDSGLoader chLoader = DDSGLoader(chFile);
    graph = chLoader.loadFlagsGraph();
    qm = new IntegerCHDistanceQueryManagerWithMapping(*graph, mappingFile);
}

//______________________________________________________________________________________________________________________
unsigned int IntegerDistanceQueryManagerWithMappingAPI::distanceQuery(long long unsigned int source, long long unsigned int target) {
    return qm -> findDistance(source, target);
}

//______________________________________________________________________________________________________________________
void IntegerDistanceQueryManagerWithMappingAPI::clearStructures() {
    delete qm;
    delete graph;
}
