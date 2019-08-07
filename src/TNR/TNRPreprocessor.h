//
// Author: Xenty (Michal Cvach)
// Created on: 05.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRPREPROCESSOR_H
#define CONTRACTION_HIERARCHIES_TNRPREPROCESSOR_H

#include <unordered_set>
#include "../GraphBuilding/Structures/IntegerStructures/IntegerUpdateableGraph.h"
#include "Structures/AccessNodeData.h"
#include "../GraphBuilding/Structures/IntegerStructures/IntegerFlagsGraph.h"
#include "../GraphBuilding/Loaders/IntegerXenGraphLoader.h"


using namespace std;

// This class can create a Transit Node Routing (TNR) data-structure based on a given IntegerUpdateableGraph.
//______________________________________________________________________________________________________________________
class TNRPreprocessor {
public:
    static void preprocessUsingCH(IntegerUpdateableGraph & graph, string outputPath, unsigned int transitNodesAmount = 1000);
private:
    static void outputGraph(string outputPath, IntegerUpdateableGraph & graph, vector < pair < unsigned int, IntegerQueryEdge > > & allEdges, vector < unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, vector < vector < AccessNodeData > > & forwardAccessNodes, vector < vector < AccessNodeData > > & backwardAccessNodes, vector < vector < bool > > & isLocal, unsigned int transitNodesAmount);
    static void findForwardAccessNodes(unsigned int source, vector < AccessNodeData> & accessNodes, vector < unsigned int > & forwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph);
    static void findBackwardAccessNodes(unsigned int source, vector < AccessNodeData> & accessNodes, vector < unsigned int > & backwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph);
    static void prepareLocalityFilter(vector < vector < bool > > & isLocal, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces);
    static bool notEmptySearchSpacesUnion(unsigned int i, unsigned int j, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces);
};


#endif //CONTRACTION_HIERARCHIES_TNRPREPROCESSOR_H
