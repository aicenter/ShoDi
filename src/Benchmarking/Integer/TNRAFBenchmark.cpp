//
// Author: Xenty (Michal Cvach)
// Created on: 06.12.19
//

#include "TNRAFBenchmark.h"
#include "../../TNRAF/TNRAFDistanceQueryManager.h"
#include "../../Timer/Timer.h"

//______________________________________________________________________________________________________________________
double TNRAFBenchmark::runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, TransitNodeRoutingArcFlagsGraph & graph, vector < long long unsigned int > & distances) {
    TNRAFDistanceQueryManager queryManager(graph);

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