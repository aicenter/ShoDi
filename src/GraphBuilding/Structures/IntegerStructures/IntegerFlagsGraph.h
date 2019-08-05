//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#ifndef TRANSIT_NODE_ROUTING_FLAGSGRAPH_H
#define TRANSIT_NODE_ROUTING_FLAGSGRAPH_H

#include <utility>
#include <vector>
#include "IntegerQueryEdge.h"
#include "../../../CH/Integer/Structures/IntegerNodeData.h"
#include "IntegerUpdateableGraph.h"

using namespace std;

// Class representing a graph for specifically for the CH query algorithm.
// We use the representation suggested in the "Contraction Hierarchies: Faster and Simpler Hierarchical
// Routing in Road Networks" article by Robert Geisberger, Peter Sanders, Dominik Schultes, and Daniel Delling.
// Basically, for example Dijkstra requires undirected edges to be saved at both nodes. This representation
// only saves every edge at the incident node with the lower rank. This makes sense, because we only expand edges
// going from lower ranked nodes to higher ranked nodes in the query algorithm anyway. We store additional flags
// "forward" and "backward" with each edge to determine in which direction the edge should be expanded.
//______________________________________________________________________________________________________________________
class IntegerFlagsGraph{
protected:
    vector< vector < IntegerQueryEdge > > neighbours;
    vector< IntegerNodeData > nodesData;
    void processOriginalEdges(vector < IntegerOutputEdge > & edges);
    void processShortcuts( vector < IntegerOutputShortcutEdge > & shortcuts);
public:
    IntegerFlagsGraph(unsigned int n);
    IntegerFlagsGraph(IntegerUpdateableGraph & g);
    void addEdge(unsigned int from, unsigned int to, long long unsigned int weight, bool fw, bool bw);
    const unsigned int nodes() const;
    const vector< IntegerQueryEdge > & nextNodes(const unsigned int x)const;
    IntegerNodeData & data(unsigned int node);
    void resetForwardInfo(const unsigned int node);
    void resetBackwardInfo(const unsigned int node);
    void resetForwardStall(const unsigned int node);
    void resetBackwardStall(const unsigned int node);
};


#endif //TRANSIT_NODE_ROUTING_FLAGSGRAPH_H
