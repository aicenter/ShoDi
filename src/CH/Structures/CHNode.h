//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHNODE_H
#define TRANSIT_NODE_ROUTING_CHNODE_H

/**
 * Auxiliary node structure which is used in the CHPriorityQueue(). Since the weight in the queue can be less than zero
 * (for example for a node where its degree is higher than the amount of shortcuts added after contracting that node)
 * signed integer is used for the weight here. This is the main difference against DijkstraNode(), where the weight
 * is unsigned because distance during Dijkstra run can only be positive (in our case)
 */
class CHNode {
public:
    /**
     * A simple constructor.

     * @param x[in] The ID of the node.
     * @param y[in] The priority of the node.
     */
    CHNode(const unsigned int x, const int y);

    unsigned int id;
    int weight;
private:
};


#endif //TRANSIT_NODE_ROUTING_CHNODE_H
