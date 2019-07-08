//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_BASICDIJKSTRA_H
#define TRANSIT_NODE_ROUTING_BASICDIJKSTRA_H


#include "Structures/IntegerStructures/IntegerGraph.h"

// A simple implementation of a basic Dijkstra's algorithm, which we compare the CH with.
//______________________________________________________________________________________________________________________
class BasicIntegerDijkstra {
public:
    static long long unsigned int run(const unsigned int source, const unsigned int target, const IntegerGraph & graph);
    static long long unsigned int runWithPathOutput(const unsigned int source, const unsigned int target, const IntegerGraph & graph);
private:
    static void outputPath(const unsigned int x, const long long unsigned int * dist, const vector < vector < unsigned int > > & prev);
};


#endif //TRANSIT_NODE_ROUTING_BASICDIJKSTRA_H
