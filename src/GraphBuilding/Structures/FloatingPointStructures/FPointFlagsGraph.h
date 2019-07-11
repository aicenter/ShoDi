//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTFLAGSGRAPH_H
#define CONTRACTION_HIERARCHIES_FPOINTFLAGSGRAPH_H

#include <vector>
#include "FPointQueryEdge.h"
#include "../../../CH/FloatingPoint/Structures/FPointNodeData.h"

using namespace std;

// Class representing a graph for specifically for the CH query algorithm.
// We use the representation suggested in the "Contraction Hierarchies: Faster and Simpler Hierarchical
// Routing in Road Networks" article by Robert Geisberger, Peter Sanders, Dominik Schultes, and Daniel Delling.
// Basically, for example Dijkstra requires undirected edges to be saved at both nodes. This representation
// only saves every edge at the incident node with the lower rank. This makes sense, because we only expand edges
// going from lower ranked nodes to higher ranked nodes in the query algorithm anyway. We store additional flags
// "forward" and "backward" with each edge to determine in which direction the edge should be expanded.
//______________________________________________________________________________________________________________________
class FPointFlagsGraph{
protected:
    vector< vector < FPointQueryEdge > > neighbours;
    vector< FPointNodeData > nodesData;
public:
    FPointFlagsGraph(unsigned int n);
    void addEdge(unsigned int from, unsigned int to, double weight, bool fw, bool bw);
    const unsigned int nodes() const;
    const vector< FPointQueryEdge > & nextNodes(const unsigned int x)const;
    FPointNodeData & data(unsigned int node);
    void resetForwardInfo(const unsigned int node);
    void resetBackwardInfo(const unsigned int node);
    void resetForwardStall(const unsigned int node);
    void resetBackwardStall(const unsigned int node);
};


#endif //CONTRACTION_HIERARCHIES_FPOINTFLAGSGRAPH_H
