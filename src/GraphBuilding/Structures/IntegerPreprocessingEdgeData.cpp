//
// Author: Xenty (Michal Cvach)
// Created on: 29.8.18
//

#include "IntegerPreprocessingEdgeData.h"

//______________________________________________________________________________________________________________________
IntegerPreprocessingEdgeData::IntegerPreprocessingEdgeData(const long long unsigned int b, const unsigned int c, const bool d) : weight(b), middleNode(c), isShortcut(d) {

}

//______________________________________________________________________________________________________________________
IntegerPreprocessingEdgeData::IntegerPreprocessingEdgeData(const IntegerPreprocessingEdgeData & data) : weight(data.weight), middleNode(data.middleNode), isShortcut(data.isShortcut) {

}