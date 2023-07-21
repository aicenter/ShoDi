//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include "TNRBenchmark.h"
#include "../Timer/Timer.h"
#include "../TNR/TNRDistanceQueryManager.h"
#include "../TNR/TNRDistanceQueryManagerWithMapping.h"

//______________________________________________________________________________________________________________________
double TNRBenchmark::benchmark(const std::vector < std::pair< unsigned int, unsigned int> > & trips, TransitNodeRoutingGraph & graph, std::vector < unsigned int > & distances) {
    TNRDistanceQueryManager queryManager(graph);

    Timer tnrTimer("Transit Node Routing trips benchmark");
    tnrTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    tnrTimer.finish();
    return tnrTimer.getRealTimeSeconds();
}

//______________________________________________________________________________________________________________________
double TNRBenchmark::benchmarkWithMapping(const std::vector < std::pair< long long unsigned int, long long unsigned int> > & trips, TransitNodeRoutingGraph & graph, std::vector < unsigned int > & distances, std::string mappingFilePath) {
    TNRDistanceQueryManagerWithMapping queryManager(graph, mappingFilePath);

    Timer tnrTimer("Transit Node Routing trips benchmark with mapping");
    tnrTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    tnrTimer.finish();
    return tnrTimer.getRealTimeSeconds();
}
