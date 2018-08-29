//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHFLAGSGRAPHQUERYMANAGER_H
#define TRANSIT_NODE_ROUTING_CHFLAGSGRAPHQUERYMANAGER_H


#include <vector>
#include <queue>
#include "../GraphBuilder/FlagsGraph.h"
#include "CHQueryManager.h"
#include "Structures/QueryPriorityQueue.h"

using namespace std;

class CHFlagsGraphQueryManager : public CHQueryManager {
public:
    CHFlagsGraphQueryManager(vector<unsigned int> & x, const FlagsGraph & g);
    long long unsigned int findDistance(const unsigned int source, const unsigned int target);
protected:
    void forwardStall(unsigned int stallnode, long long unsigned int stalldistance);
    void backwardStall(unsigned int stallnode, long long unsigned int stalldistance);
    vector<unsigned int> & ranks;
    const FlagsGraph & graph;
    long long unsigned int upperbound;
    vector<long long unsigned int> forwardDist;
    vector<long long unsigned int> backwardDist;
    vector<unsigned int> forwardChanged;
    vector<unsigned int> backwardChanged;
    vector<unsigned int> forwardStallChanged;
    vector<unsigned int> backwardStallChanged;
    vector<bool> forwardReached;
    vector<bool> backwardReached;
    vector<bool> forwardSettled;
    vector<bool> backwardSettled;
    vector<bool> forwardStalled;
    vector<bool> backwardStalled;
    //priority_queue<DijkstraNode> forwardQ;
    //priority_queue<DijkstraNode> backwardQ;
    void prepareStructuresForNextQuery();
    //void relaxForwardEdges(unsigned int curNode, long long unsigned int curLen, priority_queue<DijkstraNode, vector<DijkstraNode>, bool> & pq);
    //void relaxBackwardEdges(unsigned int curNode, long long unsigned int curLen, priority_queue<DijkstraNode, vector<DijkstraNode>, bool> & pq);
};

#endif //TRANSIT_NODE_ROUTING_CHFLAGSGRAPHQUERYMANAGER_H
