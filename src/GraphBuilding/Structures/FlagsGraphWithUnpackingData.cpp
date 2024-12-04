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
// Created on: 30.8.18
//

#include <climits>
#include <boost/numeric/conversion/cast.hpp>
#include "FlagsGraphWithUnpackingData.h"
#include "constants_defines.h"

//______________________________________________________________________________________________________________________
FlagsGraphWithUnpackingData::FlagsGraphWithUnpackingData(unsigned int n) {
    neighbours.resize(n);
    nodesData.resize(n);
    forwardPrev.resize(n, UINT_MAX);
    backwardPrev.resize(n, UINT_MAX);
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::addEdge(unsigned int from, unsigned int to, unsigned int weight, bool fw, bool bw, unsigned int mNode) {
    neighbours.at(from).push_back(QueryEdgeWithUnpackingData(to, weight, fw, bw, mNode));
}

//______________________________________________________________________________________________________________________
unsigned int FlagsGraphWithUnpackingData::nodes() const {
    return boost::numeric_cast<unsigned int>(neighbours.size());
}

// Returns all neighbours of a given node 'x'. Keep in mind that this returns neighbour both in the forward and
// backward direction, so we always have to check if the neighbour is in the correct direction when expanding a node.
//______________________________________________________________________________________________________________________
const std::vector< QueryEdgeWithUnpackingData > & FlagsGraphWithUnpackingData::nextNodes(const unsigned int x)const {
    return neighbours.at(x);
}

// Returns the data for a certain node.
//______________________________________________________________________________________________________________________
NodeData & FlagsGraphWithUnpackingData::data(unsigned int node) {
    return nodesData[node];
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::resetForwardInfo(const unsigned int node) {
    nodesData[node].forwardDist = UINT_MAX;
    nodesData[node].forwardSettled = false;
    nodesData[node].forwardReached = false;

}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::resetBackwardInfo(const unsigned int node) {
    nodesData[node].backwardDist = UINT_MAX;
    nodesData[node].backwardSettled = false;
    nodesData[node].backwardReached = false;
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::resetForwardStall(const unsigned int node) {
    nodesData[node].forwardStalled = false;
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::resetBackwardStall(const unsigned int node) {
    nodesData[node].backwardStalled = false;
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::setForwardPrev(unsigned int x, unsigned int y) {
    forwardPrev[x] = y;
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::setBackwardPrev(unsigned int x, unsigned int y) {
    backwardPrev[x] = y;
}

//______________________________________________________________________________________________________________________
unsigned int FlagsGraphWithUnpackingData::getForwardPrev(unsigned int x) {
    return forwardPrev[x];
}

//______________________________________________________________________________________________________________________
unsigned int FlagsGraphWithUnpackingData::getBackwardPrev(unsigned int x) {
    return backwardPrev[x];
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::resetForwardPrev(unsigned int x) {
    forwardPrev[x] = UINT_MAX;
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::resetBackwardPrev(unsigned int x) {
    backwardPrev[x] = UINT_MAX;
}

//______________________________________________________________________________________________________________________
unsigned int FlagsGraphWithUnpackingData::getMiddleNode(unsigned int source, unsigned int target, bool direction) {
    for(size_t i = 0; i < neighbours[source].size(); i++) {
        if (neighbours[source][i].targetNode == target) {
            if (direction == FORWARD && neighbours[source][i].forward) {
                return neighbours[source][i].middleNode;
            } else if (direction == BACKWARD && neighbours[source][i].backward) {
                return neighbours[source][i].middleNode;
            }
        }
    }

    return UINT_MAX;
}

//______________________________________________________________________________________________________________________
unsigned int FlagsGraphWithUnpackingData::getDistance(unsigned int node1, unsigned int node2, bool direction) {
    unsigned int source = node1;
    unsigned int target = node2;
    if (nodesData[source].rank > nodesData[target].rank) {
        source = node2;
        target = node1;
    }

    for(size_t i = 0; i < neighbours[source].size(); i++) {
        if (neighbours[source][i].targetNode == target) {
            if (direction == FORWARD && neighbours[source][i].forward) {
                return neighbours[source][i].weight;
            } else if (direction == BACKWARD && neighbours[source][i].backward) {
                return neighbours[source][i].weight;
            }

        }
    }

    return UINT_MAX;

}
