//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTSHORTCUTEDGE_H
#define CONTRACTION_HIERARCHIES_FPOINTSHORTCUTEDGE_H


// This structure is used during the preprocessing phase to keep track of all the created shortcuts.
// Each instance represents one shortcut with all it's info necessary for additional preprocessing.
//______________________________________________________________________________________________________________________
class FPointShortcutEdge {
public:
    FPointShortcutEdge(unsigned int a, unsigned int b, double c, unsigned int d);
    double weight;
    unsigned int sourceNode;
    unsigned int targetNode;
    unsigned int middleNode;
};


#endif //CONTRACTION_HIERARCHIES_FPOINTSHORTCUTEDGE_H
