//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
#define TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H

#include <map>
#include <set>
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
    static void flushGraph(string & filePath, Graph & graph);
    static void flushCHRanks(string & filePath);
    static void initializePriorityQueue(CHpriorityQueue & priorityQueue, Graph & graph);
    static void contractNodes(CHpriorityQueue & priorityQueue, Graph & graph);
    static void contractOneNode(const unsigned int x, Graph & graph);
    static void adjustNeighbourgDegrees(const unsigned int x, Graph & graph);
    static unsigned int calculatePossibleShortcuts(const unsigned int i, Graph & graph);
    static void getDistancesUsingNode(const unsigned int i, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distances, map<unsigned int, set<unsigned int> > & souAndTar);
    static unsigned long long int longestPossibleShortcut(const unsigned int source, set < unsigned int > & targets, map < pair < unsigned int, unsigned int >, unsigned long long int > & distances);
    static void updateNeighboursPriorities(const unsigned int x, Graph & graph, CHpriorityQueue & priorityQueue);
    static long long unsigned int runRestrictedDijkstra(const unsigned int source, const unsigned int target, const long long unsigned int shortcutLength, const Graph & graph);
    static void oneToManyRestrictedDijkstra(const unsigned int source, set<unsigned int> & targets, const long long unsigned int upperBound, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distancesWithoutX);
    static void oneToManyRestrictedDijkstraWithHopLimit(const unsigned int source, set<unsigned int> & targets, const long long unsigned int upperBound, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distancesWithoutX, unsigned int hoplimit = 5, unsigned int maxexpanded = 100);
    static vector<bool> contracted;
    static vector<unsigned int> preprocessingDegrees;
    static vector<unsigned int> nodeRanks;
    static vector<long long unsigned int> dijkstraDistance;


};


#endif //TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
