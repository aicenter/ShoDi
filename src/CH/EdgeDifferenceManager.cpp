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
// Created on: 1.8.18
//

#include <climits>
#include <cstdio>
#include "EdgeDifferenceManager.h"

//______________________________________________________________________________________________________________________
std::vector<unsigned int> EdgeDifferenceManager::neighboursContracted(0);
std::vector<unsigned int> EdgeDifferenceManager::previousContracted(0);

//______________________________________________________________________________________________________________________
void EdgeDifferenceManager::init(const unsigned int nodes) {
    previousContracted.resize(nodes, UINT_MAX);
    neighboursContracted.resize(nodes, 0);
}

//______________________________________________________________________________________________________________________
int EdgeDifferenceManager::difference(const unsigned int contractedNode, const unsigned int x, const unsigned int possibleShortcuts, const unsigned int degree) {
    if (contractedNode != UINT_MAX) {
        if (contractedNode != previousContracted[x]) {
            neighboursContracted[x]++;
            previousContracted[x] = contractedNode;
        }
    }

    return neighboursContracted.at(x) + possibleShortcuts - degree;
}