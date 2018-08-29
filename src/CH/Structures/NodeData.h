//
// Author: Xenty (Michal Cvach)
// Created on: 29.8.18
//

#ifndef TRANSIT_NODE_ROUTING_NODEDATA_H
#define TRANSIT_NODE_ROUTING_NODEDATA_H


class NodeData {
public:
    NodeData();
    long long unsigned int forwardDist;
    long long unsigned int backwardDist;
    bool forwardReached;
    bool backwardReached;
    bool forwardSettled;
    bool backwardSettled;
    bool forwardStalled;
    bool backwardStalled;
};


#endif //TRANSIT_NODE_ROUTING_NODEDATA_H
