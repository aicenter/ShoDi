//
// Author: Xenty (Michal Cvach)
// Created on: 30.11.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H
#define CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H


#include <queue>
#include "../TNR/TNRPreprocessor.h"
#include "Structures/AccessNodeDataArcFlags.h"
#include "Structures/RegionsStructure.h"
#include "../DistanceMatrix/DistanceMatrix.h"

using namespace std;

//______________________________________________________________________________________________________________________
class TNRAFPreprocessor : public TNRPreprocessor {
public:
    static void preprocessUsingCH(UpdateableGraph & graph, Graph & originalGraph, string outputPath, unsigned int transitNodesAmount = 1000,
                                  unsigned int regionsCnt = 32, bool useDistanceMatrix = false);
    static void getPowersOf2(vector<uint32_t> & powersOf2);
protected:
    static void outputGraph(string outputPath, UpdateableGraph & graph, vector < pair < unsigned int, QueryEdge > > & allEdges, vector < unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, vector < vector < AccessNodeDataArcFlags > > & forwardAccessNodes, vector < vector < AccessNodeDataArcFlags > > & backwardAccessNodes, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces, unsigned int transitNodesAmount, RegionsStructure & regions, unsigned int regionsCnt);
    static void computeTransitNodeDistanceTable(vector<unsigned int> & transitNodes, vector<vector<unsigned int>> & distanceTable, unsigned int transitNodesCnt, Graph & originalGraph);
    static void fillTransitNodeDistanceTable(vector<unsigned int> & transitNodes, vector<vector<unsigned int>> & distanceTable, unsigned int transitNodesCnt);
    static void findForwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & forwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, FlagsGraph & graph, Graph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix);
    static void findBackwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & backwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, FlagsGraph & graph, Graph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix);
    static void computeForwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, Graph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix);
    static void computeBackwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, Graph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix);
    static void generateClustering(Graph & originalGraph, RegionsStructure & regions, unsigned int clustersCnt);
    static const unsigned int getNewNodeForCluster( vector < unsigned int > & assignedClusters, queue < unsigned int > & q);
    static void initPowersOf2(vector<uint32_t> & powersOf2);
    static DistanceMatrix * distanceMatrix;

    // FIXME this is only debug stuff
    static unsigned int totalArcFlags;
    static unsigned int trueArcFlags;
    static unsigned int totalAccessNodes;
    static unsigned int uselessAccessNodes;
    static long long unsigned int triedCombinations;
    static unsigned int incorrectANdistances;
    static unsigned int ANdistances;
    static unsigned int clusteringSkips;
};


#endif //CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H
