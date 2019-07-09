//
// Author: Xenty (Michal Cvach)
// Created on: 9.7.19
//

#include "DistanceQueryManagerAPI.h"
#include "../GraphBuilding/Loaders/DDSGFLoader.h"

// Initializes the Contraction Hierarchy for queries. This means loading the graph from the given input file, creating
// an instance of a 'FPointFlagsGraph', and then also creating an instance of the 'FPointCHDistanceQueryManager', which
// can then be used to answer queries.
//______________________________________________________________________________________________________________________
void DistanceQueryManagerAPI::initializeCH(string chFile) {
    DDSGFLoader chLoader = DDSGFLoader("../input/experimentGraphDebug.chf");
    graph = chLoader.loadFlagsGraph();
    qm = new FPointCHDistanceQueryManager(*graph);
}

// Queries after initialization are pretty straightforward, we just let the initialized QueryManager instance answer
// the queries.
//______________________________________________________________________________________________________________________
double DistanceQueryManagerAPI::distanceQuery(unsigned int source, unsigned int target) {
    return qm -> findDistance(source, target);
}

// This method clears the structures used for the Contraction Hierarchy. From Java, the user will manipulate a certain
// Java proxy class corresponding to this class. But as memory management in Java and C++ is different (C++ has no
// garbage collection), it is necessary to call this function once the user is done with his queries. Without calling
// this function, memory leaks will be possible and since the structures can be pretty big, it could lead to some
// serious problems.
//______________________________________________________________________________________________________________________
void DistanceQueryManagerAPI::clearStructures() {
    delete qm;
    delete graph;
}