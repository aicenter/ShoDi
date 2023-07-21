//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#include <string>
#include "../Dijkstra/BasicDijkstra.h"
#include "../Timer/Timer.h"
#include "DijkstraBenchmark.h"
#include "../GraphBuilding/Loaders/XenGraphLoader.h"

//______________________________________________________________________________________________________________________
double DijkstraBenchmark::benchmark(const std::vector < std::pair< unsigned int, unsigned int> > & trips, const Graph & graph, std::vector < unsigned int > & distances) {
    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = BasicDijkstra::run(trips.at(i).first, trips.at(i).second, graph);
    }

    dijkstraTimer.finish();
    return dijkstraTimer.getRealTimeSeconds();
}

//______________________________________________________________________________________________________________________
double DijkstraBenchmark::benchmarkUsingMapping(const std::vector < std::pair< long long unsigned int, long long unsigned int> > & trips, const Graph & graph, std::vector < unsigned int > & distances, std::string mappingFilePath) {
    XenGraphLoader mappingLoader(mappingFilePath);
    std::unordered_map<long long unsigned int, unsigned int> mapping;
    mappingLoader.loadNodesMapping(mapping);

    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = BasicDijkstra::run(mapping.at(trips.at(i).first), mapping.at(trips.at(i).second), graph);
    }

    dijkstraTimer.finish();
    return dijkstraTimer.getRealTimeSeconds();
}
