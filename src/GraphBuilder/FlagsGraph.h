//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#ifndef TRANSIT_NODE_ROUTING_FLAGSGRAPH_H
#define TRANSIT_NODE_ROUTING_FLAGSGRAPH_H

#include <utility>
#include <vector>
#include "Edge.h"
#include "../CH/Structures/NodeData.h"

using namespace std;

// Class representing a graph for specifically for the CH query algorithm.
//______________________________________________________________________________________________________________________
class FlagsGraph{
private:
    vector< vector < Edge > > neighbours;
    vector< NodeData > nodesData;
public:
    FlagsGraph(unsigned int n);
    void addEdge(unsigned int from, unsigned int to, long long unsigned int weight, bool fw, bool bw);
    const unsigned int nodes() const;
    const vector< Edge > & nextNodes(const unsigned int x)const;
    const unsigned int degree(unsigned int node)const;
    NodeData & data(unsigned int node);
    void resetForwardInfo(const unsigned int node);
    void resetBackwardInfo(const unsigned int node);
    void resetForwardStall(const unsigned int node);
    void resetBackwardStall(const unsigned int node);
};


#endif //TRANSIT_NODE_ROUTING_FLAGSGRAPH_H
