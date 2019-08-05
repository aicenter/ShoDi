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


using namespace std;

//______________________________________________________________________________________________________________________
class TNRPreprocessor {
public:
    static void preprocessUsingCH(IntegerUpdateableGraph & graph, unsigned int transitNodesAmount = 1000);
private:
    static void findForwardAccessNodes(unsigned int source, vector < AccessNodeData> & accessNodes, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph);
    static void findBackwardAccessNodes(unsigned int source, vector < AccessNodeData> & accessNodes, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph);
};


#endif //CONTRACTION_HIERARCHIES_TNRPREPROCESSOR_H
