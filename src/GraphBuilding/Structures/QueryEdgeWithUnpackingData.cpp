//
// Author: Xenty (Michal Cvach)
// Created on: 07.08.19
//

#include "QueryEdgeWithUnpackingData.h"

//______________________________________________________________________________________________________________________
QueryEdgeWithUnpackingData::QueryEdgeWithUnpackingData(const unsigned int a, const unsigned int b, const bool c, const bool d, const unsigned int mNode) : weight(b), targetNode(a), middleNode(mNode), forward(c), backward(d) {

}

//______________________________________________________________________________________________________________________
QueryEdgeWithUnpackingData::QueryEdgeWithUnpackingData(const QueryEdgeWithUnpackingData & x) : weight(x.weight), targetNode(x.targetNode), middleNode(x.middleNode), forward(x.forward), backward(x.backward) {

}