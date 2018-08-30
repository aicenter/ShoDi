//
// Author: Xenty (Michal Cvach)
// Created on: 23.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H
#define TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H

#include "../GraphBuilder/FlagsGraph.h"
#include "../GraphBuilder/Graph.h"

class CHBenchmark {
public:
    static double runAndMeasureFlagsGraphOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, FlagsGraph & graph, vector < long long unsigned int > & distances);
};


#endif //TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H
