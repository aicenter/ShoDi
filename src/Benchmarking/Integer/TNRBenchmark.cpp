//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include "TNRBenchmark.h"
#include "../../Timer/Timer.h"
#include "../../TNR/TNRDistanceQueryManager.h"

double TNRBenchmark::runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, TransitNodeRoutingGraph & graph, vector < long long unsigned int > & distances) {
    TNRDistanceQueryManager queryManager(graph);

    Timer tnrTimer("Transit Node Routing trips benchmark");
    tnrTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    tnrTimer.finish();
    tnrTimer.printMeasuredTime();
    return tnrTimer.getMeasuredTimeInSeconds();
}