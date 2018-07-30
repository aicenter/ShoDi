//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include "Graph.h"

//______________________________________________________________________________________________________________________
Graph::Graph(unsigned int n) {
    this->content.resize(n);
}

//______________________________________________________________________________________________________________________
void Graph::addEdge(unsigned int from, unsigned int to, unsigned int weight) {
    this->content.at(from).push_back(make_pair(to, weight));
}

//______________________________________________________________________________________________________________________
const unsigned int Graph::nodes()const {
    return this->content.size();
}

//______________________________________________________________________________________________________________________
const vector<pair<unsigned int, unsigned int>> & Graph::neighnours(const unsigned int x)const {
    return this->content.at(x);
}