//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
#define TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H

#include <queue>
#include <string>
#include "../GraphBuilder/Graph.h"
#include "Structures/CHNode.h"

using namespace std;

class CHPreprocessor {
public:
    static void preprocessAndSave(string filePath, Graph & graph);
private:
    static void initializePriorityQueue(priority_queue<CHNode, vector<CHNode>, bool (*)(CHNode, CHNode)> priorityQueue, Graph & graph);
    static unsigned int calculatePossibleShortcuts(const unsigned int i, Graph & graph);
    static long long unsigned int runRestrictedDijkstra(const unsigned int source, const unsigned int target, const long long unsigned int shortcutLength, const Graph & graph);
    static vector<bool> contracted;

};


#endif //TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
