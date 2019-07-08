//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include "GraphBuilding/Loaders/DDSGLoader.h"
#include "GraphBuilding/Loaders/DIMACSLoader.h"
#include "GraphBuilding/Loaders/TripsLoader.h"
#include "GraphBuilding/Loaders/XenGraphLoader.h"
#include "IntegerDijkstra/BasicIntegerDijkstra.h"
#include "Integer/IntegerCHBenchmark.h"
#include "Integer/IntegerCorectnessValidator.h"
#include "Integer/IntegerDijkstraBenchmark.h"
#include "FloatingPoint/FPointDijkstraBenchmark.h"
#include "Timer/Timer.h"
#include "Integer/IntegerCHPreprocessor.h"
#include "Integer/IntegerCHPathQueryManager.h"
#include "Integer/IntegerCHDistanceQueryManager.h"

// This function constructs a 'Contraction Hierarchy' from a given graph a saves it into a binary file in a format
// described briefly in the 'DDSGLoader.h' file.
//______________________________________________________________________________________________________________________
void constructDDSGCH() {
    Timer timer("Whole CH construction timer");
    timer.begin();

    DIMACSLoader graphLoader = DIMACSLoader("../input/USA-road-t.USA.gr");
    IntegerUpdateableGraph * graph = graphLoader.loadUpdateableGraph();
    IntegerCHPreprocessor::preprocessForDDSG(*graph);
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
    TripsLoader tripsLoader = TripsLoader("../input/BAY1000randomTrips");
    //Loader tripsLoader = Loader("../input/USA1000randomTripsNewGen");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader myLoader = DDSGLoader("../input/USA.BAY.MY.DDSG.ch");
    IntegerFlagsGraph * myCH = myLoader.loadFlagsGraph();

    vector<long long unsigned int> myDistances(trips.size());
    double myTime = IntegerCHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *myCH, myDistances);

    delete myCH;

    DDSGLoader theirLoader = DDSGLoader("../input/USA.BAY.DDSG.ch");
    IntegerFlagsGraph * theirCH = theirLoader.loadFlagsGraph();

    vector<long long unsigned int> theirDistances(trips.size());
    double theirTime = IntegerCHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *theirCH, theirDistances);

    IntegerCorectnessValidator::validateVerbose(myDistances, theirDistances);
    printf("Their implementation was %lf times faster than mine!\n", myTime/theirTime);

    delete theirCH;

}

// This function runs a set of queries first using a given 'Contraction Hierarchy' and then also with 'Dijkstra'.
// The output is validated, since 'Dijkstra' should give correct results, if there are any mismatches, it probably
// means that the 'Contraction Hierarchy' implementation isn't working correctly. Also the time of both algorithms
// is measured and the speedup of 'Contraction Hierarchies' in comparison with 'Dijkstra' is also computed and printed.
//______________________________________________________________________________________________________________________
void compareCHWithDijkstra() {
    TripsLoader tripsLoader = TripsLoader("../input/BAY1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader chLoader = DDSGLoader("../input/USA.BAY.MY.DDSG.ch");
    IntegerFlagsGraph * ch = chLoader.loadFlagsGraph();

    vector<long long unsigned int> chDistances(trips.size());
    double chTime = IntegerCHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, chDistances);

    delete ch;

    DIMACSLoader dijkstraLoader = DIMACSLoader("../input/USA-road-t.BAY.gr");
    IntegerGraph * dijkstraGraph = dijkstraLoader.loadGraph();

    vector<long long unsigned int> dijkstraDistances(trips.size());
    double dijkstraTime = IntegerDijkstraBenchmark::runAndMeasureOutputAndRetval(trips, *dijkstraGraph, dijkstraDistances);

    IntegerCorectnessValidator::validateVerbose(chDistances, dijkstraDistances);
    printf("Contraction Hierarchies were %lf times faster than Dijkstra!\n", dijkstraTime/chTime);

    delete dijkstraGraph;

}

