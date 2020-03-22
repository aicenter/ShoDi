//
// Author: Xenty (Michal Cvach)
// Created on: 21.03.20
//

#include "TNRAFDistanceQueryManagerAPI.h"
#include "../GraphBuilding/Loaders/TGAFLoader.h"

//______________________________________________________________________________________________________________________
void TNRAFDistanceQueryManagerAPI::initializeTNRAF(string tnrafFile, string mappingFile) {
    TGAFLoader tnrafLoader = TGAFLoader(tnrafFile);
    graph = tnrafLoader.loadTNRAFforDistanceQueries();
    qm = new TNRAFDistanceQueryManagerWithMapping(*graph, mappingFile);
}

//______________________________________________________________________________________________________________________
unsigned int TNRAFDistanceQueryManagerAPI::distanceQuery(long long unsigned int source, long long unsigned int target) {
    return qm -> findDistance(source, target);
}

//______________________________________________________________________________________________________________________
void TNRAFDistanceQueryManagerAPI::clearStructures() {
    delete qm;
    delete graph;
}