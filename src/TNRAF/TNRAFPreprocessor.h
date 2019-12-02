//
// Author: Xenty (Michal Cvach)
// Created on: 30.11.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H
#define CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H


#include "../TNR/TNRPreprocessor.h"
#include "Structures/AccessNodeDataArcFlags.h"
#include "Structures/RegionsStructure.h"

using namespace std;

//______________________________________________________________________________________________________________________
class TNRAFPreprocessor : public TNRPreprocessor {
public:
    static void preprocessUsingCH(IntegerUpdateableGraph & graph, IntegerGraph & originalGraph, string outputPath, unsigned int transitNodesAmount = 1000,
                                  unsigned int regionsCnt = 32);
    static void getPowersOf2(vector<uint32_t> & powersOf2);
protected:
    static void outputGraph(string outputPath, IntegerUpdateableGraph & graph, vector < pair < unsigned int, IntegerQueryEdge > > & allEdges, vector < unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, vector < vector < AccessNodeDataArcFlags > > & forwardAccessNodes, vector < vector < AccessNodeDataArcFlags > > & backwardAccessNodes, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces, unsigned int transitNodesAmount, RegionsStructure & regions, unsigned int regionsCnt);
    static void computeTransitNodeDistanceTable(vector<unsigned int> & transitNodes, vector<vector<unsigned int>> & distanceTable, unsigned int transitNodesCnt, IntegerGraph & originalGraph);
    static void findForwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & forwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph, IntegerGraph & originalGraph, RegionsStructure & regions);
    static void findBackwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & backwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph, IntegerGraph & originalGraph, RegionsStructure & regions);
    static void computeForwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, IntegerGraph & originalGraph, RegionsStructure & regions);
    static void computeBackwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, IntegerGraph & originalGraph, RegionsStructure & regions);
    static void generateClustering(IntegerGraph & originalGraph, RegionsStructure & regions, unsigned int clustersCnt);
    static void initPowersOf2(vector<uint32_t> & powersOf2);
};


#endif //CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H
