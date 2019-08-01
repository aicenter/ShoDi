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

//______________________________________________________________________________________________________________________
class FPointCHBenchmark {
public:
    static double runAndMeasureFlagsGraphOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, FPointFlagsGraph & graph, vector < double > & distances);
    static void runMeasureAndOutputDistanceQueriesWithMapping(const vector < pair < long long unsigned int, long long unsigned int> > & trips, FPointFlagsGraph & graph, string mappingFilepath, vector < double > & distances);
};


#endif //CONTRACTION_HIERARCHIES_FPOINTCHBENCHMARK_H
