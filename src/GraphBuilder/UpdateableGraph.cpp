//
// Author: Xenty (Michal Cvach)
// Created on: 21.8.18
//

#include "UpdateableGraph.h"

//______________________________________________________________________________________________________________________
UpdateableGraph::UpdateableGraph(unsigned int n) {
    this->followingNodes.resize(n);
    this->previousNodes.resize(n);
}

//______________________________________________________________________________________________________________________
bool UpdateableGraph::addEdge(unsigned int from, unsigned int to, long long unsigned int weight) {
    if (followingNodes.at(from).count(to) == 1) {
        if (followingNodes.at(from).at(to) > weight) {
            followingNodes.at(from).at(to) = weight;
            previousNodes.at(to).at(from) = weight;
            return true;
        }
        return false;
    } else {
        followingNodes.at(from).insert(make_pair(to, weight));
        previousNodes.at(to).insert(make_pair(from, weight));
        return true;
    }
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::removeEdge(unsigned int from, unsigned int to) {
    followingNodes.at(from).erase(to);
    previousNodes.at(to).erase(from);
}

//______________________________________________________________________________________________________________________
const unsigned int UpdateableGraph::nodes() const {
    return followingNodes.size();
}

//______________________________________________________________________________________________________________________
const unordered_map<unsigned int, long long unsigned int> & UpdateableGraph::incomingEdges(const unsigned int x)const {
    return previousNodes.at(x);
}

//______________________________________________________________________________________________________________________
const unordered_map<unsigned int, long long unsigned int> & UpdateableGraph::outgoingEdges(const unsigned int x)const {
    return followingNodes.at(x);
}

//______________________________________________________________________________________________________________________
const unsigned int UpdateableGraph::degree(unsigned int node)const {
    return followingNodes.at(node).size() + previousNodes.at(node).size();
}