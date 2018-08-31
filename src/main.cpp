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
#include "CH/CHQueryManager.h"
#include "CH/CHPathQueryManager.h"


// This function will construct Contraction Hierarchies from a given graph. This means preprocessing the graph and
// and saving the structures into a file.
//______________________________________________________________________________________________________________________
void constructCH() {
    Loader graphLoader = Loader("../input/USA-road-t.COL.gr");
    //Loader graphLoader = Loader("../input/Rome-road.gr");
    //Loader graphLoader = Loader("input/Rome-road.gr");
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    CHPreprocessor::preprocessAndSaveWithUnpackingData("../input/USA.COL.CH_measure2", *graph);
    //CHPreprocessor::preprocessAndSaveWithUnpackingData("../input/Rome_debug3", *graph);
    //CHPreprocessor::preprocessAndSaveWithUnpackingData("input/Rome_test", *graph);
    delete graph;
}

// Runs Dijkstra and Contraction Hierarchies over some set of trips and compares the run times. Dijkstra and CH are run
// one after each other.
//______________________________________________________________________________________________________________________
void compareDijkstraWithCHMemoryEconomical() {
    Loader dijkstraGraphLoader = Loader("../input/USA-road-t.COL.gr");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();
    Loader tripsLoader = Loader("../input/COL100randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    vector<long long unsigned int> dijkstraDistanes(trips.size());
    double dijkstraTime = DijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistanes);
    delete dijkstraGraph;

    Loader chGraphLoader = Loader("../input/USA-road-t.COL.gr");
    Graph * chGraph = chGraphLoader.loadCHGraphWithShortcuts("../input/USA.COL.CH_measure2_shortcuts");
    Loader ranksLoader = Loader("../input/USA.COL.CH_measure2_ranks");
    vector<unsigned int> ranks;
    ranksLoader.loadRanks(ranks);

    vector<long long unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::runAndMeasureOutputAndRetval(trips, *chGraph, ranks, chDistances);
    //for (unsigned int i = 0; i < 20; i++) {
    //    printf("Dijkstra / ch for trip %i: %llu / %llu (trips index from 0)\n", i, dijkstraDistanes[i], chDistances[i]);
    //}
    CorectnessValidator::validateVerbose(dijkstraDistanes, chDistances);
    printf("CH were %lf times faster than Dijkstra!\n", dijkstraTime/chTime);

    delete chGraph;

}

// Runs Dijkstra and Contraction Hierarchies over some set of trips and compares the run times. Dijkstra and CH are run
// basically together, which increases memory requirements by a fair amount, because there will be two copies of the
// graph at the same time in the memory - one for the Dijkstra and second with shortcuts and additional information
// (such as node ranks) for CH. For this reason the compareDijkstraWithCHMemoryEconomical() should be preferred.
//______________________________________________________________________________________________________________________
void compareDijkstraWithCH() {
    Loader dijkstraGraphLoader = Loader("../input/Rome-road.gr");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();
    Loader chGraphLoader = Loader("../input/Rome-road.gr");
    Graph * chGraph = chGraphLoader.loadCHGraphWithShortcuts("../input/Rome_debug3_shortcuts");
    Loader ranksLoader = Loader("../input/Rome_debug3_ranks");
    vector<unsigned int> ranks;
    ranksLoader.loadRanks(ranks);

    Loader tripsLoader = Loader("../input/rome_1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    vector<long long unsigned int> dijkstraDistanes(trips.size());
    vector<long long unsigned int> chDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistanes);
    double chTime = CHBenchmark::runAndMeasureOutputAndRetval(trips, *chGraph, ranks, chDistances);
    //for (unsigned int i = 0; i < 20; i++) {
    //    printf("Dijkstra / ch for trip %i: %llu / %llu (trips index from 0)\n", i, dijkstraDistanes[i], chDistances[i]);
    //}
    CorectnessValidator::validateVerbose(dijkstraDistanes, chDistances);
    printf("CH were %lf times faster than Dijkstra!\n", dijkstraTime/chTime);

    delete dijkstraGraph;
    delete chGraph;

}

// Will print the node sequence with edge lengths on a certain path computed by Dijkstra - can be used for debug.
//______________________________________________________________________________________________________________________
void getDijkstraPathForTrip() {
    Loader dijkstraGraphLoader = Loader("../input/Rome-road.gr");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    Loader tripsLoader = Loader("../input/rome_1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    unsigned int chosenTrip = 0;
    unsigned long long int distance = BasicDijkstra::runWithPathOutput(trips[chosenTrip].first, trips[chosenTrip].second, *dijkstraGraph);

    delete dijkstraGraph;

}

// Will print the node sequence on a certain path computed by Contraction Hierarchies - this means that the paths will
// be unpacked from the shortcuts. Can be used for debug, especially if CH returns different paths than Dijkstra.
//______________________________________________________________________________________________________________________
void getCHPathForTrip() {
    Loader chGraphLoader = Loader("../input/Rome-road.gr");
    Graph * chGraph = chGraphLoader.loadCHGraphWithShortcuts("../input/Rome_debug3_shortcuts");
    Loader ranksLoader = Loader("../input/Rome_debug3_ranks");
    vector<unsigned int> ranks;
    ranksLoader.loadRanks(ranks);
    Loader unpackingDataLoader = Loader("../input/Rome_debug3_unpacking");
    map < pair < unsigned int, unsigned int >, unsigned int > unpackingData;
    unpackingDataLoader.loadUnpackingData(unpackingData);

    Loader tripsLoader = Loader("../input/rome_1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    unsigned int chosenTrip = 0;

    CHPathQueryManager qm(ranks, unpackingData);
    long long unsigned int distance = qm.findPath(trips.at(chosenTrip).first, trips.at(chosenTrip).second, *chGraph);
    printf("Returned distance: %llu\n", distance);

    delete chGraph;
}

// Runs one CH query, but doesn't print the path, only the distance.
//______________________________________________________________________________________________________________________
void runOneCHQuery() {
    Loader chGraphLoader = Loader("../input/artifGraph1.CH_graph");
    Graph * chGraph = chGraphLoader.loadGraph();
    Loader ranksLoader = Loader("../input/artifGraph1.CH_ranks");
    vector<unsigned int> ranks;
    ranksLoader.loadRanks(ranks);

    Loader tripsLoader = Loader("../input/artif1_1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    unsigned int chosenTrip = 0;

    CHQueryManager qm(ranks);
    long long unsigned int distance = qm.findDistance(trips.at(chosenTrip).first, trips.at(chosenTrip).second, *chGraph);
    printf("Returned distance: %llu\n", distance);

    delete chGraph;
}

// Simple mainfunction, uncomment the function you want to use.
//______________________________________________________________________________________________________________________
int main() {
    //constructCH();
    //compareDijkstraWithCH();
    compareDijkstraWithCHMemoryEconomical();
    //runOneCHQuery();
    //getDijkstraPathForTrip();
    //getCHPathForTrip();

    return 0;
}