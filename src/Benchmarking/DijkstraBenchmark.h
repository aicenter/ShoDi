//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H
#define TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H

#include <vector>
#include "../GraphBuilding/Structures/Graph.h"

using namespace std;

// A class that allows us to run Dijkstra's Algorithm over a vector of trips. The time of the queries is recorded
// and returned.
//______________________________________________________________________________________________________________________
class DijkstraBenchmark {
public:
    static void runAndMeasure(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph);
    static void runAndMeasureWithOutput(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < unsigned int > & distances);
    static double runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < unsigned int > & distances);
};


#endif //TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H
