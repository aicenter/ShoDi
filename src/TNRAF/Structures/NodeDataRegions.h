//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#ifndef CONTRACTION_HIERARCHIES_NODEDATAREGIONS_H
#define CONTRACTION_HIERARCHIES_NODEDATAREGIONS_H


#include "../../CH/Structures/NodeData.h"

class NodeDataRegions : public NodeData {
public:
    NodeDataRegions();
    unsigned int region;
};


#endif //CONTRACTION_HIERARCHIES_NODEDATAREGIONS_H
