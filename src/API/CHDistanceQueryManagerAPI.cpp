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
unsigned int CHDistanceQueryManagerAPI::distanceQuery(long long unsigned int start, long long unsigned int goal) {
    return qm -> findDistance(start, goal);
}

//______________________________________________________________________________________________________________________
void CHDistanceQueryManagerAPI::clearStructures() {
    delete qm;
    delete graph;
}
