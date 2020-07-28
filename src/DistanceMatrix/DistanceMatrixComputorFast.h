//
// Created by Jan Neumann (neumaja5@fel.cvut.cz) on 28.07.20.
//

#ifndef SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H
#define SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H

#include <vector>
#include "DistanceMatrix.h"
#include "DistanceMatrixComputor.h"

using namespace std;

class DistanceMatrixComputorFast : public DistanceMatrixComputor {
public:
    void computeDistanceMatrix(GraphLoader &graphLoader) override;

    DistanceMatrix * getDistanceMatrixInstance() override;

    /**
     * This function will compute the full distance matrix for the given adjacency matrix.
     * The matrix will be stored in the instance of the computor class,
     * so that it can be output or used further.
     *
     * @param graphAdjMatrix[in] The adjacency matrix of a graph for which we want to compute the distance matrix.
     */
    void computeDistanceMatrix(const vector<int> &graphAdjMatrix);

private:
    vector<int> distanceTable;
};


#endif //SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H
