//
// Author: Xenty (Michal Cvach)
// Created on: 23.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H
#define TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H

#include "CHBenchmark.h"
#include "../GraphBuilder/FlagsGraph.h"

class CHBenchmarkWithRanks : public CHBenchmark {
public:
    static double runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < unsigned int > & ranks, vector < long long unsigned int > & distances);
    static double runAndMeasureFlagsGraphOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, FlagsGraph & graph, vector < unsigned int > & ranks, vector < long long unsigned int > & distances);
};


#endif //TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H
