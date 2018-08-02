//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include "GraphBuilder/Loader.h"
#include "Dijkstra/BasicDijkstra.h"
#include "Benchmarking/DijkstraBenchmark.h"
#include "Timer/Timer.h"
#include "TNR/Grid.h"
#include "Utils/MinMaxCoordsFinder.h"
#include "CH/CHPreprocessor.h"

int main() {
    Loader graphLoader = Loader("../input/USA-road-t.COL.gr");
    Graph * graph = graphLoader.loadGraph();
    //Loader tripsLoader = Loader("../input/COL10000randomTrips");
    //vector< pair < unsigned int, unsigned int > > trips;
    //tripsLoader.loadTrips(trips);
    //DijkstraBenchmark::runAndMeasure(trips, *graph);

    //graph->printFirst100NodesDegrees();

    CHPreprocessor::preprocessAndSave("../input/USA.COL.CH", *graph);

    delete graph;
    return 0;
}