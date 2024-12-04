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
// Created on: 31.07.19
//

#include "../GraphBuilding/Loaders/XenGraphLoader.h"
#include "CHDistanceQueryManagerWithMapping.h"

// Initializes the query manager. Here, the mapping from the original indices to our indices is loaded.
//______________________________________________________________________________________________________________________
CHDistanceQueryManagerWithMapping::CHDistanceQueryManagerWithMapping(FlagsGraph<NodeData>& g, std::string mappingFilepath) : qm(g) {
    XenGraphLoader mappingLoader(mappingFilepath);
    mappingLoader.loadNodesMapping(mapping);
}

// Used to find the shortest distance from start to goal where start and goal are the original indices.
//______________________________________________________________________________________________________________________
unsigned int CHDistanceQueryManagerWithMapping::findDistance(const long long unsigned int start, const long long unsigned int goal) {
    return qm.findDistance(mapping.at(start), mapping.at(goal));
}