//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_BASICDIJKSTRA_H
#define TRANSIT_NODE_ROUTING_BASICDIJKSTRA_H


#include "../GraphBuilding/Structures/Graph.h"

// A simple implementation of a basic Dijkstra's algorithm, which we mainly use as a baseline to compare the other
// methods with, but it is also used during the preprocessing phases of some of the more complex methods.
//______________________________________________________________________________________________________________________
class BasicDijkstra {
public:
    static unsigned int run(const unsigned int source, const unsigned int target, const Graph & graph);
    static unsigned int runWithPathOutput(const unsigned int source, const unsigned int target, const Graph & graph);
    static void computeOneToAllDistances(const unsigned int source, const Graph & graph, vector<unsigned int> & distances);
    static void computeOneToAllDistancesInReversedGraph(const unsigned int source, const Graph & graph, vector<unsigned int> & distances);

private:
    static void outputPath(const unsigned int x, const unsigned int * dist, const vector < vector < unsigned int > > & prev);
};


#endif //TRANSIT_NODE_ROUTING_BASICDIJKSTRA_H
