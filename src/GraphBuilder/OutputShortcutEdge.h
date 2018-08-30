//
// Author: Xenty (Michal Cvach)
// Created on: 29.8.18
//

#ifndef TRANSIT_NODE_ROUTING_OUTPUTSHORTCUTEDGE_H
#define TRANSIT_NODE_ROUTING_OUTPUTSHORTCUTEDGE_H


#include "OutputEdge.h"

class OutputShortcutEdge : public OutputEdge {
public:
    OutputShortcutEdge(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e);
    unsigned int middleNode;
};


#endif //TRANSIT_NODE_ROUTING_OUTPUTSHORTCUTEDGE_H
