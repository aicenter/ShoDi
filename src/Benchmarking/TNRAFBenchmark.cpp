//
// Author: Xenty (Michal Cvach)
// Created on: 06.12.19
//

#include "TNRAFBenchmark.h"
#include "../TNRAF/TNRAFDistanceQueryManager.h"
#include "../Timer/Timer.h"

// Runs the Transit Node Routing with Arc Flags query algorithm on a set set of queries (trips). Returns the cumulative
// time the queries took in seconds. Additionally stores the returned distances into a vector so that their correctness
// can be later verified.
//______________________________________________________________________________________________________________________
double TNRAFBenchmark::runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, TransitNodeRoutingArcFlagsGraph & graph, vector < unsigned int > & distances) {
    TNRAFDistanceQueryManager queryManager(graph);

    Timer tnrTimer("Transit Node Routing trips benchmark");
    tnrTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    queryManager.printQueriesAnalysis();

    tnrTimer.finish();
    tnrTimer.printMeasuredTime();
    return tnrTimer.getMeasuredTimeInSeconds();
}