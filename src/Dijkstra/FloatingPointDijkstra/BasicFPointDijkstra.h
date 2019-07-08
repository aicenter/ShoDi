//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_BASICFPOINTDIJKSTRA_H
#define CONTRACTION_HIERARCHIES_BASICFPOINTDIJKSTRA_H

#include "../../GraphBuilding/Structures/FloatingPointStructures/FPointGraph.h"
#include <vector>

using namespace std;

// A simple implementation of a basic Dijkstra's algorithm, which we compare the CH with.
//______________________________________________________________________________________________________________________
class BasicFPointDijkstra {
public:
    static double run(const unsigned int source, const unsigned int target, const FPointGraph & graph);
    static double runWithPathOutput(const unsigned int source, const unsigned int target, const FPointGraph & graph);
private:
    static void outputPath(const unsigned int x, double * dist, const vector < vector < unsigned int > > & prev);
};

#endif //CONTRACTION_HIERARCHIES_BASICFPOINTDIJKSTRA_H
