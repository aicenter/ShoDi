//
// Author: horakj31 (Jan Horak)
// Created on: 25.07.24
//

#include <string>
#include <iostream>
#include "../Astar/Astar.h"
#include "../Timer/Timer.h"
#include "AstarBenchmark.h"
#include "../GraphBuilding/Loaders/CsvGraphLoader.h"
#include "../GraphBuilding/Loaders/XenGraphLoader.h"

//______________________________________________________________________________________________________________________
double AstarBenchmark::benchmark(const std::vector < std::pair< unsigned int, unsigned int> > & trips, const Graph & graph, const std::vector<std::pair<double, double>>& locations, std::vector < unsigned int > & distances) {
    Timer aStarTimer("Astar trips benchmark");
    aStarTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = Astar::run(trips.at(i).first, trips.at(i).second, graph, locations);
    }

    aStarTimer.finish();
    return aStarTimer.getRealTimeSeconds();
}

//______________________________________________________________________________________________________________________
double AstarBenchmark::benchmarkUsingMapping(const std::vector < std::pair< long long unsigned int, long long unsigned int> > & trips, const Graph & graph, const std::vector<std::pair<double, double>>& locations, std::vector < unsigned int > & distances, std::string mappingFilePath) {
    XenGraphLoader mappingLoader(mappingFilePath);
    std::unordered_map<long long unsigned int, unsigned int> mapping;
    mappingLoader.loadNodesMapping(mapping);

    Timer aStarTimer("Astar trips benchmark");
    aStarTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = Astar::run(mapping.at(trips.at(i).first), mapping.at(trips.at(i).second), graph, locations);
    }

    aStarTimer.finish();
    return aStarTimer.getRealTimeSeconds();
}
