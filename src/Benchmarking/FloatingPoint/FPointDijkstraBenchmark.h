//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTDIJKSTRABENCHMARK_H
#define CONTRACTION_HIERARCHIES_FPOINTDIJKSTRABENCHMARK_H

#include <vector>
#include "../../GraphBuilding/Structures/FloatingPointStructures/FPointGraph.h"

using namespace std;

// A class that allows us to run Dijkstra's Algorithm over a vector of trips. The time of the queries is recorded
// and returned.
//______________________________________________________________________________________________________________________
class FPointDijkstraBenchmark {
public:
    static void runAndMeasure(const vector < pair < unsigned int, unsigned int> > & trips, const FPointGraph & graph);
    static void runAndMeasureWithOutput(const vector < pair < unsigned int, unsigned int> > & trips, const FPointGraph & graph, vector < double > & distances);
    static double runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, const FPointGraph & graph, vector < double > & distances);
};

#endif //CONTRACTION_HIERARCHIES_FPOINTDIJKSTRABENCHMARK_H
