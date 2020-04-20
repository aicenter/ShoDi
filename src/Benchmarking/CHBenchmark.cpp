//
// Author: Xenty (Michal Cvach)
// Created on: 23.8.18
//

#include "CHBenchmark.h"
#include "../CH/CHDistanceQueryManager.h"
#include "../Timer/Timer.h"
#include "../CH/CHDistanceQueryManagerWithMapping.h"

//______________________________________________________________________________________________________________________
double CHBenchmark::benchmark(const vector < pair < unsigned int, unsigned int> > & trips, FlagsGraph & graph, vector < unsigned int > & distances) {
    CHDistanceQueryManager queryManager(graph);

    Timer chTimer("Contraction hierarchies trips benchmark");
    chTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    chTimer.finish();
    return chTimer.getMeasuredTimeInSeconds();
}

//______________________________________________________________________________________________________________________
double CHBenchmark::benchmarkUsingMapping(const vector < pair < long long unsigned int, long long unsigned int> > & trips, FlagsGraph & graph, vector < unsigned int > & distances, string mappingFilePath) {
    CHDistanceQueryManagerWithMapping queryManager(graph, mappingFilePath);

    Timer chTimer("Contraction hierarchies trips benchmark with mapping");
    chTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    chTimer.finish();
    return chTimer.getMeasuredTimeInSeconds();
}