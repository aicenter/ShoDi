//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTOUTPUTEDGE_H
#define CONTRACTION_HIERARCHIES_FPOINTOUTPUTEDGE_H


class FPointOutputEdge {
public:
    FPointOutputEdge(unsigned int a, unsigned int b, double c, unsigned int d);
    double weight;
    unsigned int sourceNode;
    unsigned int targetNode;
    unsigned int flags;
};


#endif //CONTRACTION_HIERARCHIES_FPOINTOUTPUTEDGE_H
