//
// Author: Xenty (Michal Cvach)
// Created on: 06.12.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRAFBENCHMARK_H
#define CONTRACTION_HIERARCHIES_TNRAFBENCHMARK_H

#include "../GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.h"

using namespace std;

// Allows the user to benchmark the Transit Node Routing with Arc Flags using a given set of queries (trips).
//______________________________________________________________________________________________________________________
class TNRAFBenchmark {
public:
    static double runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, TransitNodeRoutingArcFlagsGraph & graph, vector < long long unsigned int > & distances);
};


#endif //CONTRACTION_HIERARCHIES_TNRAFBENCHMARK_H
