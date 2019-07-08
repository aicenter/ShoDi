//
// Author: Xenty (Michal Cvach)
// Created on: 6.8.18
//

#ifndef TRANSIT_NODE_ROUTING_HOPSDIJKSTRANODE_H
#define TRANSIT_NODE_ROUTING_HOPSDIJKSTRANODE_H

#include "IntegerDijkstra/IntegerDijkstraNode.h"

// This structure is used during preprocessing for the Contraction Hierarchies. In addition to the classic 'nodeID' and
// 'weight' we want to store 'hops' in the current path to the node. This is used to reduce search space during
// many-to-many queries. We usually don't consider paths with more than 5 hops during the preprocessing phase.
//______________________________________________________________________________________________________________________
class IntegerHopsDijkstraNode : public IntegerDijkstraNode {
public:
    IntegerHopsDijkstraNode(unsigned int x, long long unsigned int y, unsigned int z);
    unsigned int hops;
};


#endif //TRANSIT_NODE_ROUTING_HOPSDIJKSTRANODE_H
