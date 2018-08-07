//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include "GraphBuilder/Loader.h"
#include "Dijkstra/BasicDijkstra.h"
#include "Benchmarking/CHBenchmark.h"
#include "Benchmarking/CorectnessValidator.h"
#include "Benchmarking/DijkstraBenchmark.h"
#include "Timer/Timer.h"
#include "TNR/Grid.h"
#include "Utils/MinMaxCoordsFinder.h"
#include "CH/CHPreprocessor.h"

void constructCH() {
    Loader graphLoader = Loader("../input/artifGraph1.gr");
    Graph * graph = graphLoader.loadGraph();
    CHPreprocessor::preprocessAndSave("../input/artifGraph1.CH", *graph);
    delete graph;
}

void compareDijkstraWithCH() {
    Loader dijkstraGraphLoader = Loader("../input/artifGraph1.gr");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();
    Loader chGraphLoader = Loader("../input/artifGraph1.CH_graph");
    Graph * chGraph = chGraphLoader.loadGraph();
    Loader ranksLoader = Loader("../input/artifGraph1.CH_ranks");
    vector<unsigned int> ranks;
    ranksLoader.loadRanks(ranks);

    Loader tripsLoader = Loader("../input/artif1_1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    vector<long long unsigned int> dijkstraDistanes(trips.size());
    vector<long long unsigned int> chDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistanes);
    double chTime = CHBenchmark::runAndMeasureOutputAndRetval(trips, *chGraph, ranks, chDistances);
    for (unsigned int i = 0; i < 10; i++) {
        printf("Dijkstra / ch for trip %i: %llu / %llu (trips index from 0)\n", i, dijkstraDistanes[i], chDistances[i]);
    }
    //CorectnessValidator::validateVerbose(dijkstraDistanes, chDistances);
    printf("CH were %lf times faster than Dijkstra!\n", dijkstraTime/chTime);

    delete dijkstraGraph;
    delete chGraph;

}

int main() {
    //constructCH();
    compareDijkstraWithCH();

    return 0;
}