//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include "GraphBuilder/Loader.h"
#include "Dijkstra/BasicDijkstra.h"
#include "Benchmarking/DijkstraBenchmark.h"

int main() {
    Loader graphLoader = Loader("../input/USA-road-t.USA.gr");
    Graph * graph = graphLoader.loadGraph();
    Loader tripsLoader = Loader("../input/100trips1");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);
    DijkstraBenchmark::runAndMeasure(trips, *graph);

    delete graph;
    return 0;
}