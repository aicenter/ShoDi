//
// Author: Xenty (Michal Cvach)
// Created on: 20.8.18
//

#ifndef TRANSIT_NODE_ROUTING_SIMPLEGRAPH_H
#define TRANSIT_NODE_ROUTING_SIMPLEGRAPH_H

#include <vector>
#include <map>

using namespace std;

// Class representing a simple graph - Simple graph is a graph where there only exists at most one edge between each
// pair of nodes. Simple graph doesn't contain multiple edges (parallel edges). In our case, if there are more edges
// than one between two nodes in the original graph, we only preserve the one with the lowest weight.
//______________________________________________________________________________________________________________________
class SimpleGraph{
private:
    vector< map < unsigned int, long long unsigned int > > followingNodes;
public:
    SimpleGraph(unsigned int n);
    void addEdge(unsigned int from, unsigned int to, long long unsigned int weight);
    const unsigned int nodes() const;
    const map<unsigned int, long long unsigned int> & edges(const unsigned int x)const;
};


#endif //TRANSIT_NODE_ROUTING_SIMPLEGRAPH_H
