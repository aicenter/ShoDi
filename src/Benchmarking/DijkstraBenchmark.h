//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H
#define TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H

#include <vector>

using namespace std;

class DijkstraBenchmark {
public:
    static void runAndMeasure(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph);
};


#endif //TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H
