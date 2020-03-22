//
// Author: Xenty (Michal Cvach)
// Created on: 21.03.20
//

#ifndef SHORTEST_PATHS_TNRAFDISTANCEQUERYMANAGERAPI_H
#define SHORTEST_PATHS_TNRAFDISTANCEQUERYMANAGERAPI_H


#include "../TNRAF/TNRAFDistanceQueryManagerWithMapping.h"

using namespace std;

// This class server as the API which will be used to call the Transit Node Routing with Arc Flags functionality
// from Java. It should be enough to generate JNI code for this class using SWIG and then use the generated code
// in Java. Using this API, one can load a Transit Node Routing with Arc Flags data structure (basically prepare
// all the structures for queries), then realize the actual queries, and then clean the structures when they are no
// longer needed.
// Please note, that if you do not call the 'clearStructures' function manually from Java, memory leaks will occur.
//______________________________________________________________________________________________________________________
class TNRAFDistanceQueryManagerAPI {
public:
    void initializeTNRAF(string tnrafFile, string mappingFile);
    unsigned int distanceQuery(long long unsigned int source, long long unsigned int target);
    void clearStructures();
private:
    TNRAFDistanceQueryManagerWithMapping * qm;
    TransitNodeRoutingArcFlagsGraph * graph;
};


#endif //SHORTEST_PATHS_TNRAFDISTANCEQUERYMANAGERAPI_H
