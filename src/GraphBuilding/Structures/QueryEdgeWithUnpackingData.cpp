//
// Author: Xenty (Michal Cvach)
// Created on: 07.08.19
//

#include "QueryEdgeWithUnpackingData.h"

QueryEdgeWithUnpackingData::QueryEdgeWithUnpackingData(const unsigned int a, const long long unsigned int b, const bool c, const bool d, const unsigned int mNode) : weight(b), targetNode(a), forward(c), backward(d), middleNode(mNode) {

}
QueryEdgeWithUnpackingData::QueryEdgeWithUnpackingData(const QueryEdgeWithUnpackingData & x) : weight(x.weight), targetNode(x.targetNode), forward(x.forward), backward(x.backward), middleNode(x.middleNode) {

}