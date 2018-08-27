//
// Author: Xenty (Michal Cvach)
// Created on: 6.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHQUERYMANAGER_H
#define TRANSIT_NODE_ROUTING_CHQUERYMANAGER_H

#include <vector>
#include <unordered_set>
#include "../GraphBuilder/Graph.h"

using namespace std;

// This class is responsible for the Contraction Hierarchies 'distance' queries - when we only require the 'distance'
// between two points and don't care about the actual path.
//______________________________________________________________________________________________________________________
class CHQueryManager {
public:
    long long unsigned int findDistance(const unsigned int source, const unsigned int target, const Graph & graph);
protected:
    long long unsigned int existsBackwardEdge(const unsigned int x, const unsigned int y, const Graph & graph);
    long long unsigned int existsForwardEdge(const unsigned int x, const unsigned int y, const Graph & graph);
    void forwardStall(const unsigned int node, const long long unsigned int weight, unordered_set<unsigned int> & forwardStalled, const Graph & graph, const long long unsigned int * fromDistance);
    void backwardStall(const unsigned int node, const long long unsigned int weight, unordered_set<unsigned int> & backwardStalled, const Graph & graph, const long long unsigned int * toDistance);
};


#endif //TRANSIT_NODE_ROUTING_CHQUERYMANAGER_H
