//
// Author: Xenty (Michal Cvach)
// Created on: 30.11.19
//

#ifndef CONTRACTION_HIERARCHIES_ACCESSNODEDATAARCFLAGS_H
#define CONTRACTION_HIERARCHIES_ACCESSNODEDATAARCFLAGS_H


#include <vector>
#include <cstdint>
#include "../../TNR/Structures/AccessNodeData.h"

using namespace std;

//______________________________________________________________________________________________________________________
class AccessNodeDataArcFlags : public AccessNodeData {
public:
    AccessNodeDataArcFlags(unsigned int a, unsigned int b, unsigned int regionsCnt);
    AccessNodeDataArcFlags(unsigned int a, unsigned int b, unsigned int regionsCnt, uint32_t regFlags, vector<unsigned int> & powersOf2);
    vector<bool> regionFlags;
};


#endif //CONTRACTION_HIERARCHIES_ACCESSNODEDATAARCFLAGS_H
