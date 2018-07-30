//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#include <string>
#include <BasicDijkstra.h>
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