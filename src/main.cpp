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
#include "CH/CHDistanceQueryManager.h"

// This function constructs a 'Contraction Hierarchy' from a given graph a saves it into a binary file in a format
// described briefly in the 'DDSGLoader.h' file.
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

// Loads two 'Contraction Hierarchies' and runs a set of queries on both of them. Running times are compared and also
// output is validated (it only checks if the output of both 'CHs' is the same. If both 'CHs' aren't working correctly,
// this function won't detect that).
//______________________________________________________________________________________________________________________
void compareMineWithReference() {
    Loader tripsLoader = Loader("../input/BAY1000randomTrips");
    //Loader tripsLoader = Loader("../input/USA1000randomTripsNewGen");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader myLoader = DDSGLoader("../input/USA.BAY.MY.DDSG.ch");
    FlagsGraph * myCH = myLoader.loadFlagsGraph();

    vector<long long unsigned int> myDistances(trips.size());
    double myTime = CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *myCH, myDistances);

    delete myCH;

    DDSGLoader theirLoader = DDSGLoader("../input/USA.BAY.DDSG.ch");
    FlagsGraph * theirCH = theirLoader.loadFlagsGraph();

    vector<long long unsigned int> theirDistances(trips.size());
    double theirTime = CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *theirCH, theirDistances);

    CorectnessValidator::validateVerbose(myDistances, theirDistances);
    printf("Their implementation was %lf times faster than mine!\n", myTime/theirTime);

    delete theirCH;

}

// This function runs a set of queries first using a given 'Contraction Hierarchy' and then also with 'Dijkstra'.
// The output is validated, since 'Dijkstra' should give correct results, if there are any mismatches, it probably
// means that the 'Contraction Hierarchy' implementation isn't working correctly. Also the time of both algorithms
// is measured and the speedup of 'Contraction Hierarchies' in comparison with 'Dijkstra' is also computed and printed.
//______________________________________________________________________________________________________________________
void compareCHWithDijkstra() {
    Loader tripsLoader = Loader("../input/BAY1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader("../input/USA.BAY.MY.DDSG.ch");
    FlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<long long unsigned int> chDistances(trips.size());
    double chTime = CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, chDistances);

    delete ch;

    Loader dijkstraLoader = Loader("../input/USA-road-t.BAY.gr");
    Graph * dijkstraGraph = dijkstraLoader.loadGraph();

    vector<long long unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = DijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistances);

    CorectnessValidator::validateVerbose(chDistances, dijkstraDistances);
    printf("Contraction Hierarchies were %lf times faster than Dijkstra!\n", dijkstraTime/chTime);

    delete dijkstraGraph;

}

// Runs a set of queries in a given 'Contraction Hierarchy'. Sum of the query times is computed and returned.
// Average time per query can be simply counted by dividing the result by the amount of queries.
//______________________________________________________________________________________________________________________
void runCHBenchmark() {
    Loader tripsLoader = Loader("../input/BAY1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader loader = DDSGLoader("../input/USA.BAY.MY.DDSG.ch");
    FlagsGraph * ch = loader.loadFlagsGraph();

    vector<long long unsigned int> distances(trips.size());
    CHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, distances);
}

// Will print the node sequence with edge lengths on a certain path computed by Dijkstra - can be used for debug.
//______________________________________________________________________________________________________________________
void getDijkstraPathForTrip() {
    Loader dijkstraGraphLoader = Loader("../input/USA-road-t.BAY.gr");
    Graph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    Loader tripsLoader = Loader("../input/BAY1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    unsigned int chosenTrip = 1;
    BasicDijkstra::runWithPathOutput(trips[chosenTrip].first, trips[chosenTrip].second, *dijkstraGraph);

    delete dijkstraGraph;

}

// Will print the node sequence on a certain path computed by Contraction Hierarchies - this means that the paths will
// be unpacked from the shortcuts. Can be used for debug, especially if CH returns different paths than Dijkstra.
// WARNING: 'CH' currently doesn't compute correct paths, so using this isn't recommended until the 'CHPathQueryManager'
// is fixed.
//______________________________________________________________________________________________________________________
void getCHPathForTrip() {
    DDSGLoader chLoader = DDSGLoader("../input/USA.BAY.MY.DDSG.ch");
    FlagsGraphWithUnpackingData * chGraph = chLoader.loadFlagsGraphWithUnpackingData();

    Loader tripsLoader = Loader("../input/BAY1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    unsigned int chosenTrip = 1;

    CHPathQueryManager queryManager(*chGraph);
    //chGraph->debugPrint();
    long long unsigned int distance = queryManager.findDistance(trips[chosenTrip].first, trips[chosenTrip].second);
    printf("Returned distance: %llu\n", distance);

    delete chGraph;
}

// Transforms a graph from the DIMACS challenge format into the .ddsg format used by the Karlsruhe researchers
// in their implementation. The output format isn't used anywhere in this project.
//______________________________________________________________________________________________________________________
void DIMACStoDDSG() {
    Loader loader = Loader("../input/USA-road-t.BAY.gr");
    loader.transformToDDSG("../input/USA-road-t.BAY.ddsg");
}

// Simple main function, uncomment the function you want to use.
//______________________________________________________________________________________________________________________
int main() {
    //constructDDSGCH();
    //runCHBenchmark();

    compareCHWithDijkstra();
    //compareMineWithReference();
    //getDijkstraPathForTrip();
    //getCHPathForTrip();
    //DIMACStoDDSG();

    return 0;
}