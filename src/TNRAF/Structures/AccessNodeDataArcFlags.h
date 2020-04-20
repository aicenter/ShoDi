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

/**
 * Extension of the AccessNodeData class. In this case, each instance represents an access node, for the Arc Flags we
 * need the actual flags which are stored in a vector of bools.
 */
class AccessNodeDataArcFlags : public AccessNodeData {
public:
    /**
     *
     * @param a[in] The ID for the access node.
     * @param b[in] The distance to the access node.
     * @param regionsCnt[in] The amount of regions in the graph (determines how big flag vector will be needed for
     * Arc Flags)
     */
    AccessNodeDataArcFlags(
            unsigned int a,
            unsigned int b,
            unsigned int regionsCnt);

    /**
     * Initializes the arc flags based on the regFlags input variable.
     *
     * @param a[in] The ID for the access node.
     * @param b[in] The distance to the access node.
     * @param regionsCnt[in] The amount of regions in the graph (determines how big flag vector will be needed for
     * Arc Flags)
     * @param regFlags[in] The Arc Flags for the access node stored in a 32 bit unsigned int variable.
     * @param powersOf2[in] Auxiliary vector with values needed for extraction of the actual Arc FLags from the regFlags
     * variable.
     */
    AccessNodeDataArcFlags(
            unsigned int a,
            unsigned int b,
            unsigned int regionsCnt,
            uint32_t regFlags,
            vector<unsigned int> & powersOf2);

    vector<bool> regionFlags;
};


#endif //CONTRACTION_HIERARCHIES_ACCESSNODEDATAARCFLAGS_H
