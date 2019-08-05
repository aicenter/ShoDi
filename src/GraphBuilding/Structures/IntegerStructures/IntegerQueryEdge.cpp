//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#include "IntegerQueryEdge.h"

//______________________________________________________________________________________________________________________
IntegerQueryEdge::IntegerQueryEdge(const unsigned int a, const long long unsigned int b, const bool c, const bool d) : weight(b), targetNode(a), forward(c), backward(d) {

}

//______________________________________________________________________________________________________________________
IntegerQueryEdge::IntegerQueryEdge(const IntegerQueryEdge & x) : weight(x.weight), targetNode(x.targetNode), forward(x.forward), backward(x.backward) {

}