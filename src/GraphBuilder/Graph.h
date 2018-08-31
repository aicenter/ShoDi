//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#ifndef TRANSIT_NODE_ROUTING_GRAPH_H
#define TRANSIT_NODE_ROUTING_GRAPH_H

#include <utility>
#include <vector>
#include "SimpleGraph.h"

using namespace std;

// Class representing a graph.
// This simple graph representation is used for the Dijkstra Algorithm. For the Contraction Hierarchies,
// other representation seem to work better.
//______________________________________________________________________________________________________________________
class Graph{
private:
    vector< vector < pair< unsigned int, long long unsigned int > > > followingNodes;
    vector< vector < pair< unsigned int, long long unsigned int > > > previousNodes;
public:
    Graph(unsigned int n);
    Graph(SimpleGraph & x);
    void addEdge(unsigned int from, unsigned int to, long long unsigned int weight);
    const unsigned int nodes() const;
    const vector<pair<unsigned int, long long unsigned int>> & incomingEdges(const unsigned int x)const;
    const vector<pair<unsigned int, long long unsigned int>> & outgoingEdges(const unsigned int x)const;
};

#endif //TRANSIT_NODE_ROUTING_GRAPH_H
