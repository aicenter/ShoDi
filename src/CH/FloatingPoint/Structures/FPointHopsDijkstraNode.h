//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTHOPSDIJKSTRANODE_H
#define CONTRACTION_HIERARCHIES_FPOINTHOPSDIJKSTRANODE_H

#include "../../../Dijkstra/FloatingPointDijkstra/FPointDijkstraNode.h"

// This structure is used during preprocessing for the Contraction Hierarchies. In addition to the classic 'nodeID' and
// 'weight' we want to store 'hops' in the current path to the node. This is used to reduce search space during
// many-to-many queries. We usually don't consider paths with more than 5 hops during the preprocessing phase.
//______________________________________________________________________________________________________________________
class FPointHopsDijkstraNode : public FPointDijkstraNode {
public:
    FPointHopsDijkstraNode(unsigned int x, double y, unsigned int z);
    unsigned int hops;
};


#endif //CONTRACTION_HIERARCHIES_FPOINTHOPSDIJKSTRANODE_H
