//
// Author: Xenty (Michal Cvach)
// Created on: 30.8.18
//

#ifndef TRANSIT_NODE_ROUTING_SHORTCUTEDGE_H
#define TRANSIT_NODE_ROUTING_SHORTCUTEDGE_H

// This structure is used during the preprocessing phase to keep track of all the created shortcuts.
// Each instance represents one shortcut with all it's info necessary for additional preprocessing.
//______________________________________________________________________________________________________________________
class ShortcutEdge {
public:
    ShortcutEdge(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
    unsigned int weight;
    unsigned int sourceNode;
    unsigned int targetNode;
    unsigned int middleNode;
};


#endif //TRANSIT_NODE_ROUTING_SHORTCUTEDGE_H
