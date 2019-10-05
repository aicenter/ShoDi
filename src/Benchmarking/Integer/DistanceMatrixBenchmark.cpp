//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include "DistanceMatrixBenchmark.h"
#include "../../Timer/Timer.h"

// Runs the given set of queries, records the time required for those queries, and puts the results inside the provided
// vector. Returns the time it took to answer all queries in seconds. The distances are not validated inside this
// function, user is advised to validate that the results are correct somewhere outside this function.
//______________________________________________________________________________________________________________________
double DistanceMatrixBenchmark::runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, IntegerDistanceMatrix & matrix, vector < long long unsigned int > & distances) {
    Timer dijkstraTimer("Distance Matrix trips benchmark");
    dijkstraTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = matrix.findDistance(trips.at(i).first, trips.at(i).second);
    }

    dijkstraTimer.finish();
    dijkstraTimer.printMeasuredTime();
    return dijkstraTimer.getMeasuredTimeInSeconds();
}