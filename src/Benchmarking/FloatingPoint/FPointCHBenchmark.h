//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTCHBENCHMARK_H
#define CONTRACTION_HIERARCHIES_FPOINTCHBENCHMARK_H

#include "../../GraphBuilding/Structures/FloatingPointStructures/FPointFlagsGraph.h"
#include <vector>
#include <string>

using namespace std;

// This class is responsible for running a CH Benchmark and outputing the time. The only method
// 'runAndMeasureFlagsGraphOutputAndRetval()' just takes a vector of trips as an argument and computes shortest
// distance for all of them, while also computing the time it spent doing it.
//______________________________________________________________________________________________________________________
class FPointCHBenchmark {
public:
    static double runAndMeasureFlagsGraphOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, FPointFlagsGraph & graph, vector < double > & distances);
    static void runMeasureAndOutputDistanceQueriesWithMapping(const vector < pair < long long unsigned int, long long unsigned int> > & trips, FPointFlagsGraph & graph, string mappingFilepath, vector < double > & distances);
};


#endif //CONTRACTION_HIERARCHIES_FPOINTCHBENCHMARK_H
