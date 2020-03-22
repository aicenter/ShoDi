//
// Author: Xenty (Michal Cvach)
// Created on: 21.03.20
//

#ifndef SHORTEST_PATHS_TNRDISTANCEQUERYMANAGERAPI_H
#define SHORTEST_PATHS_TNRDISTANCEQUERYMANAGERAPI_H


#include "../TNR/TNRDistanceQueryManagerWithMapping.h"

using namespace std;

// This class server as the API which will be used to call the Transit Node Routing functionality from Java.
// It should be enough to generate JNI code for this class using SWIG and then use the generated code in Java.
// Using this API, one can load a Transit Node Routing data structure (to prepare all the structures for queries),
// then realize the actual queries, and then clean the structures when they are no longer needed.
// Please note, that if you do not call the 'clearStructures' function manually from Java, memory leaks will occur.
//______________________________________________________________________________________________________________________
class TNRDistanceQueryManagerAPI {
public:
    void initializeTNR(string tnrFile, string mappingFile);
    unsigned int distanceQuery(long long unsigned int source, long long unsigned int target);
    void clearStructures();
private:
    TNRDistanceQueryManagerWithMapping * qm;
    TransitNodeRoutingGraph * graph;
};


#endif //SHORTEST_PATHS_TNRDISTANCEQUERYMANAGERAPI_H
