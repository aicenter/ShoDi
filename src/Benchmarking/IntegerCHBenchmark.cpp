//
// Author: Xenty (Michal Cvach)
// Created on: 23.8.18
//

#include "IntegerCHBenchmark.h"
#include "../CH/IntegerCHDistanceQueryManager.h"
#include "../Timer/Timer.h"

//______________________________________________________________________________________________________________________
double IntegerCHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, IntegerFlagsGraph & graph, vector < long long unsigned int > & distances) {
    IntegerCHDistanceQueryManager queryManager(graph);

    Timer chTimer("Contraction hierarchies trips benchmark");
    chTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    chTimer.finish();
    chTimer.printMeasuredTime();
    return chTimer.getMeasuredTimeInSeconds();
}