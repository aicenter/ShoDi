//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include <boost/numeric/conversion/cast.hpp>
#include <climits>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <stdexcept>
#include <string>
#include "../Dijkstra/DijkstraNode.h"
#include "../constants.h"

template<class IntType> std::unique_ptr<IntType[]> DistanceMatrixComputorSlow<IntType>::compute_and_get_distance_matrix(GraphLoader& graphLoader, int scaling_factor) {
    Graph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, scaling_factor);
    computeDistanceMatrix(graph);
    // return this->distanceTable;
    return this->getDistanceMatrixInstance();
}

//______________________________________________________________________________________________________________________
template<class IntType> void DistanceMatrixComputorSlow<IntType>::computeDistanceMatrix(const Graph& graph) {
    this->size = graph.nodes();

    this->distanceTable = std::make_unique<IntType[]>(((size_t) this->size) * ((size_t) this->size));

    for (unsigned int i = 0; i < this->size; ++i) {
        if (i % 100 == 0) {
            std::cout << "\rComputed " << i << '/' << this->size << " rows of the distance matrix.";
        }
        fillDistanceMatrixRow(i, graph);
    }

    std::cout << "\rComputed " << this->size << '/' << this->size << " rows of the distance matrix." << std::endl;
}

//______________________________________________________________________________________________________________________
template<class IntType> void DistanceMatrixComputorSlow<IntType>::computeDistanceMatrixInReversedGraph(const Graph& graph) {
    const unsigned int nodesCnt = graph.nodes();

    this->distanceTable = std::make_unique<IntType[]>(((size_t) nodesCnt) * ((size_t) nodesCnt));
    this->size = nodesCnt;

    for (unsigned int i = 0; i < nodesCnt; ++i) {
        if (i % 100 == 0) {
            std::cout << "\rComputed " << i << '/' << nodesCnt << " rows of the distance matrix.";
        }
        fillDistanceMatrixRow(i, graph, true);
    }

    std::cout << "\rComputed " << nodesCnt << '/' << nodesCnt << " rows of the distance matrix." << std::endl;
}

//______________________________________________________________________________________________________________________
template<class IntType> void DistanceMatrixComputorSlow<IntType>::fillDistanceMatrixRow(const unsigned int rowID, const Graph& graph, bool useReversedGraph) {
    auto n = (size_t) graph.nodes();
    auto * distance = new IntType[n];

    const IntType max = std::numeric_limits<IntType>::max();
    for(size_t i = 0; i < n; i++) {
        distance[i] = max;
    }

    distance[rowID] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(rowID, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        if (useReversedGraph) {
            const auto& neighbours = graph.incomingEdges(current.ID);
            for (auto &neighbour : neighbours) {
                IntType newDistance = boost::numeric_cast<IntType>(current.weight + neighbour.second);
                if (newDistance < distance[neighbour.first]) {
                    distance[neighbour.first] = newDistance;
                    q.push(DijkstraNode(neighbour.first, newDistance));
                }
            }
        } else {
            const auto& neighbours = graph.outgoingEdges(current.ID);
            for (auto &neighbour : neighbours) {
                IntType newDistance = boost::numeric_cast<IntType>(current.weight + neighbour.second);
                if (newDistance < distance[neighbour.first]) {
                    distance[neighbour.first] = newDistance;
                    q.push(DijkstraNode(neighbour.first, newDistance));
                }
            }
        }

        q.pop();

    }

    for (size_t i = 0; i < (size_t) n; ++i) {
        this->distanceTable[((size_t) rowID) * ((size_t) n) + ((size_t) i)] = distance[i];
    }
    delete [] distance;
}

//______________________________________________________________________________________________________________________
template<class IntType> Graph DistanceMatrixComputorSlow<IntType>::loadGraph(GraphLoader &graphLoader, int scaling_factor) {
    Graph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, scaling_factor);
    return graph;
}
