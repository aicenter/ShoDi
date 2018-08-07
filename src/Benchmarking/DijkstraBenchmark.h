//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H
#define TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H

#include <vector>
#include "../GraphBuilder/Graph.h"

using namespace std;

class DijkstraBenchmark {
public:
    static void runAndMeasure(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph);
    static void runAndMeasureWithOutput(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < long long unsigned int > & distances);
    static double runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < long long unsigned int > & distances);
};


#endif //TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H
