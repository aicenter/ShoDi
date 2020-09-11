/******************************************************************************
* File:             DistanceMatrixComputorFast.h
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/

#ifndef SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H
#define SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H

#include <vector>
#include "DistanceMatrix.h"
#include "DistanceMatrixComputor.h"

using namespace std;

class DistanceMatrixComputorFast : public DistanceMatrixComputor {
public:
    void computeDistanceMatrix(GraphLoader &graphLoader, unsigned int precisionLoss) override;

    DistanceMatrix * getDistanceMatrixInstance() override;

    /**
     * This function will compute the full distance matrix for the given adjacency matrix.
     * The matrix will be stored in the instance of the computor class,
     * so that it can be output or used further.
     *
     * @param graphAdjMatrix[in] The adjacency matrix of a graph for which we want to compute the distance matrix.
     */
    void computeDistanceMatrix(const vector<dist_t> &graphAdjMatrix);

private:
    vector<dist_t> distanceTable;
};


#endif //SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H
