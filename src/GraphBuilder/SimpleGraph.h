//
// Author: Xenty (Michal Cvach)
// Created on: 20.8.18
//

#ifndef TRANSIT_NODE_ROUTING_SIMPLEGRAPH_H
#define TRANSIT_NODE_ROUTING_SIMPLEGRAPH_H

#include <vector>
#include <map>

using namespace std;

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
