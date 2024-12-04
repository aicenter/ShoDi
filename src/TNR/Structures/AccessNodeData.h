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
// Created on: 05.08.19
//

#ifndef CONTRACTION_HIERARCHIES_ACCESSNODEDATA_H
#define CONTRACTION_HIERARCHIES_ACCESSNODEDATA_H

/**
 * Auxiliary class containing data about one access node. This is basically a named pair containing the ID of the
// access node and the distance to it.
 */
class AccessNodeData {
public:
    /**
     * A simple constructor to initialize an access node.
     *
     * @param a[in] The new access node ID.
     * @param b[in] The distance to the new access node.
     */
    AccessNodeData(
            unsigned int a,
            unsigned int b);

    unsigned int accessNodeID;
    unsigned int distanceToNode;

    bool operator==(const AccessNodeData &rhs) const = default;
};


#endif //CONTRACTION_HIERARCHIES_ACCESSNODEDATA_H
