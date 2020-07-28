//
// Created by Jan Neumann (neumaja5@fel.cvut.cz) on 28.07.20.
//

#include "DistanceMatrixComputorFast.h"

#include "johnson.hpp"

void DistanceMatrixComputorFast::computeDistanceMatrix(const vector<int> &graphAdjMatrix) {
    auto * graph = johnson::johnson_init(graphAdjMatrix);
    distanceTable.resize(graphAdjMatrix.size());
    johnson::johnson_parallel(graph, distanceTable.data());
}

void DistanceMatrixComputorFast::computeDistanceMatrix(GraphLoader &graphLoader) {
    vector<int> adjMatrix = graphLoader.loadAdjacencyMatrix();
    computeDistanceMatrix(adjMatrix);
}

DistanceMatrix *DistanceMatrixComputorFast::getDistanceMatrixInstance() {
    return new DistanceMatrix(std::move(distanceTable));
}
