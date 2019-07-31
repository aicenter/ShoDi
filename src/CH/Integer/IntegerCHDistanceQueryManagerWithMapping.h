//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#ifndef CONTRACTION_HIERARCHIES_INTEGERCHDISTANCEQUERYMANAGERWITHMAPPING_H
#define CONTRACTION_HIERARCHIES_INTEGERCHDISTANCEQUERYMANAGERWITHMAPPING_H


#include "../../GraphBuilding/Structures/IntegerStructures/IntegerFlagsGraph.h"
#include "IntegerCHDistanceQueryManager.h"
#include <string>
#include <unordered_map>

using namespace std;

class IntegerCHDistanceQueryManagerWithMapping {
public:
    IntegerCHDistanceQueryManagerWithMapping(IntegerFlagsGraph & g, string mappingFilepath);
    double findDistance(const long long unsigned int source, const long long unsigned int target);
private:
    IntegerCHDistanceQueryManager qm;
    unordered_map<long long unsigned int, unsigned int> mapping;
};


#endif //CONTRACTION_HIERARCHIES_INTEGERCHDISTANCEQUERYMANAGERWITHMAPPING_H
