//
// Created by xenty on 8.7.19.
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTGRAPH_H
#define CONTRACTION_HIERARCHIES_FPOINTGRAPH_H

#include "FPointSimpleGraph.h"
#include <vector>

using namespace std;

// Class representing a graph.
// This simple graph representation is used for the Dijkstra Algorithm. For the Contraction Hierarchies,
// other representation seem to work better.
//______________________________________________________________________________________________________________________
class FPointGraph{
private:
    vector< vector < pair< unsigned int, double > > > followingNodes;
    vector< vector < pair< unsigned int, double > > > previousNodes;
public:
    FPointGraph(unsigned int n);
    FPointGraph(FPointSimpleGraph & x);
    void addEdge(unsigned int from, unsigned int to, double weight);
    const unsigned int nodes() const;
    const vector<pair<unsigned int, double>> & incomingEdges(const unsigned int x)const;
    const vector<pair<unsigned int, double>> & outgoingEdges(const unsigned int x)const;
};


#endif //CONTRACTION_HIERARCHIES_FPOINTGRAPH_H
