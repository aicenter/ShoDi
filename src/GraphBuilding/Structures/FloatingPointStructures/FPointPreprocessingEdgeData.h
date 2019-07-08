//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTPREPROCESSINGEDGEDATA_H
#define CONTRACTION_HIERARCHIES_FPOINTPREPROCESSINGEDGEDATA_H


// This structure represents one edge during the preprocessing phase. We have to keep track of whether the edge
// is a shortcut or not and if it's a shortcut we also need to know it's middle node.
//______________________________________________________________________________________________________________________
class FPointPreprocessingEdgeData {
public:
    FPointPreprocessingEdgeData(double b, const unsigned int c, const bool d);
    double weight;
    unsigned int middleNode;
    bool isShortcut;
};

#endif //CONTRACTION_HIERARCHIES_FPOINTPREPROCESSINGEDGEDATA_H
