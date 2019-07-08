//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTCHPREPROCESSOR_H
#define CONTRACTION_HIERARCHIES_FPOINTCHPREPROCESSOR_H

#include "../Generic/Structures/CHpriorityQueue.h"
#include "../../GraphBuilding/Structures/FloatingPointStructures/FPointUpdateableGraph.h"
#include "../../GraphBuilding/Structures/FloatingPointStructures/FPointShortcutEdge.h"
#include <unordered_set>

using namespace std;

// This class is responsible for preprocessing a given graph for the Contraction Hierarchies.
//______________________________________________________________________________________________________________________
class FPointCHPreprocessor {
public:
    static void preprocessForDDSGF(FPointUpdateableGraph &graph);
private:
    static void reinsertShortcuts(FPointUpdateableGraph & graph);
    static void initializePriorityQueue(CHpriorityQueue & priorityQueue, FPointUpdateableGraph & graph);
    static void contractNodesWithUnpackingData(CHpriorityQueue & priorityQueue, FPointUpdateableGraph & graph);
    static void adjustNeighbourgDegrees(const unsigned int x, FPointUpdateableGraph & graph);
    static void getDistancesUsingNode(const unsigned int i, FPointUpdateableGraph & graph);
    static double longestPossibleShortcut(const unsigned int source);
    static void updateNeighboursPriorities(const unsigned int x, FPointUpdateableGraph & graph, CHpriorityQueue & priorityQueue);
    static void manyToManyWithBuckets(FPointUpdateableGraph & graph, bool deep);
    static void initBuckets(const unsigned int x, FPointUpdateableGraph & graph, double & lowestBucketVal);
    static void oneToManyWithBuckets(const unsigned int source, const double upperBound, FPointUpdateableGraph & graph, unsigned int hoplimit = 5, unsigned int maxexpanded = 1000);
    static void getPossibleShortcuts(const unsigned int i, FPointUpdateableGraph & graph, bool deep);
    static unsigned int calculateShortcutsAmount();
    static void actuallyAddShortcutsWithUnpackingData(FPointUpdateableGraph & graph, unsigned int x);
    static void removeContractedNodeEdges(FPointUpdateableGraph & graph, unsigned int x);
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
    static vector<double> dijkstraDistance;
    static vector<FPointShortcutEdge> allShortcuts;
    static unordered_map<pair<unsigned int, unsigned int>, double, pair_hash> distances;
    static unordered_map<pair<unsigned int, unsigned int>, double, pair_hash> distancesWithoutX;
    static vector<unsigned int> sources;
    static vector<unsigned int> targets;
    static unordered_set<unsigned int> targetsSet;
    static unordered_map<unsigned int, vector<pair<unsigned int, double > > > buckets;



};


#endif //CONTRACTION_HIERARCHIES_FPOINTCHPREPROCESSOR_H
