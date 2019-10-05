//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include "IntegerDistanceMatrix.h"

//______________________________________________________________________________________________________________________
IntegerDistanceMatrix::IntegerDistanceMatrix(const unsigned int nodes) : distances(nodes, vector<unsigned int>(nodes)) {

}

// This is basically a query algorithm. Each query is answered using a single table lookup, as we already have all the
// shortest distances precomputed in the matrix.
//______________________________________________________________________________________________________________________
unsigned int IntegerDistanceMatrix::findDistance(const unsigned int source, const unsigned int target) {
    return distances[source][target];
}

// Auxiliary function used during the initialization to set the distances.
//______________________________________________________________________________________________________________________
void IntegerDistanceMatrix::setDistance(unsigned int source, unsigned int target, unsigned int distance) {
    distances[source][target] = distance;
}