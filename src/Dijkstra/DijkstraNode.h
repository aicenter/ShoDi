//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_DIJKSTRANODE_H
#define TRANSIT_NODE_ROUTING_DIJKSTRANODE_H

// A simple structure used for the priority queue during the BasicDijkstra runs. We need both the 'nodeID' and 'weight'
// which is the distance of the node from the source node at the current moment. We order the nodes in an ascending
// order based on the weight.
//______________________________________________________________________________________________________________________
class DijkstraNode {
public:
    DijkstraNode(unsigned int x, long long unsigned int y);
    unsigned int ID;
    long long unsigned int weight;

};


#endif //TRANSIT_NODE_ROUTING_DIJKSTRANODE_H
