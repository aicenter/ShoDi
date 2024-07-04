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



class DistanceMatrixComputorFast : public DistanceMatrixComputor<std::vector<dist_t>> {
public:

    Distance_matrix_travel_time_provider * getDistanceMatrixInstance() override;

    std::vector<dist_t> loadGraph(GraphLoader &graphLoader, int scaling_factor) override;

    void computeDistanceMatrix(const std::vector<dist_t> &graphData) override;
};


#endif //SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H
