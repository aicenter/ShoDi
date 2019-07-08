//
// Created by xenty on 8.7.19.
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTSIMPLEGRAPH_H
#define CONTRACTION_HIERARCHIES_FPOINTSIMPLEGRAPH_H

#include <vector>
#include <map>

using namespace std;

// Class representing a simple graph - Simple graph is a graph where there only exists at most one edge between each
// pair of nodes. Simple graph doesn't contain multiple edges (parallel edges). In our case, if there are more edges
// than one between two nodes in the original graph, we only preserve the one with the lowest weight.
//______________________________________________________________________________________________________________________
class FPointSimpleGraph {
private:
    vector< map < unsigned int, double > > followingNodes;
public:
    FPointSimpleGraph(unsigned int n);
    void addEdge(unsigned int from, unsigned int to, double weight);
    const unsigned int nodes() const;
    const map<unsigned int, double> & edges(const unsigned int x)const;
};


#endif //CONTRACTION_HIERARCHIES_FPOINTSIMPLEGRAPH_H