// Runs a set of queries in a given 'Contraction Hierarchy'. Sum of the query times is computed and returned.
// Average time per query can be simply counted by dividing the result by the amount of queries.
//______________________________________________________________________________________________________________________
void runCHBenchmark() {
    TripsLoader tripsLoader = TripsLoader("../input/BAY1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    DDSGLoader loader = DDSGLoader("../input/USA.BAY.MY.DDSG.ch");
    IntegerFlagsGraph * ch = loader.loadFlagsGraph();

    vector<long long unsigned int> distances(trips.size());
    IntegerCHBenchmark::runAndMeasureFlagsGraphOutputAndRetval(trips, *ch, distances);
}

// Will print the node sequence with edge lengths on a certain path computed by Dijkstra - can be used for debug.
//______________________________________________________________________________________________________________________
void getDijkstraPathForTrip() {
    DIMACSLoader dijkstraGraphLoader = DIMACSLoader("../input/USA-road-t.BAY.gr");
    IntegerGraph * dijkstraGraph = dijkstraGraphLoader.loadGraph();

    TripsLoader tripsLoader = TripsLoader("../input/BAY1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    unsigned int chosenTrip = 1;
    BasicIntegerDijkstra::runWithPathOutput(trips[chosenTrip].first, trips[chosenTrip].second, *dijkstraGraph);

    delete dijkstraGraph;

}

// Will print the node sequence on a certain path computed by Contraction Hierarchies - this means that the paths will
// be unpacked from the shortcuts. Can be used for debug, especially if CH returns different paths than Dijkstra.
// WARNING: 'CH' currently doesn't compute correct paths, so using this isn't recommended until the 'IntegerCHPathQueryManager'
// is fixed.
//______________________________________________________________________________________________________________________
void getCHPathForTrip() {
    DDSGLoader chLoader = DDSGLoader("../input/USA.BAY.MY.DDSG.ch");
    IntegerFlagsGraphWithUnpackingData * chGraph = chLoader.loadFlagsGraphWithUnpackingData();

    TripsLoader tripsLoader = TripsLoader("../input/BAY1000randomTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    unsigned int chosenTrip = 1;

    IntegerCHPathQueryManager queryManager(*chGraph);
    //chGraph->debugPrint();
    long long unsigned int distance = queryManager.findDistance(trips[chosenTrip].first, trips[chosenTrip].second);
    printf("Returned distance: %llu\n", distance);

    delete chGraph;
}

// Transforms a graph from the DIMACS challenge format into the .ddsg format used by the Karlsruhe researchers
// in their implementation. The output format isn't used anywhere in this project.
//______________________________________________________________________________________________________________________
void DIMACStoDDSG() {
    DIMACSLoader loader = DIMACSLoader("../input/USA-road-t.BAY.gr");
    loader.transformToDDSG("../input/USA-road-t.BAY.ddsg");
}

void testDoubleDijkstra() {
    XenGraphLoader graphLoader = XenGraphLoader("../input/graph.xeng");
    FPointGraph * graph = graphLoader.loadGraph();

    TripsLoader tripsLoader = TripsLoader("../input/doubleExperimentTrips");
    vector< pair < unsigned int, unsigned int > > trips;
    tripsLoader.loadTrips(trips);

    vector<double> dijkstraDistances(trips.size());
    FPointDijkstraBenchmark::runAndMeasureWithOutput(trips, *graph, dijkstraDistances);

    for(unsigned int i = 0; i < 20; i++) {
        printf("Distance for trip %u: %f\n", i, dijkstraDistances[i]);
    }

    delete graph;


}

// Simple main function, uncomment the function you want to use.
//______________________________________________________________________________________________________________________
int main() {
    //constructDDSGCH();
    //runCHBenchmark();

    //compareCHWithDijkstra();
    //compareMineWithReference();
    //getDijkstraPathForTrip();
    //getCHPathForTrip();
    //DIMACStoDDSG();

    testDoubleDijkstra();

    return 0;
}