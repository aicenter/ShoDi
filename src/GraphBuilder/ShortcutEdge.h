//
// Author: Xenty (Michal Cvach)
// Created on: 30.8.18
//

#ifndef TRANSIT_NODE_ROUTING_SHORTCUTEDGE_H
#define TRANSIT_NODE_ROUTING_SHORTCUTEDGE_H


class ShortcutEdge {
public:
    ShortcutEdge(unsigned int a, unsigned int b, long long unsigned int c, unsigned int d);
    long long unsigned int weight;
    unsigned int sourceNode;
    unsigned int targetNode;
    unsigned int middleNode;
};


#endif //TRANSIT_NODE_ROUTING_SHORTCUTEDGE_H
