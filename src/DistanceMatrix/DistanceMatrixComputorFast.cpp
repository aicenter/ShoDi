/******************************************************************************
* File:             DistanceMatrixComputorFast.cpp
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/


#include "DistanceMatrixComputorFast.h"
#include "GraphBuilding/Structures/AdjMatrixGraph.h"
#include "johnson.hpp"

void DistanceMatrixComputorFast::computeDistanceMatrix(const vector<dist_t> &graphAdjMatrix) {
    auto* graph = johnson::johnson_init(graphAdjMatrix);
    distanceTable = std::make_unique<dist_t[]>(graphAdjMatrix.size());
    size = static_cast<unsigned>(std::sqrt(graphAdjMatrix.size()));
    johnson::johnson_parallel(graph, distanceTable.get());
}

vector<dist_t> DistanceMatrixComputorFast::loadGraph(GraphLoader &graphLoader, unsigned int precisionLoss) {
    AdjMatrixGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, precisionLoss);

    return graph.matrix();
}

Distance_matrix_travel_time_provider *DistanceMatrixComputorFast::getDistanceMatrixInstance() {
    return new Distance_matrix_travel_time_provider(std::move(distanceTable), size);
}
