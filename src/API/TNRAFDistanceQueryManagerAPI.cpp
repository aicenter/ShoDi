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
unsigned int TNRAFDistanceQueryManagerAPI::distanceQuery(long long unsigned int start, long long unsigned int goal) {
    return qm -> findDistance(start, goal);
}

//______________________________________________________________________________________________________________________
void TNRAFDistanceQueryManagerAPI::clearStructures() {
    delete qm;
    delete graph;
}