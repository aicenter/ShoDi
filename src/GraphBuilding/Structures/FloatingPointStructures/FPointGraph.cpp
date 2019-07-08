//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#include "FPointGraph.h"

//______________________________________________________________________________________________________________________
FPointGraph::FPointGraph(unsigned int n) {
    this->followingNodes.resize(n);
    this->previousNodes.resize(n);
}

//______________________________________________________________________________________________________________________
FPointGraph::FPointGraph(FPointSimpleGraph & x) {
    const unsigned int nodes = x.nodes();
    this->followingNodes.resize(nodes);
    this->previousNodes.resize(nodes);

    for(unsigned int i = 0; i < nodes; i++) {
        for(auto iter = x.edges(i).begin(); iter != x.edges(i).end(); ++iter) {
            this->addEdge(i, (*iter).first, (*iter).second);
        }
    }

}

//______________________________________________________________________________________________________________________
void FPointGraph::addEdge(unsigned int from, unsigned int to, double weight) {
    this->followingNodes.at(from).push_back(make_pair(to, weight));
    this->previousNodes.at(to).push_back(make_pair(from, weight));
}

//______________________________________________________________________________________________________________________
const unsigned int FPointGraph::nodes()const {
    return this->followingNodes.size();
}

//______________________________________________________________________________________________________________________
const vector<pair<unsigned int, double>> & FPointGraph::incomingEdges(const unsigned int x)const {
    return this->previousNodes.at(x);
}

//______________________________________________________________________________________________________________________
const vector<pair<unsigned int, double>> & FPointGraph::outgoingEdges(const unsigned int x)const {
    return this->followingNodes.at(x);
}
