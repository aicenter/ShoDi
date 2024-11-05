//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include "DistanceMatrixBenchmark.h"
#include "../Timer/Timer.h"
#include "../GraphBuilding/Loaders/XenGraphLoader.h"

//______________________________________________________________________________________________________________________
double DistanceMatrixBenchmark::benchmark(const std::vector < std::pair< unsigned int, unsigned int> > & trips, DistanceMatrixInterface& matrix, std::vector <dist_t> & distances) {
    Timer dijkstraTimer("Distance Matrix trips benchmark");
    dijkstraTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = matrix.findDistance(trips.at(i).first, trips.at(i).second);
    }

    dijkstraTimer.finish();
    return dijkstraTimer.getRealTimeSeconds();
}

//______________________________________________________________________________________________________________________
double DistanceMatrixBenchmark::benchmarkUsingMapping(const std::vector < std::pair< long long unsigned int, long long unsigned int> > & trips, DistanceMatrixInterface& matrix, std::vector <dist_t> & distances, std::string mappingFilePath) {
    XenGraphLoader mappingLoader(mappingFilePath);
    std::unordered_map<long long unsigned int, unsigned int> mapping;
    mappingLoader.loadNodesMapping(mapping);
    
    Timer dijkstraTimer("Distance Matrix trips benchmark");
    dijkstraTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = matrix.findDistance(mapping.at(trips.at(i).first), mapping.at(trips.at(i).second));
    }

    dijkstraTimer.finish();
    return dijkstraTimer.getRealTimeSeconds();
}
