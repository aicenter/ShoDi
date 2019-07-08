//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTQUERYEDGE_H
#define CONTRACTION_HIERARCHIES_FPOINTQUERYEDGE_H


//______________________________________________________________________________________________________________________
class FPointQueryEdge {
public:
    FPointQueryEdge(const unsigned int a, double b, const bool c, const bool d);
    double weight;
    unsigned int targetNode;
    bool forward;
    bool backward;
};


#endif //CONTRACTION_HIERARCHIES_FPOINTQUERYEDGE_H
