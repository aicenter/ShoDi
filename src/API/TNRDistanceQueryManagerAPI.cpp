//
// Author: Xenty (Michal Cvach)
// Created on: 21.03.20
//

#include "TNRDistanceQueryManagerAPI.h"
#include "../GraphBuilding/Loaders/TNRGLoader.h"

//______________________________________________________________________________________________________________________
void TNRDistanceQueryManagerAPI::initializeTNR(string tnrFile, string mappingFile) {
    TNRGLoader tnrloader = TNRGLoader(tnrFile);
    graph = tnrloader.loadTNRforDistanceQueries();
    qm = new TNRDistanceQueryManagerWithMapping(*graph, mappingFile);
}

//______________________________________________________________________________________________________________________
unsigned int TNRDistanceQueryManagerAPI::distanceQuery(long long unsigned int source, long long unsigned int target) {
    return qm -> findDistance(source, target);
}

//______________________________________________________________________________________________________________________
void TNRDistanceQueryManagerAPI::clearStructures() {
    delete qm;
    delete graph;
}