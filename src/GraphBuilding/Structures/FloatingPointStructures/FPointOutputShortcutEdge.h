//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTOUTPUTSHORTCUTEDGE_H
#define CONTRACTION_HIERARCHIES_FPOINTOUTPUTSHORTCUTEDGE_H

#include "FPointOutputEdge.h"

class FPointOutputShortcutEdge : public FPointOutputEdge {
public:
    FPointOutputShortcutEdge(unsigned int a, unsigned int b, double c, unsigned int d, unsigned int e);
    unsigned int middleNode;
};


#endif //CONTRACTION_HIERARCHIES_FPOINTOUTPUTSHORTCUTEDGE_H
