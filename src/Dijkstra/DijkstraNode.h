//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_DIJKSTRANODE_H
#define TRANSIT_NODE_ROUTING_DIJKSTRANODE_H

/**
 * A simple structure used for the priority queue during the BasicDijkstra runs. We need both the 'nodeID' and 'weight'
 * which is the distance of the node from the source node at the current moment. We order the nodes in an ascending
 * order based on the weight.
 */
class DijkstraNode {
public:
    /**
     * A simple constructor.
     *
     * @param x[in] The ID of the node.
     * @param y[in] The distance to the node (weight for the priority queue).
     */
    DijkstraNode(unsigned int x, unsigned int y);

    unsigned int ID;
    unsigned int weight;

};


#endif //TRANSIT_NODE_ROUTING_DIJKSTRANODE_H
