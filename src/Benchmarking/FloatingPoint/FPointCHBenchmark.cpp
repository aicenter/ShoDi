//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#include "FPointCHBenchmark.h"
#include "../../Timer/Timer.h"
#include "../../CH/FloatingPoint/FPointCHDistanceQueryManager.h"
#include "../../CH/FloatingPoint/FPointCHDistanceQueryManagerWithMapping.h"


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

//______________________________________________________________________________________________________________________
void FPointCHBenchmark::runMeasureAndOutputDistanceQueriesWithMapping(const vector < pair < long long unsigned int, long long unsigned int> > & trips, FPointFlagsGraph & graph, string mappingFilepath, vector < double > & distances) {
    FPointCHDistanceQueryManagerWithMapping queryManager(graph, mappingFilepath);

    Timer chTimer("Contraction hierarchies trips benchmark with mapping");
    chTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    chTimer.finish();
    chTimer.printMeasuredTime();
}