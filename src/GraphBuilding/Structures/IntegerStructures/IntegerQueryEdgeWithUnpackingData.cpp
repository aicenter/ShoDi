//
// Author: Xenty (Michal Cvach)
// Created on: 07.08.19
//

#include "IntegerQueryEdgeWithUnpackingData.h"

IntegerQueryEdgeWithUnpackingData::IntegerQueryEdgeWithUnpackingData(const unsigned int a, const long long unsigned int b, const bool c, const bool d, const unsigned int mNode) : weight(b), targetNode(a), forward(c), backward(d), middleNode(mNode) {

}
IntegerQueryEdgeWithUnpackingData::IntegerQueryEdgeWithUnpackingData(const IntegerQueryEdgeWithUnpackingData & x) : weight(x.weight), targetNode(x.targetNode), forward(x.forward), backward(x.backward), middleNode(x.middleNode) {

}