//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include "GraphBuilder/DDSGLoader.h"
#include "GraphBuilder/Loader.h"
#include "Dijkstra/BasicDijkstra.h"
#include "Benchmarking/CHBenchmark.h"
#include "Benchmarking/CorectnessValidator.h"
#include "Benchmarking/DijkstraBenchmark.h"
#include "Timer/Timer.h"
#include "CH/CHPreprocessor.h"
#include "CH/CHPathQueryManager.h"

//______________________________________________________________________________________________________________________
void constructDDSGCH() {
    Timer timer("Whole CH construction timer");
    timer.begin();

    Loader graphLoader = Loader("../input/USA-road-t.USA.gr");
    UpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    CHPreprocessor::preprocessForDDSG(*graph);
    graphLoader.putAllEdgesIntoUpdateableGraph(*graph);
    graph->flushInDdsgFormat("../input/USA.USA.MY.DDSG");

    timer.finish();
    timer.printMeasuredTime();

    delete graph;
}


//______________________________________________________________________________________________________________________
void compareMineWithReference() {
    Loader tripsLoader = Loader("../input/BAY1000randomTrips");
    //Loader tripsLoader = Loader("../input/USA1000randomTripsNewGen");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader myLoader = DDSGLoader("../input/USA.BAY.MY.DDSG.ch");
    FlagsGraph * myCH = myLoader.loadFlagsGraphWithRanks();

    vector<long long unsigned int> myDistances(trips.size());
    double myTime = CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *myCH, myDistances);

    delete myCH;

    DDSGLoader theirLoader = DDSGLoader("../input/USA.BAY.DDSG.ch");
    FlagsGraph * theirCH = theirLoader.loadFlagsGraphWithRanks();

    vector<long long unsigned int> theirDistances(trips.size());
    double theirTime = CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *theirCH, theirDistances);

    CorectnessValidator::validateVerbose(myDistances, theirDistances);
    printf("Their implementation was %lf times faster than mine!\n", myTime/theirTime);

    delete theirCH;

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
    /*Loader chGraphLoader = Loader("../input/Rome-road.gr");
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

    delete chGraph;*/
}

//______________________________________________________________________________________________________________________
void DIMACStoDDSG() {
    Loader loader = Loader("../input/USA-road-t.BAY.gr");
    loader.transformToDDSG("../input/USA-road-t.BAY.ddsg");
}

// Simple main function, uncomment the function you want to use.
//______________________________________________________________________________________________________________________
int main() {
    constructDDSGCH();
    //compareMineWithReference();

    //getDijkstraPathForTrip();
    //getCHPathForTrip();
    //DIMACStoDDSG();

    return 0;
}