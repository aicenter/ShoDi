//
// Author: Xenty (Michal Cvach)
// Created on: 6.8.18
//

#include <string>
#include "CHBenchmark.h"
#include "../CH/CHQueryManager.h"
#include "../CH/CHMyCHVersionNewQueryManager.h"
#include "../Timer/Timer.h"

//______________________________________________________________________________________________________________________
void CHBenchmark::runAndMeasure(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph) {
    CHQueryManager queryManager;

    Timer chTimer("Contraction hierarchies trips benchmark");
    chTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        queryManager.findDistance(trips.at(i).first, trips.at(i).second, graph);
    }

    chTimer.finish();
    chTimer.printMeasuredTime();
}

//______________________________________________________________________________________________________________________
void CHBenchmark::runAndMeasureWithOutput(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < long long unsigned int > & distances) {
    CHQueryManager queryManager;

    Timer chTimer("Contraction hierarchies trips benchmark");
    chTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second, graph);
    }

    chTimer.finish();
    chTimer.printMeasuredTime();
}

//______________________________________________________________________________________________________________________
double CHBenchmark::runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < long long unsigned int > & distances) {
    CHQueryManager queryManager;

    Timer chTimer("Contraction hierarchies trips benchmark");
    chTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second, graph);
    }

    chTimer.finish();
    chTimer.printMeasuredTime();
    return chTimer.getMeasuredTimeInSeconds();
}

//______________________________________________________________________________________________________________________
double CHBenchmark::runNewQueryAlgorithmMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < long long unsigned int > & distances) {
    CHMyChVersionNewQueryManager queryManager(graph);

    Timer chTimer("Contraction hierarchies trips benchmark");
    chTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        //printf("~~~ Start of query: %u ~~~\n", i);
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
        //printf("~~~ End of query: %u ~~~\n", i);
    }

    chTimer.finish();
    chTimer.printMeasuredTime();
    return chTimer.getMeasuredTimeInSeconds();
}