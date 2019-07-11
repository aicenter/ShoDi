//
// Author: Xenty (Michal Cvach)
// Created on: 23.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H
#define TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H

#include "../../GraphBuilding/Structures/IntegerStructures/IntegerFlagsGraph.h"
#include "../../GraphBuilding/Structures/IntegerStructures/IntegerGraph.h"

// This class is responsible for running a CH Benchmark and outputing the time. The only method
// 'runAndMeasureFlagsGraphOutputAndRetval()' just takes a vector of trips as an argument and computes shortest
// distance for all of them, while also computing the time it spent doing it.
//______________________________________________________________________________________________________________________
class IntegerCHBenchmark {
public:
    static double runAndMeasureFlagsGraphOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, IntegerFlagsGraph & graph, vector < long long unsigned int > & distances);
};


#endif //TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H
