//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#ifndef CONTRACTION_HIERARCHIES_DISTANCEMATRIXCOMPUTOR_H
#define CONTRACTION_HIERARCHIES_DISTANCEMATRIXCOMPUTOR_H

#include "../GraphBuilding/Structures/Graph.h"
#include "DistanceMatrix.h"

using namespace std;

// This class can compute a full distance matrix for a given Graph. Meaning all possible pairs
// of source -> target distances will be computed and stored. Finding a shortest path using a distance matrix is the
// fastest possible option, since each query is then answered using a single table lookup. The memory requirements
// are pretty high though, so this can not be used all the time. This particular class uses 'n' runs of Dijkstra
// to compute the full distance matrix (where 'n' is the number of nodes). This should be fast enough for small graphs
// especially since the road graphs are usually very sparse.
//______________________________________________________________________________________________________________________
class DistanceMatrixComputor {
public:
    void computeDistanceMatrix(const Graph & graph);
    void computeDistanceMatrixInReversedGraph(const Graph & graph);
    void outputDistanceMatrixToFile(string path);
    DistanceMatrix * getDistanceMatrixInstance();
private:
    void fillDistanceMatrixRow(const unsigned int rowID, const Graph & graph, bool useReversedGraph = false);
    vector<vector<unsigned int>> distanceTable;
};


#endif //CONTRACTION_HIERARCHIES_DISTANCEMATRIXCOMPUTOR_H
