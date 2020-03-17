//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#ifndef CONTRACTION_HIERARCHIES_INTEGERNODEDATAREGIONS_H
#define CONTRACTION_HIERARCHIES_INTEGERNODEDATAREGIONS_H


#include "../../CH/Structures/IntegerNodeData.h"

class IntegerNodeDataRegions : public IntegerNodeData {
public:
    IntegerNodeDataRegions();
    unsigned int region;
};


#endif //CONTRACTION_HIERARCHIES_INTEGERNODEDATAREGIONS_H
