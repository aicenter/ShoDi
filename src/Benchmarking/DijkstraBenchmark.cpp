//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#include <string>
#include "../Dijkstra/BasicDijkstra.h"
#include "../Timer/Timer.h"
#include "DijkstraBenchmark.h"

//______________________________________________________________________________________________________________________
void DijkstraBenchmark::runAndMeasure(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph) {
    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        BasicDijkstra::run(trips.at(i).first, trips.at(i).second, graph);
    }

    dijkstraTimer.finish();
    dijkstraTimer.printMeasuredTime();
}

//______________________________________________________________________________________________________________________
void DijkstraBenchmark::runAndMeasureWithOutput(const vector<pair<unsigned int, unsigned int> > &trips, const Graph &graph, vector<unsigned int> &distances) {
    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = BasicDijkstra::run(trips.at(i).first, trips.at(i).second, graph);
    }

    dijkstraTimer.finish();
    dijkstraTimer.printMeasuredTime();
}

//______________________________________________________________________________________________________________________
double DijkstraBenchmark::runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < unsigned int > & distances) {
    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = BasicDijkstra::run(trips.at(i).first, trips.at(i).second, graph);
    }

    dijkstraTimer.finish();
    dijkstraTimer.printMeasuredTime();
    return dijkstraTimer.getMeasuredTimeInSeconds();
}