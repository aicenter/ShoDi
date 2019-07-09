//
// Author: Xenty (Michal Cvach)
// Created on: 9.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTCHDISTANCEQUERYMANAGERWITHMAPPING_H
#define CONTRACTION_HIERARCHIES_FPOINTCHDISTANCEQUERYMANAGERWITHMAPPING_H

#include "../../GraphBuilding/Structures/FloatingPointStructures/FPointFlagsGraph.h"
#include "FPointCHDistanceQueryManager.h"
#include <string>
#include <unordered_map>

using namespace std;

class FPointCHDistanceQueryManagerWithMapping {
public:
    FPointCHDistanceQueryManagerWithMapping(FPointFlagsGraph & g, string mappingFilepath);
    double findDistance(const long long unsigned int source, const long long unsigned int target);
private:
    FPointCHDistanceQueryManager qm;
    unordered_map<long long unsigned int, unsigned int> mapping;
};


#endif //CONTRACTION_HIERARCHIES_FPOINTCHDISTANCEQUERYMANAGERWITHMAPPING_H
