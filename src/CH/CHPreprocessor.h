//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
#define TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include "../GraphBuilding/Structures/UpdateableGraph.h"
#include "../GraphBuilding/Structures/ShortcutEdge.h"
#include "Structures/CHNode.h"
#include "Structures/CHpriorityQueue.h"


using namespace std;

// This class is responsible for preprocessing a given graph for the Contraction Hierarchies.
//______________________________________________________________________________________________________________________
class CHPreprocessor {
public:
    static void preprocessForDDSG(UpdateableGraph & graph);
private:
    static void reinsertShortcuts(UpdateableGraph & graph);
    static void initializePriorityQueue(CHpriorityQueue & priorityQueue, UpdateableGraph & graph);
    static void contractNodesWithUnpackingData(CHpriorityQueue & priorityQueue, UpdateableGraph & graph);
    static void adjustNeighbourgDegrees(const unsigned int x, UpdateableGraph & graph);
    static void getDistancesUsingNode(const unsigned int i, UpdateableGraph & graph);
    static unsigned long long int longestPossibleShortcut(const unsigned int source);
    static void updateNeighboursPriorities(const unsigned int x, UpdateableGraph & graph, CHpriorityQueue & priorityQueue);
    static void manyToManyWithBuckets(UpdateableGraph & graph, bool deep);
    static void initBuckets(const unsigned int x, UpdateableGraph & graph, unsigned int & lowestBucketVal);
    static void oneToManyWithBuckets(const unsigned int source, const unsigned int upperBound, UpdateableGraph & graph, unsigned int hoplimit = 5, unsigned int maxexpanded = 1000);
    static void getPossibleShortcuts(const unsigned int i, UpdateableGraph & graph, bool deep);
    static unsigned int calculateShortcutsAmount();
    static void actuallyAddShortcutsWithUnpackingData(UpdateableGraph & graph, unsigned int x);
    static void removeContractedNodeEdges(UpdateableGraph & graph, unsigned int x);
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
    static vector<unsigned int> dijkstraDistance;
    static vector<ShortcutEdge> allShortcuts;
    static unordered_map<pair<unsigned int, unsigned int>, unsigned int, pair_hash> distances;
    static unordered_map<pair<unsigned int, unsigned int>, unsigned int, pair_hash> distancesWithoutX;
    static vector<unsigned int> sources;
    static vector<unsigned int> targets;
    static unordered_set<unsigned int> targetsSet;
    static unordered_map<unsigned int, vector<pair<unsigned int, unsigned int > > > buckets;



};


#endif //TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
