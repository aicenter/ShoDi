//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#ifndef CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGARCFLAGSGRAPH_H
#define CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGARCFLAGSGRAPH_H


#include "TransitNodeRoutingGraph.h"
#include "../../../TNRAF/Structures/IntegerNodeDataRegions.h"
#include "../../../TNRAF/Structures/AccessNodeDataArcFlags.h"

class TransitNodeRoutingArcFlagsGraph : public TransitNodeRoutingGraph {
public:
    TransitNodeRoutingArcFlagsGraph(unsigned int nodes, unsigned int transitNodesAmount);
    IntegerNodeDataRegions & data(unsigned int node) override;
    void addForwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance, unsigned int regionsCnt, uint32_t regionsFlags, vector<unsigned int> & powersOf2);
    void addBackwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance, unsigned int regionsCnt, uint32_t regionsFlags, vector<unsigned int> & powersOf2);
    unsigned int findTNRAFDistance(unsigned int source, unsigned int target);
protected:
    vector < vector < AccessNodeDataArcFlags > > forwardAccessNodes;
    vector < vector < AccessNodeDataArcFlags > > backwardAccessNodes;
    vector< IntegerNodeDataRegions > nodesData;
    vector < unsigned int > regions;
};


#endif //CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGARCFLAGSGRAPH_H
