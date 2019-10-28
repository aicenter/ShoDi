//
// Author: Xenty (Michal Cvach)
// Created on: 28.10.19
//

#ifndef CONTRACTION_HIERARCHIES_SIMPLEEDGE_H
#define CONTRACTION_HIERARCHIES_SIMPLEEDGE_H

//______________________________________________________________________________________________________________________
class SimpleEdge {
public:
    SimpleEdge(unsigned int f, unsigned int t);
    SimpleEdge(const SimpleEdge & se);
    unsigned int from;
    unsigned int to;
};


#endif //CONTRACTION_HIERARCHIES_SIMPLEEDGE_H
