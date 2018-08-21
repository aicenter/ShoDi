//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
#define TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H

#include <map>
#include <unordered_map>
#include <unordered_set>
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
    static void getDistancesUsingNode(const unsigned int i, Graph & graph);
    static unsigned long long int longestPossibleShortcut(const unsigned int source);
    static void updateNeighboursPriorities(const unsigned int x, Graph & graph, CHpriorityQueue & priorityQueue);
    static long long unsigned int runRestrictedDijkstra(const unsigned int source, const unsigned int target, const long long unsigned int shortcutLength, const Graph & graph);
    static void oneToManyRestrictedDijkstra(const unsigned int source, unordered_set<unsigned int> & targets, const long long unsigned int upperBound, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distancesWithoutX);
    static void oneToManyRestrictedDijkstraWithHopLimit(const unsigned int source, unordered_set<unsigned int> & targets, const long long unsigned int upperBound, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distancesWithoutX, unsigned int hoplimit = 5, unsigned int maxexpanded = 100);
    static void manyToManyWithBuckets(Graph & graph, bool deep);
    static void initBuckets(const unsigned int x, Graph & graph, long long unsigned int & lowestBucketVal);
    static void oneToManyWithBuckets(const unsigned int source, const long long unsigned int upperBound, Graph & graph, unsigned int hoplimit = 5, unsigned int maxexpanded = 1000);

    static void getPossibleShortcuts(const unsigned int i, Graph & graph, bool deep);
    static unsigned int calculateShortcutsAmount();
    static void actuallyAddShortcuts(Graph & graph);
    static void actuallyAddShortcutsWithUnpackingData(Graph & graph, unsigned int x);
    static void clearStructures();

    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1,T2> &p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);

            return h1 ^ h2;
        }
    };

    static vector<bool> contracted;
    static vector<unsigned int> preprocessingDegrees;
    static vector<unsigned int> nodeRanks;
    static vector<long long unsigned int> dijkstraDistance;
    static vector<pair<pair<unsigned int, unsigned int>, unsigned int>> unpacking;

    static unordered_map<pair<unsigned int, unsigned int>, long long unsigned int, pair_hash> distances;
    static unordered_map<pair<unsigned int, unsigned int>, long long unsigned int, pair_hash> distancesWithoutX;
    static vector<unsigned int> sources;
    static vector<unsigned int> targets;
    static unordered_set<unsigned int> targetsSet;
    static unordered_map<unsigned int, vector<pair<unsigned int, long long unsigned int > > > buckets;



};


#endif //TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
