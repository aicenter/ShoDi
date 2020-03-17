//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#include "QueryEdge.h"

//______________________________________________________________________________________________________________________
QueryEdge::QueryEdge(const unsigned int a, const long long unsigned int b, const bool c, const bool d) : weight(b), targetNode(a), forward(c), backward(d) {

}

//______________________________________________________________________________________________________________________
QueryEdge::QueryEdge(const QueryEdge & x) : weight(x.weight), targetNode(x.targetNode), forward(x.forward), backward(x.backward) {

}