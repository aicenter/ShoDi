//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include "TNRBenchmark.h"
#include "../Timer/Timer.h"
#include "../TNR/TNRDistanceQueryManager.h"
#include "../TNR/TNRDistanceQueryManagerWithMapping.h"

//______________________________________________________________________________________________________________________
double TNRBenchmark::benchmark(const vector < pair < unsigned int, unsigned int> > & trips, TransitNodeRoutingGraph & graph, vector < unsigned int > & distances) {
    TNRDistanceQueryManager queryManager(graph);

    Timer tnrTimer("Transit Node Routing trips benchmark");
    tnrTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    tnrTimer.finish();
    return tnrTimer.getMeasuredTimeInSeconds();
}

//______________________________________________________________________________________________________________________
double TNRBenchmark::benchmarkWithMapping(const vector < pair < long long unsigned int, long long unsigned int> > & trips, TransitNodeRoutingGraph & graph, vector < unsigned int > & distances, string mappingFilePath) {
    TNRDistanceQueryManagerWithMapping queryManager(graph, mappingFilePath);

    Timer tnrTimer("Transit Node Routing trips benchmark with mapping");
    tnrTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    tnrTimer.finish();
    return tnrTimer.getMeasuredTimeInSeconds();
}