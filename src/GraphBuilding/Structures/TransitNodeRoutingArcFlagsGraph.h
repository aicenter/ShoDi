//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#ifndef CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGARCFLAGSGRAPH_H
#define CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGARCFLAGSGRAPH_H


#include "TransitNodeRoutingGraph.h"
#include "../../TNRAF/Structures/NodeDataRegions.h"
#include "../../TNRAF/Structures/AccessNodeDataArcFlags.h"

// This class contains all the information required for the Transit Node Routing with Arc Flags query algorithm.
// This class is an extension of the TransitNodeRoutingGraph. Additionally we have to store regions for all the nodes
// in the graph, and for each access node we need to store its Arc Flags. The findTNRAFDistance function then utilises
// those additional information to compute the shortest distance even quicker.
//______________________________________________________________________________________________________________________
class TransitNodeRoutingArcFlagsGraph : public TransitNodeRoutingGraph {
public:
    TransitNodeRoutingArcFlagsGraph(unsigned int nodes, unsigned int transitNodesAmount);
    virtual NodeDataRegions & data(unsigned int node) override;
    void addForwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance, unsigned int regionsCnt, uint32_t regionsFlags, vector<unsigned int> & powersOf2);
    void addBackwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance, unsigned int regionsCnt, uint32_t regionsFlags, vector<unsigned int> & powersOf2);
    unsigned int findTNRAFDistance(unsigned int source, unsigned int target);
protected:
    void resetForwardInfo(const unsigned int node) override;
    void resetBackwardInfo(const unsigned int node) override;
    vector < vector < AccessNodeDataArcFlags > > forwardAccessNodes;
    vector < vector < AccessNodeDataArcFlags > > backwardAccessNodes;
    vector< NodeDataRegions > nodesData;
};


#endif //CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGARCFLAGSGRAPH_H
