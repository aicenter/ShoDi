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
    neighbours.at(from).push_back(Edge(to, weight, fw, bw));
}

//______________________________________________________________________________________________________________________
const unsigned int FlagsGraph::nodes() const {
    return neighbours.size();
}

//______________________________________________________________________________________________________________________
const vector< Edge > & FlagsGraph::nextNodes(const unsigned int x)const {
    return neighbours.at(x);
}


//______________________________________________________________________________________________________________________
const unsigned int FlagsGraph::degree(unsigned int node)const {
    unsigned int degree = 0;
    for(auto iter = neighbours.at(node).begin(); iter != neighbours.at(node).end(); ++iter) {
        if ((*iter).backward) {
            degree++;
        }
        if ((*iter).forward) {
            degree++;
        }
    }
    return degree;

}

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