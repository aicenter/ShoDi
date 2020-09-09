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
double DijkstraBenchmark::benchmark(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < unsigned int > & distances) {
    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = BasicDijkstra::run(trips.at(i).first, trips.at(i).second, graph);
    }

    dijkstraTimer.finish();
    return dijkstraTimer.getMeasuredTimeInSeconds();
}

//______________________________________________________________________________________________________________________
double DijkstraBenchmark::benchmarkUsingMapping(const vector < pair < long long unsigned int, long long unsigned int> > & trips, const Graph & graph, vector < unsigned int > & distances, string mappingFilePath) {
    XenGraphLoader mappingLoader(mappingFilePath);
    unordered_map<long long unsigned int, unsigned int> mapping;
    mappingLoader.loadNodesMapping(mapping);

    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = BasicDijkstra::run(mapping.at(trips.at(i).first), mapping.at(trips.at(i).second), graph);
    }

    dijkstraTimer.finish();
    return dijkstraTimer.getMeasuredTimeInSeconds();
}