//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#include <climits>
#include "FlagsGraph.h"

//______________________________________________________________________________________________________________________
FlagsGraph::FlagsGraph(unsigned int n) {
    neighbours.resize(n);
    nodesData.resize(n);
}


//______________________________________________________________________________________________________________________
void FlagsGraph::addEdge(unsigned int from, unsigned int to, long long unsigned int weight, bool fw, bool bw) {
    neighbours.at(from).push_back(QueryEdge(to, weight, fw, bw));
}

//______________________________________________________________________________________________________________________
const unsigned int FlagsGraph::nodes() const {
    return neighbours.size();
}

// Returns all neighbours of a given node 'x'. Keep in mind that this returns neighbour both in the forward and
// backward direction, so we always have to check if the neighbour is in the correct direction when expanding a node.
//______________________________________________________________________________________________________________________
const vector< QueryEdge > & FlagsGraph::nextNodes(const unsigned int x)const {
    return neighbours.at(x);
}

// Returns the data for a certain node.
//______________________________________________________________________________________________________________________
NodeData & FlagsGraph::data(unsigned int node) {
    return nodesData[node];
}

//______________________________________________________________________________________________________________________
void FlagsGraph::resetForwardInfo(const unsigned int node) {
    nodesData[node].forwardDist = ULLONG_MAX;
    nodesData[node].forwardSettled = false;
    nodesData[node].forwardReached = false;

}

//______________________________________________________________________________________________________________________
void FlagsGraph::resetBackwardInfo(const unsigned int node) {
    nodesData[node].backwardDist = ULLONG_MAX;
    nodesData[node].backwardSettled = false;
    nodesData[node].backwardReached = false;
}

//______________________________________________________________________________________________________________________
void FlagsGraph::resetForwardStall(const unsigned int node) {
    nodesData[node].forwardStalled = false;
}

//______________________________________________________________________________________________________________________
void FlagsGraph::resetBackwardStall(const unsigned int node) {
    nodesData[node].backwardStalled = false;
}