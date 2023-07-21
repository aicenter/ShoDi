/******************************************************************************
* File:             DistanceMatrixComputorFast.h
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/

#ifndef SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H
#define SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H

#include <vector>
#include "Distance_matrix_travel_time_provider.h"
#include "DistanceMatrixComputor.h"

using namespace std;

class DistanceMatrixComputorFast : public DistanceMatrixComputor<vector<dist_t>> {
public:

    Distance_matrix_travel_time_provider * getDistanceMatrixInstance() override;

    vector<dist_t> loadGraph(GraphLoader &graphLoader, unsigned int precisionLoss) override;

    void computeDistanceMatrix(const vector<dist_t> &graphData) override;
};


#endif //SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H
