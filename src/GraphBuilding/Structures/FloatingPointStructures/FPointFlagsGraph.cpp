//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#include "FPointFlagsGraph.h"
#include <limits>

//______________________________________________________________________________________________________________________
FPointFlagsGraph::FPointFlagsGraph(unsigned int n) {
    neighbours.resize(n);
    nodesData.resize(n);
}


//______________________________________________________________________________________________________________________
void FPointFlagsGraph::addEdge(unsigned int from, unsigned int to, double weight, bool fw, bool bw) {
    neighbours.at(from).push_back(FPointQueryEdge(to, weight, fw, bw));
}

//______________________________________________________________________________________________________________________
const unsigned int FPointFlagsGraph::nodes() const {
    return neighbours.size();
}

// Returns all neighbours of a given node 'x'. Keep in mind that this returns neighbour both in the forward and
// backward direction, so we always have to check if the neighbour is in the correct direction when expanding a node.
//______________________________________________________________________________________________________________________
const vector< FPointQueryEdge > & FPointFlagsGraph::nextNodes(const unsigned int x)const {
    return neighbours.at(x);
}

// Returns the data for a certain node.
//______________________________________________________________________________________________________________________
FPointNodeData & FPointFlagsGraph::data(unsigned int node) {
    return nodesData[node];
}

//______________________________________________________________________________________________________________________
void FPointFlagsGraph::resetForwardInfo(const unsigned int node) {
    nodesData[node].forwardDist = numeric_limits<double>::max();
    nodesData[node].forwardSettled = false;
    nodesData[node].forwardReached = false;

}

//______________________________________________________________________________________________________________________
void FPointFlagsGraph::resetBackwardInfo(const unsigned int node) {
    nodesData[node].backwardDist = numeric_limits<double>::max();
    nodesData[node].backwardSettled = false;
    nodesData[node].backwardReached = false;
}

//______________________________________________________________________________________________________________________
void FPointFlagsGraph::resetForwardStall(const unsigned int node) {
    nodesData[node].forwardStalled = false;
}

//______________________________________________________________________________________________________________________
void FPointFlagsGraph::resetBackwardStall(const unsigned int node) {
    nodesData[node].backwardStalled = false;
}