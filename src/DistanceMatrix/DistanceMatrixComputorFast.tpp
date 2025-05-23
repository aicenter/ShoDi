/******************************************************************************
* File:             DistanceMatrixComputorFast.tpp
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/


#include "../GraphBuilding/Structures/AdjMatrixGraph.h"
#include "johnson.hpp"

template<class IntType>
std::unique_ptr<IntType[]> DistanceMatrixComputorFast<IntType>::compute_and_get_distance_matrix(
    GraphLoader& graphLoader,
    int scaling_factor
) {
    std::vector<dist_t> graphData = loadGraph(graphLoader, scaling_factor);
    computeDistanceMatrix(graphData);
    return this->getDistanceMatrixInstance();
}

template<class IntType>
void DistanceMatrixComputorFast<IntType>::computeDistanceMatrix(const std::vector<dist_t>& graphAdjMatrix) {
    auto* graph = johnson::johnson_init(graphAdjMatrix);
    this->distanceTable = std::make_unique<IntType[]>(graphAdjMatrix.size());

    this->size = static_cast<unsigned>(std::sqrt(graphAdjMatrix.size()));
    johnson::johnson_parallel(graph, this->distanceTable.get());
    delete graph;
}

template<class IntType>
std::vector<dist_t> DistanceMatrixComputorFast<IntType>::loadGraph(GraphLoader& graphLoader, int scaling_factor) {
    AdjMatrixGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, scaling_factor);

    return graph.matrix();
}
