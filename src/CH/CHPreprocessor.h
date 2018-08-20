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
    static void preprocessAndSaveWithUnpackingData(string filePath, Graph & graph);
private:
    static void flushCHgraph(string & filePath, Graph & graph);
    static void flushGraph(string & filePath, Graph & graph);
    static void flushCHRanks(string & filePath);
    static void flushUnpackingData(string & filePath);
    static void initializePriorityQueue(CHpriorityQueue & priorityQueue, Graph & graph);
    static void contractNodes(CHpriorityQueue & priorityQueue, Graph & graph);
    static void contractNodesWithUnpackingData(CHpriorityQueue & priorityQueue, Graph & graph);
    static void adjustNeighbourgDegrees(const unsigned int x, Graph & graph);
    static void getDistancesUsingNode(const unsigned int i, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distances, vector<unsigned int> & s, vector<unsigned int> & t);
    static unsigned long long int longestPossibleShortcut(const unsigned int source, vector < unsigned int > & targets, map < pair < unsigned int, unsigned int >, unsigned long long int > & distances);
    static void updateNeighboursPriorities(const unsigned int x, Graph & graph, CHpriorityQueue & priorityQueue);
    static long long unsigned int runRestrictedDijkstra(const unsigned int source, const unsigned int target, const long long unsigned int shortcutLength, const Graph & graph);
    static void oneToManyRestrictedDijkstra(const unsigned int source, set<unsigned int> & targets, const long long unsigned int upperBound, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distancesWithoutX);
    static void oneToManyRestrictedDijkstraWithHopLimit(const unsigned int source, set<unsigned int> & targets, const long long unsigned int upperBound, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distancesWithoutX, unsigned int hoplimit = 5, unsigned int maxexpanded = 100);
    static void manyToManyWithBuckets(vector < unsigned int > & sources, vector < unsigned int > & targets, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distances, map<pair<unsigned int, unsigned int>, long long unsigned int> & distancesWithoutX, bool deep);
    static void initBuckets(const unsigned int x, Graph & graph, map<unsigned int, vector<pair<unsigned int, long long unsigned int > > > & buckets, long long unsigned int & lowestBucketVal);
    static void oneToManyWithBuckets(const unsigned int source, set<unsigned int> & targets, const long long unsigned int upperBound, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distancesWithoutX, map<unsigned int, vector<pair<unsigned int, long long unsigned int > > > & buckets, unsigned int hoplimit = 5, unsigned int maxexpanded = 1000);

    static void getPossibleShortcuts(const unsigned int i, Graph & graph, bool deep);
    static unsigned int calculateShortcutsAmount();
    static void actuallyAddShortcuts(Graph & graph);
    static void actuallyAddShortcutsWithUnpackingData(Graph & graph, unsigned int x);
    static void clearStructures();

    static vector<bool> contracted;
    static vector<unsigned int> preprocessingDegrees;
    static vector<unsigned int> nodeRanks;
    static vector<long long unsigned int> dijkstraDistance;
    static vector<pair<pair<unsigned int, unsigned int>, unsigned int>> unpacking;

    static map<pair<unsigned int, unsigned int>, long long unsigned int> distances;
    static map<pair<unsigned int, unsigned int>, long long unsigned int> distancesWithoutX;
    static vector<unsigned int> sources;
    static vector<unsigned int> targets;


};


#endif //TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
