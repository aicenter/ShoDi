//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#include "FPointCHBenchmark.h"
#include "../../Timer/Timer.h"
#include "../../CH/FloatingPoint/FPointCHDistanceQueryManager.h"

//______________________________________________________________________________________________________________________
double FPointCHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, FPointFlagsGraph & graph, vector < double > & distances) {
    FPointCHDistanceQueryManager queryManager(graph);

    Timer chTimer("Contraction hierarchies trips benchmark");
    chTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    chTimer.finish();
    chTimer.printMeasuredTime();
    return chTimer.getMeasuredTimeInSeconds();
}