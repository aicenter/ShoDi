//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#ifndef CONTRACTION_HIERARCHIES_INTEGERDISTANCEQUERYMANAGERWITHMAPPINGAPI_H
#define CONTRACTION_HIERARCHIES_INTEGERDISTANCEQUERYMANAGERWITHMAPPINGAPI_H


#include "../CH/Integer/IntegerCHDistanceQueryManagerWithMapping.h"
#include <string>

using namespace std;

//______________________________________________________________________________________________________________________
class IntegerDistanceQueryManagerWithMappingAPI {
public:
    void initializeCH(string chFile, string mappingFile);
    unsigned int distanceQuery(long long unsigned int source, long long unsigned int target);
    void clearStructures();
private:
    IntegerCHDistanceQueryManagerWithMapping * qm;
    IntegerFlagsGraph * graph;
};


#endif //CONTRACTION_HIERARCHIES_INTEGERDISTANCEQUERYMANAGERWITHMAPPINGAPI_H
