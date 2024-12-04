/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

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
