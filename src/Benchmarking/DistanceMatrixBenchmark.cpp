//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include "DistanceMatrixBenchmark.h"
#include "../Timer/Timer.h"

//______________________________________________________________________________________________________________________
double DistanceMatrixBenchmark::benchmark(const vector < pair < unsigned int, unsigned int> > & trips, DistanceMatrix & matrix, vector < int > & distances) {
    Timer dijkstraTimer("Distance Matrix trips benchmark");
    dijkstraTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = matrix.findDistance(trips.at(i).first, trips.at(i).second);
    }

    dijkstraTimer.finish();
    return dijkstraTimer.getMeasuredTimeInSeconds();
}