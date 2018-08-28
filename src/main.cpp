//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include "GraphBuilder/DDSGLoader.h"
#include "GraphBuilder/Loader.h"
#include "Dijkstra/BasicDijkstra.h"
#include "Benchmarking/CHBenchmark.h"
#include "Benchmarking/CHBenchmarkWithRanks.h"
#include "Benchmarking/CorectnessValidator.h"
#include "Benchmarking/DijkstraBenchmark.h"
#include "Timer/Timer.h"
#include "TNR/Grid.h"
#include "Utils/MinMaxCoordsFinder.h"
#include "CH/CHPreprocessor.h"
#include "CH/CHQueryManager.h"
#include "CH/CHPathQueryManager.h"
#include "CH/CHAlternativeQueryManger.h"


// This function will construct Contraction Hierarchies from a given graph. This means preprocessing the graph and
// and saving the structures into a file.
//______________________________________________________________________________________________________________________
void constructCH() {
    Loader graphLoader = Loader("../input/USA-road-t.USA.gr");
    //Loader graphLoader = Loader("../input/Rome-road.gr");
    //Loader graphLoader = Loader("input/Rome-road.gr");
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    CHPreprocessor::preprocessAndSaveWithUnpackingData("../input/USA.USA.CH_new", *graph);
    //CHPreprocessor::preprocessAndSaveWithUnpackingData("../input/Rome_debug3", *graph);
    //CHPreprocessor::preprocessAndSaveWithUnpackingData("input/Rome_test", *graph);
    delete graph;
}

//______________________________________________________________________________________________________________________
void additionalCHPreprocess() {
    Loader chGraphLoader = Loader("../input/USA-road-t.USA.gr");
    ShrinkingGraph * chGraph = chGraphLoader.loadCHWithShortcutsIntoShrinkingGraph("../input/USA.USA.CH_new_shortcuts");
    Loader ranksLoader = Loader("../input/USA.USA.CH_new_ranks");
    vector<unsigned int> ranks;
    ranksLoader.loadRanks(ranks);

    chGraph->removeUnnecesarryEdges(ranks);
    chGraph->flushGraph("../input/USA.USA.CH_add");

    delete chGraph;

}

// Runs Dijkstra and Contraction Hierarchies over some set of trips and compares the run times. Dijkstra and CH are run
// one after each other.
//______________________________________________________________________________________________________________________
void compareDijkstraWithCHMemoryEconomical() {
    Loader dijkstraGraphLoader = Loader("../input/USA-road-t.USA.gr");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();
    Loader tripsLoader = Loader("../input/USA100randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    vector<long long unsigned int> dijkstraDistanes(trips.size());
    double dijkstraTime = DijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistanes);
    delete dijkstraGraph;

    Loader chGraphLoader = Loader("../input/USA.USA.CH_add_ch_graph");
    Graph * chGraph = chGraphLoader.loadCHGraph();
    //Graph * chGraph = chGraphLoader.loadCHGraphWithShortcuts("../input/USA.USA.CH_new_shortcuts");

    vector<long long unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::runAndMeasureOutputAndRetval(trips, *chGraph, chDistances);
    //for (unsigned int i = 0; i < 20; i++) {
    //    printf("Dijkstra / ch for trip %i: %llu / %llu (trips index from 0)\n", i, dijkstraDistanes[i], chDistances[i]);
    //}
    CorectnessValidator::validateVerbose(dijkstraDistanes, chDistances);
    printf("CH were %lf times faster than Dijkstra!\n", dijkstraTime/chTime);

    delete chGraph;

}

//______________________________________________________________________________________________________________________
void compareDDSGwithMyCH() {
    Loader tripsLoader = Loader("../input/USA1000randomTripsNewGen");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    Loader chGraphLoader = Loader("../input/USA.USA.CH_add_ch_graph");
    Graph * chGraph = chGraphLoader.loadCHGraph();

    vector<long long unsigned int> myDistances(trips.size());
    double myTime = CHBenchmark::runAndMeasureOutputAndRetval(trips, *chGraph, myDistances);

    delete chGraph;

    DDSGLoader ddsgGraphLoader = DDSGLoader("../input/USA.USA.DDSG.ch");
    vector<unsigned int> ranks(0);
    Graph * ddsgGraph = ddsgGraphLoader.loadGraphWithRanks(ranks);

    vector<long long unsigned int> ddsgDistances(trips.size());
    double ddsgTime = CHBenchmarkWithRanks::runAndMeasureOutputAndRetval(trips, *ddsgGraph, ranks, ddsgDistances);

    CorectnessValidator::validateVerbose(myDistances, ddsgDistances);
    printf("Their implementation was %lf times faster than mine!\n", myTime/ddsgTime);

    delete ddsgGraph;

}

