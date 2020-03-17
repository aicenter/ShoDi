//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#ifndef CONTRACTION_HIERARCHIES_DISTANCEMATRIXBENCHMARK_H
#define CONTRACTION_HIERARCHIES_DISTANCEMATRIXBENCHMARK_H

#include <utility>
#include <vector>
#include "../DistanceMatrix/DistanceMatrix.h"

using namespace std;

// Allows the user to benchmark the Distance Matrix approach with a given set of queries (trips).
//______________________________________________________________________________________________________________________
class DistanceMatrixBenchmark {
public:
    static double runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, DistanceMatrix & matrix, vector < long long unsigned int > & distances);
};


#endif //CONTRACTION_HIERARCHIES_DISTANCEMATRIXBENCHMARK_H
