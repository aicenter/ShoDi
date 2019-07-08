//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTNODEDATA_H
#define CONTRACTION_HIERARCHIES_FPOINTNODEDATA_H


// A simple structure encapsulating all the important information about a node for the Contraction Hierarchies
// query algorithm.
//______________________________________________________________________________________________________________________
class FPointNodeData {
public:
    FPointNodeData();
    double forwardDist;
    double backwardDist;
    unsigned int rank;
    bool forwardReached;
    bool backwardReached;
    bool forwardSettled;
    bool backwardSettled;
    bool forwardStalled;
    bool backwardStalled;
};

#endif //CONTRACTION_HIERARCHIES_FPOINTNODEDATA_H
