//
// Author: Xenty (Michal Cvach)
// Created on: 9.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTDISTANCEQUERYMANAGERAPI_H
#define CONTRACTION_HIERARCHIES_FPOINTDISTANCEQUERYMANAGERAPI_H

#include "../CH/FloatingPoint/FPointCHDistanceQueryManager.h"
#include <string>

using namespace std;

// This class server as the API which will be used to call the Contraction Hierarchies functionality from Java.
// It should be enough to generate JNI code for this class using SWIG and then use the generated code in Java.
// Using this API, one can load a Contraction Hierarchy (basically prepare all the structures for queries),
// then realize the actual queries, and then clean the structures when they are no longer needed.
// Please note, that if you do not call the 'clearStructures' function manually from Java, memory leaks will occur.
//______________________________________________________________________________________________________________________
class FPointDistanceQueryManagerAPI {
public:
    void initializeCH(string chFile);
    double distanceQuery(unsigned int source, unsigned int target);
    void clearStructures();
private:
    FPointCHDistanceQueryManager * qm;
    FPointFlagsGraph * graph;
};


#endif //CONTRACTION_HIERARCHIES_FPOINTDISTANCEQUERYMANAGERAPI_H
