//
// Author: Xenty (Michal Cvach)
// Created on: 30.11.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H
#define CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H


#include "../TNR/TNRPreprocessor.h"
#include "Structures/AccessNodeDataArcFlags.h"

using namespace std;

//______________________________________________________________________________________________________________________
class TNRAFPreprocessor : public TNRPreprocessor {
public:
    static void preprocessUsingCH(IntegerUpdateableGraph & graph, IntegerGraph & originalGraph, string outputPath, unsigned int transitNodesAmount = 1000,
                                  unsigned int regionsCnt = 32);
    static void getPowersOf2(vector<uint32_t> & powersOf2);
protected:
    static void outputGraph(string outputPath, IntegerUpdateableGraph & graph, vector < pair < unsigned int, IntegerQueryEdge > > & allEdges, vector < unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, vector < vector < AccessNodeDataArcFlags > > & forwardAccessNodes, vector < vector < AccessNodeDataArcFlags > > & backwardAccessNodes, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces, unsigned int transitNodesAmount, vector<unsigned int> & regions, unsigned int regionsCnt);
    static void findForwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & forwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph, IntegerGraph & originalGraph, vector<unsigned int> & clusters, unsigned int regionsCnt);
    static void findBackwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & backwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph, IntegerGraph & originalGraph, vector<unsigned int> & clusters, unsigned int regionsCnt);
    static void computeForwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, IntegerGraph & originalGraph, vector<unsigned int> & clusters, unsigned int regionsCnt);
    static void computeBackwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, IntegerGraph & originalGraph, vector<unsigned int> & clusters, unsigned int regionsCnt);
    static void generateClustering(IntegerGraph & originalGraph, vector<unsigned int> & clusters, unsigned int clustersCnt);
    static void initPowersOf2(vector<uint32_t> & powersOf2);
};


#endif //CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H
