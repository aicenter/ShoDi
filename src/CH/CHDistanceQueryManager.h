//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHFLAGSGRAPHQUERYMANAGER_H
#define TRANSIT_NODE_ROUTING_CHFLAGSGRAPHQUERYMANAGER_H


#include <vector>
#include <queue>
#include "../GraphBuilder/FlagsGraph.h"
#include "Structures/QueryPriorityQueue.h"

using namespace std;

class CHDistanceQueryManager {
public:
    CHDistanceQueryManager(FlagsGraph & g);
    long long unsigned int findDistance(const unsigned int source, const unsigned int target);
protected:
    void forwardStall(unsigned int stallnode, long long unsigned int stalldistance);
    void backwardStall(unsigned int stallnode, long long unsigned int stalldistance);
    FlagsGraph & graph;
    long long unsigned int upperbound;
    vector<unsigned int> forwardChanged;
    vector<unsigned int> backwardChanged;
    vector<unsigned int> forwardStallChanged;
    vector<unsigned int> backwardStallChanged;
    void prepareStructuresForNextQuery();
};

#endif //TRANSIT_NODE_ROUTING_CHFLAGSGRAPHQUERYMANAGER_H
