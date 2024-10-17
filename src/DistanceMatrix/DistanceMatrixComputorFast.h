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


template <class IntType>
class DistanceMatrixComputorFast : public DistanceMatrixComputor<IntType> {
public:

    // Distance_matrix_travel_time_provider<IntType>* getDistanceMatrixInstance() override;

    std::vector<dist_t> loadGraph(GraphLoader &graphLoader, int scaling_factor);

    std::unique_ptr<IntType[]> compute_and_get_distance_matrix(GraphLoader& graphLoader, int scaling_factor) override;

    void computeDistanceMatrix(const std::vector<dist_t>& graphAdjMatrix);
};

#include "DistanceMatrixComputorFast.tpp"

#endif //SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H
