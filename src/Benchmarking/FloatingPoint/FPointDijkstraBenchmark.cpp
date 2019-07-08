//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#include "../../Dijkstra/FloatingPointDijkstra/BasicFPointDijkstra.h"
#include "FPointDijkstraBenchmark.h"
#include "Timer.h"

//______________________________________________________________________________________________________________________
void FPointDijkstraBenchmark::runAndMeasure(const vector < pair < unsigned int, unsigned int> > & trips, const FPointGraph & graph) {
    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        BasicFPointDijkstra::run(trips.at(i).first, trips.at(i).second, graph);
    }

    dijkstraTimer.finish();
    dijkstraTimer.printMeasuredTime();
}

//______________________________________________________________________________________________________________________
void FPointDijkstraBenchmark::runAndMeasureWithOutput(const vector<pair<unsigned int, unsigned int> > &trips, const FPointGraph &graph, vector< double > &distances) {
    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = BasicFPointDijkstra::run(trips.at(i).first, trips.at(i).second, graph);
    }

    dijkstraTimer.finish();
    dijkstraTimer.printMeasuredTime();
}

//______________________________________________________________________________________________________________________
double FPointDijkstraBenchmark::runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, const FPointGraph & graph, vector < double > & distances) {
    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = BasicFPointDijkstra::run(trips.at(i).first, trips.at(i).second, graph);
    }

    dijkstraTimer.finish();
    dijkstraTimer.printMeasuredTime();
    return dijkstraTimer.getMeasuredTimeInSeconds();
}