//
// Created by xenty on 8.7.19.
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTDIJKSTRANODE_H
#define CONTRACTION_HIERARCHIES_FPOINTDIJKSTRANODE_H


// A simple structure used for the priority queue during the BasicIntegerDijkstra runs. We need both the 'nodeID' and 'weight'
// which is the distance of the node from the source node at the current moment. We order the nodes in an ascending
// order based on the weight.
//______________________________________________________________________________________________________________________
class FPointDijkstraNode {
public:
    FPointDijkstraNode(unsigned int x, double y);
    unsigned int ID;
    double weight;

};


#endif //CONTRACTION_HIERARCHIES_FPOINTDIJKSTRANODE_H
