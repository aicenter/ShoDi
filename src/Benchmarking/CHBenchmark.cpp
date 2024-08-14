//
// Author: Xenty (Michal Cvach)
// Created on: 23.8.18
//

#include "CHBenchmark.h"
#include "../CH/CHDistanceQueryManager.h"
#include "../Timer/Timer.h"
#include "../CH/CHDistanceQueryManagerWithMapping.h"

//______________________________________________________________________________________________________________________
double CHBenchmark::benchmark(const std::vector < std::pair< unsigned int, unsigned int> > & trips, FlagsGraph<NodeData>& graph, std::vector < unsigned int > & distances) {
    CHDistanceQueryManager queryManager(graph);

    Timer chTimer("Contraction hierarchies trips benchmark");
    chTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    chTimer.finish();
    return chTimer.getRealTimeSeconds();
}

//______________________________________________________________________________________________________________________
double CHBenchmark::benchmarkUsingMapping(const std::vector < std::pair< long long unsigned int, long long unsigned int> > & trips, FlagsGraph<NodeData>& graph, std::vector < unsigned int > & distances, std::string mappingFilePath) {
    CHDistanceQueryManagerWithMapping queryManager(graph, mappingFilePath);

    Timer chTimer("Contraction hierarchies trips benchmark with mapping");
    chTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    chTimer.finish();
    return chTimer.getRealTimeSeconds();
}
