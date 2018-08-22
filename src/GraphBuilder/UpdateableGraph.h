//
// Author: Xenty (Michal Cvach)
// Created on: 21.8.18
//

#ifndef TRANSIT_NODE_ROUTING_UPDATEABLEGRAPH_H
#define TRANSIT_NODE_ROUTING_UPDATEABLEGRAPH_H

#include <vector>
#include <unordered_map>

using namespace std;

class UpdateableGraph{
protected:
    vector< unordered_map < unsigned int, long long unsigned int > > followingNodes;
    vector< unordered_map < unsigned int, long long unsigned int > > previousNodes;
public:
    UpdateableGraph(unsigned int n);
    bool addEdge(unsigned int from, unsigned int to, long long unsigned int weight);
    void removeEdge(unsigned int from, unsigned int to);
    const unsigned int nodes() const;
    const unordered_map<unsigned int, long long unsigned int> & incomingEdges(const unsigned int x)const;
    const unordered_map<unsigned int, long long unsigned int> & outgoingEdges(const unsigned int x)const;
    const unsigned int degree(unsigned int node)const;
};




#endif //TRANSIT_NODE_ROUTING_UPDATEABLEGRAPH_H
