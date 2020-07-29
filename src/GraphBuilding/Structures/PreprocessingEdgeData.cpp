//
// Author: Xenty (Michal Cvach)
// Created on: 29.8.18
//

#include "PreprocessingEdgeData.h"

//______________________________________________________________________________________________________________________
PreprocessingEdgeData::PreprocessingEdgeData(const dist_t b, const unsigned int c, const bool d) : weight(b), middleNode(c), isShortcut(d) {

}

//______________________________________________________________________________________________________________________
PreprocessingEdgeData::PreprocessingEdgeData(const PreprocessingEdgeData & data) : weight(data.weight), middleNode(data.middleNode), isShortcut(data.isShortcut) {

}
