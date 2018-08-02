//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
#define TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H

#include <map>
#include <string>
#include "../GraphBuilder/Graph.h"
#include "Structures/CHNode.h"
#include "Structures/CHpriorityQueue.h"

using namespace std;

class CHPreprocessor {
public:
    static void preprocessAndSave(string filePath, Graph & graph);
private:
    static void flushCHgraph(string & filePath, Graph & graph);
    static void initializePriorityQueue(CHpriorityQueue & priorityQueue, Graph & graph);
    static void contractNodes(CHpriorityQueue & priorityQueue, Graph & graph);
    static void contractOneNode(const unsigned int x, Graph & graph);
    static void adjustNeighbourgDegrees(const unsigned int x, Graph & graph);
    static unsigned int calculatePossibleShortcuts(const unsigned int i, Graph & graph);
    static void getDistancesUsingNode(const unsigned int i, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distances );
    static void updateNeighboursPriorities(const unsigned int x, Graph & graph, CHpriorityQueue & priorityQueue);
    static long long unsigned int runRestrictedDijkstra(const unsigned int source, const unsigned int target, const long long unsigned int shortcutLength, const Graph & graph);
    static vector<bool> contracted;
    static vector<unsigned int> preprocessingDegrees;
    static vector<unsigned int> nodeRanks;

};


#endif //TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
