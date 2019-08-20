//
// Author: Xenty (Michal Cvach)
// Created on: 30.8.18
//

#include <climits>
#include "IntegerFlagsGraphWithUnpackingData.h"
#include "../constants_defines.h"

//______________________________________________________________________________________________________________________
IntegerFlagsGraphWithUnpackingData::IntegerFlagsGraphWithUnpackingData(unsigned int n) {
    neighbours.resize(n);
    nodesData.resize(n);
    forwardPrev.resize(n, UINT_MAX);
    backwardPrev.resize(n, UINT_MAX);
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::addEdge(unsigned int from, unsigned int to, long long unsigned int weight, bool fw, bool bw, unsigned int mNode) {
    neighbours.at(from).push_back(IntegerQueryEdgeWithUnpackingData(to, weight, fw, bw, mNode));
    if(to == 4294967295) {
        printf("Adding edge from %u to %u. (Weight %llu, fw: %s, bw: %s, middle node: %u)\n", from, to, weight, fw ? "true" : "false", bw ? "true" : "false", mNode);
    }
}

//______________________________________________________________________________________________________________________
const unsigned int IntegerFlagsGraphWithUnpackingData::nodes() const {
    return neighbours.size();
}

// Returns all neighbours of a given node 'x'. Keep in mind that this returns neighbour both in the forward and
// backward direction, so we always have to check if the neighbour is in the correct direction when expanding a node.
//______________________________________________________________________________________________________________________
const vector< IntegerQueryEdgeWithUnpackingData > & IntegerFlagsGraphWithUnpackingData::nextNodes(const unsigned int x)const {
    return neighbours.at(x);
}

// Returns the data for a certain node.
//______________________________________________________________________________________________________________________
IntegerNodeData & IntegerFlagsGraphWithUnpackingData::data(unsigned int node) {
    return nodesData[node];
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::resetForwardInfo(const unsigned int node) {
    nodesData[node].forwardDist = ULLONG_MAX;
    nodesData[node].forwardSettled = false;
    nodesData[node].forwardReached = false;

}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::resetBackwardInfo(const unsigned int node) {
    nodesData[node].backwardDist = ULLONG_MAX;
    nodesData[node].backwardSettled = false;
    nodesData[node].backwardReached = false;
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::resetForwardStall(const unsigned int node) {
    nodesData[node].forwardStalled = false;
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::resetBackwardStall(const unsigned int node) {
    nodesData[node].backwardStalled = false;
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::setForwardPrev(unsigned int x, unsigned int y) {
    forwardPrev[x] = y;
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::setBackwardPrev(unsigned int x, unsigned int y) {
    backwardPrev[x] = y;
}

//______________________________________________________________________________________________________________________
unsigned int IntegerFlagsGraphWithUnpackingData::getForwardPrev(unsigned int x) {
    return forwardPrev[x];
}

//______________________________________________________________________________________________________________________
unsigned int IntegerFlagsGraphWithUnpackingData::getBackwardPrev(unsigned int x) {
    return backwardPrev[x];
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::resetForwardPrev(unsigned int x) {
    forwardPrev[x] = UINT_MAX;
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::resetBackwardPrev(unsigned int x) {
    backwardPrev[x] = UINT_MAX;
}

//______________________________________________________________________________________________________________________
unsigned int IntegerFlagsGraphWithUnpackingData::getMiddleNode(unsigned int source, unsigned int target, bool direction) {
    for(unsigned int i = 0; i < neighbours[source].size(); i++) {
        if (neighbours[source][i].targetNode == target) {
            //printf("Possible middle node: %u between %u and %u, direction: %s (neigbour forward: %s, backward: %s)\n", neighbours[source][i].middleNode, source, target, direction ? "forward" : "backward", neighbours[source][i].forward ? "true" : "false" , neighbours[source][i].backward ? "true" : "false");
            if (direction == FORWARD && neighbours[source][i].forward) {
                //printf("Returning: %u as middle node between %u and %u\n", neighbours[source][i].middleNode, source, target);
                return neighbours[source][i].middleNode;
            } else if (direction == BACKWARD && neighbours[source][i].backward) {
                //printf("Returning: %u as middle node between %u and %u\n", neighbours[source][i].middleNode, source, target);
                return neighbours[source][i].middleNode;
            }
        }
    }

    return UINT_MAX;
}

//______________________________________________________________________________________________________________________
unsigned int IntegerFlagsGraphWithUnpackingData::getDistance(unsigned int node1, unsigned int node2, bool direction) {
    unsigned int source = node1;
    unsigned int target = node2;
    if (nodesData[source].rank > nodesData[target].rank) {
        source = node2;
        target = node1;
    }

    for(unsigned int i = 0; i < neighbours[source].size(); i++) {
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
