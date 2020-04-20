//
// Author: Xenty (Michal Cvach)
// Created on: 6.8.18
//

#ifndef TRANSIT_NODE_ROUTING_HOPSDIJKSTRANODE_H
#define TRANSIT_NODE_ROUTING_HOPSDIJKSTRANODE_H

#include "../../Dijkstra/DijkstraNode.h"

/**
 * This structure is used during preprocessing for the Contraction Hierarchies. In addition to the classic 'nodeID' and
 * 'weight' we want to store 'hops' in the current path to the node. This is used to reduce search space during
 * many-to-many queries. We usually do not consider paths with more than 5 hops during the preprocessing phase.
 */
class HopsDijkstraNode : public DijkstraNode {
public:
    /**
     * A simple constructor.
     *
     * @param x[in] The ID of the node.
     * @param y[in] The distance to the node (weight for the priority queue).
     * @param z[in] The number of hops so far.
     */
    HopsDijkstraNode(unsigned int x, unsigned int y, unsigned int z);

    unsigned int hops;
};


#endif //TRANSIT_NODE_ROUTING_HOPSDIJKSTRANODE_H
