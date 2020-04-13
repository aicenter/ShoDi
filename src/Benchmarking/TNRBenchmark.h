//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRBENCHMARK_H
#define CONTRACTION_HIERARCHIES_TNRBENCHMARK_H

#include <vector>
#include "../GraphBuilding/Structures/FlagsGraph.h"
#include "../GraphBuilding/Structures/TransitNodeRoutingGraph.h"

using namespace std;

// Allows the user to benchmark the Transit Node Routing with a given set of queries (trips).
//______________________________________________________________________________________________________________________
class TNRBenchmark {
public:
    static double benchmark(const vector < pair < unsigned int, unsigned int> > & trips, TransitNodeRoutingGraph & graph, vector < unsigned int > & distances);
    static double benchmarkWithMapping(const vector < pair < long long unsigned int, long long unsigned int> > & trips, TransitNodeRoutingGraph & graph, vector < unsigned int > & distances, string mappingFilePath);
};



#endif //CONTRACTION_HIERARCHIES_TNRBENCHMARK_H
