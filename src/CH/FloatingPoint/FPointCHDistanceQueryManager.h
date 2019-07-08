//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTCHDISTANCEQUERYMANAGER_H
#define CONTRACTION_HIERARCHIES_FPOINTCHDISTANCEQUERYMANAGER_H

#include "../../GraphBuilding/Structures/FloatingPointStructures/FPointFlagsGraph.h"
#include <vector>

using namespace std;

// This class is responsible for the Contraction Hierarchies 'distance' queries - when we only require the 'distance'
// between two points and don't care about the actual path.
//______________________________________________________________________________________________________________________
class FPointCHDistanceQueryManager {
public:
    FPointCHDistanceQueryManager(FPointFlagsGraph & g);
    double findDistance(const unsigned int source, const unsigned int target);
protected:
    void forwardStall(unsigned int stallnode, double stalldistance);
    void backwardStall(unsigned int stallnode, double stalldistance);
    FPointFlagsGraph & graph;
    double upperbound;
    vector<unsigned int> forwardChanged;
    vector<unsigned int> backwardChanged;
    vector<unsigned int> forwardStallChanged;
    vector<unsigned int> backwardStallChanged;
    void prepareStructuresForNextQuery();
};


#endif //CONTRACTION_HIERARCHIES_FPOINTCHDISTANCEQUERYMANAGER_H
