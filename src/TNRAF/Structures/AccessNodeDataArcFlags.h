/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

//
// Author: Xenty (Michal Cvach)
// Created on: 30.11.19
//

#ifndef CONTRACTION_HIERARCHIES_ACCESSNODEDATAARCFLAGS_H
#define CONTRACTION_HIERARCHIES_ACCESSNODEDATAARCFLAGS_H


#include <vector>
#include <cstdint>
#include "../../TNR/Structures/AccessNodeData.h"



/**
 * Extension of the AccessNodeData class. In this case, each instance represents an access node, for the Arc Flags we
 * need the actual flags which are stored in a std::vector of bools.
 */
class AccessNodeDataArcFlags : public AccessNodeData {
public:
    /**
     *
     * @param a[in] The ID for the access node.
     * @param b[in] The distance to the access node.
     * @param regionsCnt[in] The amount of regions in the graph (determines how big flag std::vector will be needed for
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
     * @param regionsCnt[in] The amount of regions in the graph (determines how big flag std::vector will be needed for
     * Arc Flags)
     * @param regFlags[in] The Arc Flags for the access node stored in a 32 bit unsigned int variable.
     * @param powersOf2[in] Auxiliary std::vector with values needed for extraction of the actual Arc FLags from the regFlags
     * variable.
     */
    AccessNodeDataArcFlags(
            unsigned int a,
            unsigned int b,
            unsigned int regionsCnt,
            uint32_t regFlags,
            std::vector<unsigned int> & powersOf2);

    std::vector<bool> regionFlags;
};


#endif //CONTRACTION_HIERARCHIES_ACCESSNODEDATAARCFLAGS_H
