//
// Author: Xenty (Michal Cvach)
// Created on: 20.8.18
//

#include "SimpleGraph.h"

//______________________________________________________________________________________________________________________
SimpleGraph::SimpleGraph(unsigned int n) {
    this->followingNodes.resize(n);
}

// Only add edges if they don't exist already. If an edge exists already, check it weight and possibly adjust it.
//______________________________________________________________________________________________________________________
void SimpleGraph::addEdge(unsigned int from, unsigned int to, long long unsigned int weight) {
    if (followingNodes.at(from).count(to) == 1) {
        if (followingNodes.at(from).at(to) > weight) {
            followingNodes.at(from).at(to) = weight;
        }
    } else {
        followingNodes.at(from).insert(make_pair(to, weight));
    }
}

//______________________________________________________________________________________________________________________
const unsigned int SimpleGraph::nodes()const {
    return this->followingNodes.size();
}

//______________________________________________________________________________________________________________________
const map<unsigned int, long long unsigned int> & SimpleGraph::edges(const unsigned int x)const {
    return this->followingNodes.at(x);
}

