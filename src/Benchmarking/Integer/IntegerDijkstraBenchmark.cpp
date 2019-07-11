//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#include <string>
#include "../../Dijkstra/IntegerDijkstra/BasicIntegerDijkstra.h"
#include "../../Timer/Timer.h"
#include "IntegerDijkstraBenchmark.h"

//______________________________________________________________________________________________________________________
void IntegerDijkstraBenchmark::runAndMeasure(const vector < pair < unsigned int, unsigned int> > & trips, const IntegerGraph & graph) {
    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        BasicIntegerDijkstra::run(trips.at(i).first, trips.at(i).second, graph);
    }

    dijkstraTimer.finish();
    dijkstraTimer.printMeasuredTime();
}

//______________________________________________________________________________________________________________________
void IntegerDijkstraBenchmark::runAndMeasureWithOutput(const vector<pair<unsigned int, unsigned int> > &trips, const IntegerGraph &graph, vector<long long unsigned int> &distances) {
    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = BasicIntegerDijkstra::run(trips.at(i).first, trips.at(i).second, graph);
    }

    dijkstraTimer.finish();
    dijkstraTimer.printMeasuredTime();
}

//______________________________________________________________________________________________________________________
double IntegerDijkstraBenchmark::runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, const IntegerGraph & graph, vector < long long unsigned int > & distances) {
    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = BasicIntegerDijkstra::run(trips.at(i).first, trips.at(i).second, graph);
    }

    dijkstraTimer.finish();
    dijkstraTimer.printMeasuredTime();
    return dijkstraTimer.getMeasuredTimeInSeconds();
}