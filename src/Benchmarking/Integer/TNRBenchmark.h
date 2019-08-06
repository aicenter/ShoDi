//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRBENCHMARK_H
#define CONTRACTION_HIERARCHIES_TNRBENCHMARK_H

#include <vector>
#include "../../GraphBuilding/Structures/IntegerStructures/IntegerFlagsGraph.h"
#include "../../GraphBuilding/Structures/IntegerStructures/TransitNodeRoutingGraph.h"

using namespace std;

// Allows the user to benchmark the Transit Node Routing with a given set of queries (trips).
//______________________________________________________________________________________________________________________
class TNRBenchmark {
public:
    static double runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, TransitNodeRoutingGraph & graph, vector < long long unsigned int > & distances);
};



#endif //CONTRACTION_HIERARCHIES_TNRBENCHMARK_H
