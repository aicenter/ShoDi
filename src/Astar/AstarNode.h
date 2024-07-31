//
// Author: horakj31 (Jan Horak)
// Created on: 25.07.24
//

#ifndef ASTARNODE_H
#define ASTARNODE_H

/**
 * A simple structure used for the priority queue during the A* runs. We need both the 'nodeID' and 'weight'
 * which is the distance of the node from the source node at the current moment. We order the nodes in an ascending
 * order based on the weight.
 */
class AstarNode {
public:
    /**
     * A simple constructor.
     *
     * @param x[in] The ID of the node.
     * @param y[in] The distance to the node (weight for the priority queue).
     */
    AstarNode(unsigned int x, double y);

    unsigned int ID;
    double weight;

};


#endif //ASTARNODE_H
