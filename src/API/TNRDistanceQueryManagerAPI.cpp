//
// Author: Xenty (Michal Cvach)
// Created on: 21.03.20
//

#include "TNRDistanceQueryManagerAPI.h"
#include "../GraphBuilding/Loaders/TNRGLoader.h"

//______________________________________________________________________________________________________________________
void TNRDistanceQueryManagerAPI::initializeTNR(std::string tnrFile, std::string mappingFile) {
    TNRGLoader tnrloader = TNRGLoader(tnrFile);
    graph = tnrloader.loadTNRforDistanceQueries();
    qm = new TNRDistanceQueryManagerWithMapping(*graph, mappingFile);
}

//______________________________________________________________________________________________________________________
unsigned int TNRDistanceQueryManagerAPI::distanceQuery(long long unsigned int start, long long unsigned int goal) {
    return qm -> findDistance(start, goal);
}

//______________________________________________________________________________________________________________________
void TNRDistanceQueryManagerAPI::clearStructures() {
    delete qm;
    delete graph;
}