//
// Author: Xenty (Michal Cvach)
// Created on: 20.8.18
//

#include "SimpleGraph.h"
#include <boost/numeric/conversion/cast.hpp>

//______________________________________________________________________________________________________________________
SimpleGraph::SimpleGraph(unsigned int n) {
    this->followingNodes.resize(n);
}

//______________________________________________________________________________________________________________________
bool SimpleGraph::addEdge(unsigned int from, unsigned int to, dist_t weight) {
    if (followingNodes.at(from).count(to) == 1) {
        if (followingNodes.at(from).at(to) > weight) {
            followingNodes.at(from).at(to) = weight;
        }
    } else {
        followingNodes.at(from).insert(make_pair(to, weight));
    }

    return true;
}

//______________________________________________________________________________________________________________________
unsigned int SimpleGraph::nodes()const {
    return boost::numeric_cast<unsigned int>(this->followingNodes.size());
}

//______________________________________________________________________________________________________________________
const map<unsigned int, dist_t> & SimpleGraph::edges(const unsigned int x)const {
    return this->followingNodes.at(x);
}

