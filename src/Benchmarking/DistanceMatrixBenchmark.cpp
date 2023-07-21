//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include "DistanceMatrixBenchmark.h"
#include "../Timer/Timer.h"

//______________________________________________________________________________________________________________________
double DistanceMatrixBenchmark::benchmark(const std::vector < std::pair< unsigned int, unsigned int> > & trips, Distance_matrix_travel_time_provider & matrix, std::vector <dist_t> & distances) {
    Timer dijkstraTimer("Distance Matrix trips benchmark");
    dijkstraTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = matrix.findDistance(trips.at(i).first, trips.at(i).second);
    }

    dijkstraTimer.finish();
    return dijkstraTimer.getRealTimeSeconds();
}
