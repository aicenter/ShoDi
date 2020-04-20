//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#ifndef CONTRACTION_HIERARCHIES_NODEDATAREGIONS_H
#define CONTRACTION_HIERARCHIES_NODEDATAREGIONS_H


#include "../../CH/Structures/NodeData.h"

/**
 * Extension of the NodeData class that additionally stores information about the region of a node.
 */
class NodeDataRegions : public NodeData {
public:
    /**
     * A simple constructor.
     */
    NodeDataRegions();
    unsigned int region;
};


#endif //CONTRACTION_HIERARCHIES_NODEDATAREGIONS_H
