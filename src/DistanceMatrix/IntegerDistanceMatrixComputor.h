//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#ifndef CONTRACTION_HIERARCHIES_INTEGERDISTANCEMATRIXCOMPUTOR_H
#define CONTRACTION_HIERARCHIES_INTEGERDISTANCEMATRIXCOMPUTOR_H

#include "../GraphBuilding/Structures/IntegerStructures/IntegerGraph.h"
#include "IntegerDistanceMatrix.h"

using namespace std;

// This class can compute a full distance matrix for a given IntegerGraph. Meaning all possible pairs
// of source -> target distances will be computed and stored. Finding a shortest path using a distance matrix is the
// fastest possible option, since each query is then answered using a single table lookup. The memory requirements
// are pretty high though, so this can not be used all the time. This particular class uses 'n' runs of Dijkstra
// to compute the full distance matrix (where 'n' is the number of nodes). This should be fast enough for small graphs
// especially since the road graphs are usually very sparse.
//______________________________________________________________________________________________________________________
class IntegerDistanceMatrixComputor {
public:
    void computeDistanceMatrix(const IntegerGraph & graph);
    void computeDistanceMatrixInReversedGraph(const IntegerGraph & graph);
    void outputDistanceMatrixToFile(string path);
    IntegerDistanceMatrix * getDistanceMatrixInstance();
private:
    void fillDistanceMatrixRow(const unsigned int rowID, const IntegerGraph & graph, bool useReversedGraph = false);
    vector<vector<unsigned int>> distanceTable;
};


#endif //CONTRACTION_HIERARCHIES_INTEGERDISTANCEMATRIXCOMPUTOR_H
