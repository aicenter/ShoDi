//
// Author: Xenty (Michal Cvach)
// Created on: 23.8.18
//

#include "CHBenchmarkWithRanks.h"
#include "../CH/CHAlternativeQueryManger.h"
#include "../CH/CHFlagsGraphQueryManager.h"
#include "../Timer/Timer.h"

//______________________________________________________________________________________________________________________
double CHBenchmarkWithRanks::runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < unsigned int > & ranks, vector < long long unsigned int > & distances) {
    CHAlternativeQueryManager queryManager(ranks, graph);

    Timer chTimer("Contraction hierarchies trips benchmark");
    chTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
        //printf("Found distance for trip %u, distance: %llu", i, distances[i]);
    }

    chTimer.finish();
    chTimer.printMeasuredTime();
    return chTimer.getMeasuredTimeInSeconds();
}

//______________________________________________________________________________________________________________________
double CHBenchmarkWithRanks::runAndMeasureFlagsGraphOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, FlagsGraph & graph, vector < unsigned int > & ranks, vector < long long unsigned int > & distances) {
    CHFlagsGraphQueryManager queryManager(ranks, graph);

    Timer chTimer("Contraction hierarchies trips benchmark");
    chTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    chTimer.finish();
    chTimer.printMeasuredTime();
    return chTimer.getMeasuredTimeInSeconds();
}