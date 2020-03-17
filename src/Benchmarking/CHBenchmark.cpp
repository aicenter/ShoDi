//
// Author: Xenty (Michal Cvach)
// Created on: 23.8.18
//

#include "CHBenchmark.h"
#include "../CH/CHDistanceQueryManager.h"
#include "../Timer/Timer.h"

//______________________________________________________________________________________________________________________
double CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, FlagsGraph & graph, vector < unsigned int > & distances) {
    CHDistanceQueryManager queryManager(graph);

    Timer chTimer("Contraction hierarchies trips benchmark");
    chTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    chTimer.finish();
    chTimer.printMeasuredTime();
    return chTimer.getMeasuredTimeInSeconds();
}