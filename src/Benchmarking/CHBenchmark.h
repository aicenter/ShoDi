//
// Author: Xenty (Michal Cvach)
// Created on: 6.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHBENCHMARK_H
#define TRANSIT_NODE_ROUTING_CHBENCHMARK_H

#include <vector>
#include "../GraphBuilder/Graph.h"

using namespace std;

// A class that allows us to run Contraction Hierarchies over a vector of trips. The time of the queries is recorded
// and returned.
//______________________________________________________________________________________________________________________
class CHBenchmark {
public:
    static void runAndMeasure(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < unsigned int > & ranks);
    static void runAndMeasureWithOutput(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < unsigned int > & ranks, vector < long long unsigned int > & distances);
    static double runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < unsigned int > & ranks, vector < long long unsigned int > & distances);
private:
};


#endif //TRANSIT_NODE_ROUTING_CHBENCHMARK_H
