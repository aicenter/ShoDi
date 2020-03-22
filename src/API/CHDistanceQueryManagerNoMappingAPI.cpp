//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#include "CHDistanceQueryManagerAPI.h"
#include "../GraphBuilding/Loaders/DDSGLoader.h"

// Initializes the Contraction Hierarchy for queries. This means loading the graph from the given input file, creating
// an instance of a 'FlagsGraph', and then also creating an instance of the 'CHDistanceQueryManager', which
// can then be used to answer queries.
//______________________________________________________________________________________________________________________
void CHDistanceQueryManagerAPI::initializeCH(string chFile) {
    DDSGLoader chLoader = DDSGLoader(chFile);
    graph = chLoader.loadFlagsGraph();
    qm = new CHDistanceQueryManager(*graph);
}

// Queries after initialization are pretty straightforward, we just let the initialized QueryManager instance answer
// the queries.
//______________________________________________________________________________________________________________________
unsigned int CHDistanceQueryManagerAPI::distanceQuery(unsigned int source, unsigned int target) {
    return qm -> findDistance(source, target);
}

// This method clears the structures used for the Contraction Hierarchy. From Java, the user will manipulate a certain
// Java proxy class corresponding to this class. But as memory management in Java and C++ is different (C++ has no
// garbage collection), it is necessary to call this function once the user is done with his queries. Without calling
// this function, memory leaks will be possible and since the structures can be pretty big, it could lead to some
// serious problems.
//______________________________________________________________________________________________________________________
void CHDistanceQueryManagerAPI::clearStructures() {
    delete qm;
    delete graph;
}
