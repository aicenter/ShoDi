//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include "TNRBenchmark.h"
#include "../Timer/Timer.h"
#include "../TNR/TNRDistanceQueryManager.h"

// Runs the given set of queries, records the time required for those queries, and puts the results inside the provided
// vector. Returns the time it took to answer all queries in seconds. The distances are not validated inside this
// function, user is advised to validate that the results are correct somewhere outside this function.
//______________________________________________________________________________________________________________________
double TNRBenchmark::runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, TransitNodeRoutingGraph & graph, vector < unsigned int > & distances) {
    TNRDistanceQueryManager queryManager(graph);

    Timer tnrTimer("Transit Node Routing trips benchmark");
    tnrTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        // printf("TNR  -  doing trip: %u\n", i);
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    queryManager.printQueriesAnalysis();

    tnrTimer.finish();
    tnrTimer.printMeasuredTime();
    return tnrTimer.getMeasuredTimeInSeconds();
}