//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#include "CHDistanceQueryManagerNoMappingAPI.h"
#include "../GraphBuilding/Loaders/DDSGLoader.h"

//______________________________________________________________________________________________________________________
void CHDistanceQueryManagerNoMappingAPI::initializeCH(string chFile) {
    DDSGLoader chLoader = DDSGLoader(chFile);
    graph = chLoader.loadFlagsGraph();
    qm = new CHDistanceQueryManager(*graph);
}

//______________________________________________________________________________________________________________________
unsigned int CHDistanceQueryManagerNoMappingAPI::distanceQuery(unsigned int start, unsigned int goal) {
    return qm -> findDistance(start, goal);
}

//______________________________________________________________________________________________________________________
void CHDistanceQueryManagerNoMappingAPI::clearStructures() {
    delete qm;
    delete graph;
}
