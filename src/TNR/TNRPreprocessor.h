//
// Author: Xenty (Michal Cvach)
// Created on: 05.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRPREPROCESSOR_H
#define CONTRACTION_HIERARCHIES_TNRPREPROCESSOR_H

#include <unordered_set>
#include "../GraphBuilding/Structures/UpdateableGraph.h"
#include "Structures/AccessNodeData.h"
#include "../GraphBuilding/Structures/FlagsGraph.h"
#include "../GraphBuilding/Loaders/XenGraphLoader.h"
#include "../DistanceMatrix/DistanceMatrix.h"


using namespace std;

// This class can create a Transit Node Routing (TNR) data-structure based on a given UpdateableGraph.
//______________________________________________________________________________________________________________________
class TNRPreprocessor {
public:
    static void preprocessUsingCH(UpdateableGraph & graph, string outputPath, unsigned int transitNodesAmount = 1000);
    static void preprocessWithDMvalidation(UpdateableGraph & graph, Graph & originalGraph, string outputPath, unsigned int transitNodesAmount = 1000);
protected:
    static void outputGraph(string outputPath, UpdateableGraph & graph, vector < pair < unsigned int, QueryEdge > > & allEdges, vector < unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, vector < vector < AccessNodeData > > & forwardAccessNodes, vector < vector < AccessNodeData > > & backwardAccessNodes, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces, unsigned int transitNodesAmount);
    static void findForwardAccessNodes(unsigned int source, vector < AccessNodeData> & accessNodes, vector < unsigned int > & forwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, FlagsGraph & graph);
    static void findBackwardAccessNodes(unsigned int source, vector < AccessNodeData> & accessNodes, vector < unsigned int > & backwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, FlagsGraph & graph);
    static void findForwardAccessNodes(unsigned int source, vector < AccessNodeData> & accessNodes, vector < unsigned int > & forwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, FlagsGraph & graph, DistanceMatrix & dm);
    static void findBackwardAccessNodes(unsigned int source, vector < AccessNodeData> & accessNodes, vector < unsigned int > & backwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, FlagsGraph & graph, DistanceMatrix & dm);
    static void prepareLocalityFilter(vector < vector < bool > > & isLocal, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces);
    static bool notEmptySearchSpacesUnion(unsigned int i, unsigned int j, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces);

//# FIXME debug stuff
    static unsigned int accessNodesFw;
    static unsigned int removedAccessNodesFw;
    static unsigned int accessNodesBw;
    static unsigned int removedAccessNodesBw;
};


#endif //CONTRACTION_HIERARCHIES_TNRPREPROCESSOR_H
