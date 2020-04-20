//
// Author: Xenty (Michal Cvach)
// Created on: 29.8.18
//

#ifndef TRANSIT_NODE_ROUTING_OUTPUTSHORTCUTEDGE_H
#define TRANSIT_NODE_ROUTING_OUTPUTSHORTCUTEDGE_H


#include "OutputEdge.h"

/**
 * Simple structure used for outputing shortcut edges into the binary .ch format.
 * Each instance is one shortcut edge with all it's necessary information ('middleNode' is added)
 */
class OutputShortcutEdge : public OutputEdge {
public:
    /**
     * A simple constructor.
     *
     * @param a[in] Source node for the shortcut edge.
     * @param b[in] Target node for the shortcut edge.
     * @param c[in] Weight of the shortcut edge.
     * @param d[in] Flags for the shortcut edge.
     * @param e[in] Middle node for the shortcut edge. (Necessary to reconstruct paths in the original graph.)
     */
    OutputShortcutEdge(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e);

    unsigned int middleNode;
};


#endif //TRANSIT_NODE_ROUTING_OUTPUTSHORTCUTEDGE_H
