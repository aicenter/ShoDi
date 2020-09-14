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

class DistanceMatrixComputorFast : public DistanceMatrixComputor<vector<dist_t>> {
public:

    DistanceMatrix * getDistanceMatrixInstance() override;

    vector<dist_t> loadGraph(GraphLoader &graphLoader, unsigned int precisionLoss) override;

    void computeDistanceMatrix(const vector<dist_t> &graphData) override;

private:
    vector<dist_t> distanceTable;
};


#endif //SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H
