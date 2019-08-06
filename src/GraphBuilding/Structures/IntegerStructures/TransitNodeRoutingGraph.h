//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGGRAPH_H
#define CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGGRAPH_H


#include "IntegerFlagsGraph.h"
#include "../../../TNR/Structures/AccessNodeData.h"

//______________________________________________________________________________________________________________________
class TransitNodeRoutingGraph : public IntegerFlagsGraph {
public:
    TransitNodeRoutingGraph(unsigned int nodes, unsigned int transitNodesAmount);
    bool isLocalQuery(unsigned int source, unsigned int target);
    unsigned int findTNRDistance(unsigned int source, unsigned int target);
    void addMappingPair(unsigned int realID, unsigned int transitNodesID);
    void setDistanceTableValue(unsigned int i, unsigned int j, unsigned int value);
    void setLocalityFilterValue(unsigned int i, unsigned int j, bool value);
    void addForwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance);
    void addBackwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance);
protected:
    vector < vector < AccessNodeData > > forwardAccessNodes;
    vector < vector < AccessNodeData > > backwardAccessNodes;
    vector < vector < unsigned int > > transitNodesDistanceTable;
    vector < vector < bool > > isLocal;
    unordered_map < unsigned int, unsigned int > transitNodeMapping;
};


#endif //CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGGRAPH_H