//______________________________________________________________________________________________________________________
void compareQueryAlgorithms() {
    Loader tripsLoader = Loader("../input/USA1000randomTripsNewGen");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    Loader oldGraphLoader = Loader("../input/USA.USA.CH_add_ch_graph");
    Graph * oldGraph = oldGraphLoader.loadCHGraph();

    vector<long long unsigned int> oldDistances(trips.size());
    double oldTime = CHBenchmark::runAndMeasureOutputAndRetval(trips, *oldGraph, oldDistances);

    //delete oldGraph;
    //vector< pair < unsigned int, unsigned int > > oneTrip(1);
    //oneTrip[0].first = trips[0].first;
    //oneTrip[0].second = trips[0].second;

    vector<long long unsigned int> newDistances(trips.size());
    double newTime = CHBenchmarkWithRanks::runNewQueryAlgorithmMeasureOutputAndRetval(trips, *oldGraph, newDistances);

    CorectnessValidator::validateVerbose(oldDistances, newDistances);
    printf("New query algorithm was %lf times faster than the old one!\n", oldTime/newTime);

    delete oldGraph;

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

    Loader tripsLoader = Loader("../input/rome_1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    vector<long long unsigned int> dijkstraDistanes(trips.size());
    vector<long long unsigned int> chDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistanes);
    double chTime = CHBenchmark::runAndMeasureOutputAndRetval(trips, *chGraph, chDistances);
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
    Loader dijkstraGraphLoader = Loader("../input/USA-road-t.USA.gr");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    Loader tripsLoader = Loader("../input/USA1000randomTripsNewGen");
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
    Loader chGraphLoader = Loader("../input/USA.USA.CH_add_ch_graph");
    Graph * chGraph = chGraphLoader.loadCHGraph();

    Loader tripsLoader = Loader("../input/USA1000randomTripsNewGen");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    unsigned int chosenTrip = 1;

    CHQueryManager qm;
    long long unsigned int distance = qm.findDistance(trips.at(chosenTrip).first, trips.at(chosenTrip).second, *chGraph);
    printf("Returned distance: %llu\n", distance);

    delete chGraph;
}

//______________________________________________________________________________________________________________________
void runOneDDSGQuery() {
    DDSGLoader ddsgGraphLoader = DDSGLoader("../input/USA.USA.DDSG.ch");
    vector<unsigned int> ranks(0);
    Graph * ddsgGraph = ddsgGraphLoader.loadGraphWithRanks(ranks);

    Loader tripsLoader = Loader("../input/USA1000randomTripsNewGen");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    unsigned int chosenTrip = 895;

    CHAlternativeQueryManager qm(ranks, *ddsgGraph);
    long long unsigned int distance = qm.findDistance(trips.at(chosenTrip).first, trips.at(chosenTrip).second);
    printf("Returned distance: %llu\n", distance);

    delete ddsgGraph;
}

//______________________________________________________________________________________________________________________
void DIMACStoDDSG() {
    Loader loader = Loader("../input/USA-road-t.USA.gr");
    loader.transformToDDSG("../input/USA-road-t.USA.ddsg");
}

// Simple main function, uncomment the function you want to use.
//______________________________________________________________________________________________________________________
int main() {
    //constructCH();
    //additionalCHPreprocess();
    //compareDijkstraWithCH();
    //compareDijkstraWithCHMemoryEconomical();
    //runOneCHQuery();
    //runOneDDSGQuery();
    //getDijkstraPathForTrip();
    //getCHPathForTrip();
    //DIMACStoDDSG();
    compareDDSGwithMyCH();
    //compareQueryAlgorithms();

    return 0;
}