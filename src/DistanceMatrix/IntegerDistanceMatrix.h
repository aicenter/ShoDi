//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#ifndef CONTRACTION_HIERARCHIES_INTEGERDISTANCEMATRIX_H
#define CONTRACTION_HIERARCHIES_INTEGERDISTANCEMATRIX_H

#include <vector>

using namespace std;

// Represents a Distance Matrix for a given graph. The Distance Matrix is simply a n*n table ('n' is the number of nodes
// in the graph) containing precomputed distances between all pairs of nodes. The Distance Matrix is the fastest way
// to obtain shortest paths, as each query is then only a simple table lookup. The price for this is the memory
// complexity, which is really high (n^2). This means, that for bigger graphs, we are not able to store the Distance
// Matrix in memory at once, making it impossible to use it. Here, the Distance Matrix is used for comparison to get an
// idea of how much slower the other methods are in comparison with the Distance Matrix approach.
//______________________________________________________________________________________________________________________
class IntegerDistanceMatrix {
public:
    IntegerDistanceMatrix(const unsigned int nodes);
    unsigned int findDistance(const unsigned int source, const unsigned int target);
    void setDistance(unsigned int source, unsigned int target, unsigned int distance);
private:
    vector<vector<unsigned int>> distances;
};


#endif //CONTRACTION_HIERARCHIES_INTEGERDISTANCEMATRIX_H
