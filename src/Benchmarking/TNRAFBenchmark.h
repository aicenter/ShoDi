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
    static double benchmark(const vector < pair < unsigned int, unsigned int> > & trips, TransitNodeRoutingArcFlagsGraph & graph, vector < unsigned int > & distances);
    static double benchmarkWithMapping(const vector < pair < long long unsigned int, long long unsigned int> > & trips, TransitNodeRoutingArcFlagsGraph & graph, vector < unsigned int > & distances, string mappingFilePath);
};


#endif //CONTRACTION_HIERARCHIES_TNRAFBENCHMARK_H
