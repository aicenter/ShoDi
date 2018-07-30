//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#ifndef TRANSIT_NODE_ROUTING_GRAPH_H
#define TRANSIT_NODE_ROUTING_GRAPH_H

#include <utility>
#include <vector>

using namespace std;

class Graph{
private:
    vector< vector < pair< unsigned int, unsigned int > > > content;
public:
    Graph(unsigned int n);
    void addEdge(unsigned int from, unsigned int to, unsigned int weight);
    const unsigned int nodes() const;
    const vector<pair<unsigned int, unsigned int>> & neighnours(const unsigned int x)const;
};

#endif //TRANSIT_NODE_ROUTING_GRAPH_H
