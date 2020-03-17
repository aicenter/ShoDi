//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
#define TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include "../GraphBuilding/Structures/IntegerUpdateableGraph.h"
#include "../GraphBuilding/Structures/IntegerShortcutEdge.h"
#include "Structures/CHNode.h"
#include "Structures/CHpriorityQueue.h"


using namespace std;

// This class is responsible for preprocessing a given graph for the Contraction Hierarchies.
//______________________________________________________________________________________________________________________
class IntegerCHPreprocessor {
public:
    static void preprocessForDDSG(IntegerUpdateableGraph & graph);
private:
    static void reinsertShortcuts(IntegerUpdateableGraph & graph);
    static void initializePriorityQueue(CHpriorityQueue & priorityQueue, IntegerUpdateableGraph & graph);
    static void contractNodesWithUnpackingData(CHpriorityQueue & priorityQueue, IntegerUpdateableGraph & graph);
    static void adjustNeighbourgDegrees(const unsigned int x, IntegerUpdateableGraph & graph);
    static void getDistancesUsingNode(const unsigned int i, IntegerUpdateableGraph & graph);
    static unsigned long long int longestPossibleShortcut(const unsigned int source);
    static void updateNeighboursPriorities(const unsigned int x, IntegerUpdateableGraph & graph, CHpriorityQueue & priorityQueue);
    static void manyToManyWithBuckets(IntegerUpdateableGraph & graph, bool deep);
    static void initBuckets(const unsigned int x, IntegerUpdateableGraph & graph, long long unsigned int & lowestBucketVal);
    static void oneToManyWithBuckets(const unsigned int source, const long long unsigned int upperBound, IntegerUpdateableGraph & graph, unsigned int hoplimit = 5, unsigned int maxexpanded = 1000);
    static void getPossibleShortcuts(const unsigned int i, IntegerUpdateableGraph & graph, bool deep);
    static unsigned int calculateShortcutsAmount();
    static void actuallyAddShortcutsWithUnpackingData(IntegerUpdateableGraph & graph, unsigned int x);
    static void removeContractedNodeEdges(IntegerUpdateableGraph & graph, unsigned int x);
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
    static vector<long long unsigned int> dijkstraDistance;
    static vector<IntegerShortcutEdge> allShortcuts;
    static unordered_map<pair<unsigned int, unsigned int>, long long unsigned int, pair_hash> distances;
    static unordered_map<pair<unsigned int, unsigned int>, long long unsigned int, pair_hash> distancesWithoutX;
    static vector<unsigned int> sources;
    static vector<unsigned int> targets;
    static unordered_set<unsigned int> targetsSet;
    static unordered_map<unsigned int, vector<pair<unsigned int, long long unsigned int > > > buckets;



};


#endif //TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
