//
// Author: Xenty (Michal Cvach)
// Created on: 06.12.19
//

#include "TNRAFBenchmark.h"
#include "../TNRAF/TNRAFDistanceQueryManager.h"
#include "../Timer/Timer.h"
#include "../TNRAF/TNRAFDistanceQueryManagerWithMapping.h"

//______________________________________________________________________________________________________________________
double TNRAFBenchmark::benchmark(const std::vector < std::pair< unsigned int, unsigned int> > & trips, TransitNodeRoutingArcFlagsGraph & graph, std::vector < unsigned int > & distances) {
    TNRAFDistanceQueryManager queryManager(graph);

    Timer tnrTimer("Transit Node Routing with Arc Flags trips benchmark");
    tnrTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    tnrTimer.finish();
    return tnrTimer.getRealTimeSeconds();
}

//______________________________________________________________________________________________________________________
double TNRAFBenchmark::benchmarkWithMapping(const std::vector < std::pair< long long unsigned int, long long unsigned int> > & trips, TransitNodeRoutingArcFlagsGraph & graph, std::vector < unsigned int > & distances, std::string mappingFilePath) {
    TNRAFDistanceQueryManagerWithMapping queryManager(graph, mappingFilePath);

    Timer tnrTimer("Transit Node Routing with Arc Flags trips benchmark with mapping");
    tnrTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    tnrTimer.finish();
    return tnrTimer.getRealTimeSeconds();
}
