//
// Created by xenty on 8.7.19.
//

#include "FPointSimpleGraph.h"

//______________________________________________________________________________________________________________________
FPointSimpleGraph::FPointSimpleGraph(unsigned int n) {
    this->followingNodes.resize(n);
}

// Only add edges if they don't exist already. If an edge exists already, check it weight and possibly adjust it.
//______________________________________________________________________________________________________________________
void FPointSimpleGraph::addEdge(unsigned int from, unsigned int to, double weight) {
    if (followingNodes.at(from).count(to) == 1) {
        if (followingNodes.at(from).at(to) > weight) {
            followingNodes.at(from).at(to) = weight;
        }
    } else {
        followingNodes.at(from).insert(make_pair(to, weight));
    }
}

//______________________________________________________________________________________________________________________
const unsigned int FPointSimpleGraph::nodes()const {
    return this->followingNodes.size();
}

//______________________________________________________________________________________________________________________
const map<unsigned int, double> & FPointSimpleGraph::edges(const unsigned int x)const {
    return this->followingNodes.at(x);
}

