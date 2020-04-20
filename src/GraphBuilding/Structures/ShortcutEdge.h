//
// Author: Xenty (Michal Cvach)
// Created on: 30.8.18
//

#ifndef TRANSIT_NODE_ROUTING_SHORTCUTEDGE_H
#define TRANSIT_NODE_ROUTING_SHORTCUTEDGE_H

/**
 * This structure is used during the preprocessing phase to keep track of all the created shortcuts.
 * Each instance represents one shortcut with all it's info necessary for additional preprocessing.
 */
class ShortcutEdge {
public:
    /**
     * A simple constructor.
     *
     * @param a[in] A source node for the shortcut edge.
     * @param b[in] A target node for the shortcut edge.
     * @param c[in] The weight of the shortcut edge.
     * @param d[in] The middle node for the shortcut edge.
     */
    ShortcutEdge(unsigned int a, unsigned int b, unsigned int c, unsigned int d);

    unsigned int weight;
    unsigned int sourceNode;
    unsigned int targetNode;
    unsigned int middleNode;
};


#endif //TRANSIT_NODE_ROUTING_SHORTCUTEDGE_H
