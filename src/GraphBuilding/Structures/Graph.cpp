//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include <cstdio>
#include <boost/numeric/conversion/cast.hpp>
#include "Graph.h"

//______________________________________________________________________________________________________________________
Graph::Graph(unsigned int n) {
    this->followingNodes.resize(n);
    this->previousNodes.resize(n);
}

//______________________________________________________________________________________________________________________
Graph::Graph(SimpleGraph & x) {
    const auto nodes = x.nodes();
    this->followingNodes.resize(nodes);
    this->previousNodes.resize(nodes);

    for(unsigned int i = 0; i < nodes; i++) {
        for(auto iter = x.edges(i).begin(); iter != x.edges(i).end(); ++iter) {
            this->addEdge(i, (*iter).first, (*iter).second);
        }
    }

}

//______________________________________________________________________________________________________________________
bool Graph::addEdge(unsigned int from, unsigned int to, dist_t weight) {
    this->followingNodes.at(from).push_back(std::make_pair(to, weight));
    this->previousNodes.at(to).push_back(std::make_pair(from, weight));
    return true;
}

//______________________________________________________________________________________________________________________
unsigned int Graph::nodes()const {
    return boost::numeric_cast<unsigned int>(this->followingNodes.size());
}

//______________________________________________________________________________________________________________________
const std::vector<std::pair<unsigned int, dist_t>> & Graph::incomingEdges(const unsigned int x)const {
    return this->previousNodes.at(x);
}

//______________________________________________________________________________________________________________________
const std::vector<std::pair<unsigned int, dist_t>> & Graph::outgoingEdges(const unsigned int x)const {
    return this->followingNodes.at(x);
}
