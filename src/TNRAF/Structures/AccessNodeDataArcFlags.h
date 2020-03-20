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

// Extension of the AccessNodeData class. In this case, each instance represents an access node, for the Arc Flags we
// need the actual flags which are stored in a vector of bools.
//______________________________________________________________________________________________________________________
class AccessNodeDataArcFlags : public AccessNodeData {
public:
    AccessNodeDataArcFlags(unsigned int a, unsigned int b, unsigned int regionsCnt);
    AccessNodeDataArcFlags(unsigned int a, unsigned int b, unsigned int regionsCnt, uint32_t regFlags, vector<unsigned int> & powersOf2);
    vector<bool> regionFlags;
};


#endif //CONTRACTION_HIERARCHIES_ACCESSNODEDATAARCFLAGS_H
