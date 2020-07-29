//
// Author: Xenty (Michal Cvach)
// Created on: 20.8.18
//

#include "SimpleGraph.h"

//______________________________________________________________________________________________________________________
SimpleGraph::SimpleGraph(unsigned int n) {
    this->followingNodes.resize(n);
}

//______________________________________________________________________________________________________________________
void SimpleGraph::addEdge(unsigned int from, unsigned int to, dist_t weight) {
    if (followingNodes.at(from).count(to) == 1) {
        if (followingNodes.at(from).at(to) > weight) {
            followingNodes.at(from).at(to) = weight;
        }
    } else {
        followingNodes.at(from).insert(make_pair(to, weight));
    }
}

//______________________________________________________________________________________________________________________
unsigned int SimpleGraph::nodes()const {
    return this->followingNodes.size();
}

//______________________________________________________________________________________________________________________
const map<unsigned int, dist_t> & SimpleGraph::edges(const unsigned int x)const {
    return this->followingNodes.at(x);
}

