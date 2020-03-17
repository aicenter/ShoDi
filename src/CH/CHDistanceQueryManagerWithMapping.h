//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#ifndef CONTRACTION_HIERARCHIES_CHDISTANCEQUERYMANAGERWITHMAPPING_H
#define CONTRACTION_HIERARCHIES_CHDISTANCEQUERYMANAGERWITHMAPPING_H


#include "../GraphBuilding/Structures/FlagsGraph.h"
#include "CHDistanceQueryManager.h"
#include <string>
#include <unordered_map>

using namespace std;

class CHDistanceQueryManagerWithMapping {
public:
    CHDistanceQueryManagerWithMapping(FlagsGraph & g, string mappingFilepath);
    double findDistance(const long long unsigned int source, const long long unsigned int target);
private:
    CHDistanceQueryManager qm;
    unordered_map<long long unsigned int, unsigned int> mapping;
};


#endif //CONTRACTION_HIERARCHIES_CHDISTANCEQUERYMANAGERWITHMAPPING_H
