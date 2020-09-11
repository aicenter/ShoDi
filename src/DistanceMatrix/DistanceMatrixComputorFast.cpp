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
    auto * graph = johnson::johnson_init(graphAdjMatrix);
    distanceTable.resize(graphAdjMatrix.size());
    johnson::johnson_parallel(graph, distanceTable.data());
}

void DistanceMatrixComputorFast::computeDistanceMatrix(GraphLoader &graphLoader, unsigned int precisionLoss) {
    AdjMatrixGraph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, precisionLoss);

    vector<dist_t> adjMatrix = graph.matrix();
    computeDistanceMatrix(adjMatrix);
}

DistanceMatrix *DistanceMatrixComputorFast::getDistanceMatrixInstance() {
    return new DistanceMatrix(std::move(distanceTable));
}
