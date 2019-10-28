//
// Author: Xenty (Michal Cvach)
// Created on: 28.10.19
//

#include "SimpleEdge.h"

//______________________________________________________________________________________________________________________
SimpleEdge::SimpleEdge(unsigned int f, unsigned int t) : from(f), to(t) {

}

//______________________________________________________________________________________________________________________
SimpleEdge::SimpleEdge(const SimpleEdge & se) : from(se.from), to(se.to) {

}