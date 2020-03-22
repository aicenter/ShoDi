//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#include "CHDistanceQueryManagerAPI.h"
#include "../GraphBuilding/Loaders/DDSGLoader.h"

//______________________________________________________________________________________________________________________
void CHDistanceQueryManagerAPI::initializeCH(string chFile, string mappingFile) {
    DDSGLoader chLoader = DDSGLoader(chFile);
    graph = chLoader.loadFlagsGraph();
    qm = new CHDistanceQueryManagerWithMapping(*graph, mappingFile);
}

//______________________________________________________________________________________________________________________
unsigned int CHDistanceQueryManagerAPI::distanceQuery(long long unsigned int source, long long unsigned int target) {
    return qm -> findDistance(source, target);
}

//______________________________________________________________________________________________________________________
void CHDistanceQueryManagerAPI::clearStructures() {
    delete qm;
    delete graph;
}
