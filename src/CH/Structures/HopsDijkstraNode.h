//
// Author: Xenty (Michal Cvach)
// Created on: 6.8.18
//

#ifndef TRANSIT_NODE_ROUTING_HOPSDIJKSTRANODE_H
#define TRANSIT_NODE_ROUTING_HOPSDIJKSTRANODE_H

#include "../../Dijkstra/DijkstraNode.h"

class HopsDijkstraNode : public DijkstraNode {
public:
    HopsDijkstraNode(unsigned int x, long long unsigned int y, unsigned int z);
    unsigned int hops;
};


#endif //TRANSIT_NODE_ROUTING_HOPSDIJKSTRANODE_H
