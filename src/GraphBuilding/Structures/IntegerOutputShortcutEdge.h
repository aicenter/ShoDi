//
// Author: Xenty (Michal Cvach)
// Created on: 29.8.18
//

#ifndef TRANSIT_NODE_ROUTING_OUTPUTSHORTCUTEDGE_H
#define TRANSIT_NODE_ROUTING_OUTPUTSHORTCUTEDGE_H


#include "IntegerOutputEdge.h"

// Simple structure used for outputing shortcut edges into the binary .ch format.
// Each instance is one shortcut edge with all it's necessary information ('middleNode' is added)
//______________________________________________________________________________________________________________________
class IntegerOutputShortcutEdge : public IntegerOutputEdge {
public:
    IntegerOutputShortcutEdge(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e);
    unsigned int middleNode;
};


#endif //TRANSIT_NODE_ROUTING_OUTPUTSHORTCUTEDGE_H
