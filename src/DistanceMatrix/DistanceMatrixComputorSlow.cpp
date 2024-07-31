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
#include "DistanceMatrixComputorSlow.h"
#include "../Dijkstra/DijkstraNode.h"
#include "../constants.h"

//______________________________________________________________________________________________________________________
void DistanceMatrixComputorSlow::computeDistanceMatrix(const Graph& graph) {
    size = graph.nodes();

    distanceTable = std::make_unique<dist_t[]>(((size_t) size) * ((size_t) size));

    for (unsigned int i = 0; i < size; ++i) {
        if (i % 100 == 0) {
            std::cout << "\rComputed " << i << '/' << size << " rows of the distance matrix.";
        }
        fillDistanceMatrixRow(i, graph);
    }

    std::cout << "\rComputed " << size << '/' << size << " rows of the distance matrix." << std::endl;
}

//______________________________________________________________________________________________________________________
void DistanceMatrixComputorSlow::computeDistanceMatrixInReversedGraph(const Graph & graph) {
    const unsigned int nodesCnt = graph.nodes();

    distanceTable = std::make_unique<dist_t[]>(((size_t) nodesCnt) * ((size_t) nodesCnt));
    size = nodesCnt;

    for (unsigned int i = 0; i < nodesCnt; ++i) {
        if (i % 100 == 0) {
            std::cout << "\rComputed " << i << '/' << nodesCnt << " rows of the distance matrix.";
        }
        fillDistanceMatrixRow(i, graph, true);
    }

    std::cout << "\rComputed " << nodesCnt << '/' << nodesCnt << " rows of the distance matrix." << std::endl;
}

//______________________________________________________________________________________________________________________
void DistanceMatrixComputorSlow::fillDistanceMatrixRow(const unsigned int rowID, const Graph & graph, bool useReversedGraph) {
    size_t n = (size_t) graph.nodes();
    auto * distance = new dist_t[n];

    const dist_t max = std::numeric_limits<dist_t>::max();
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
            const std::vector < std::pair< unsigned int, unsigned int > > & neighbours = graph.incomingEdges(current.ID);
            for (auto &neighbour : neighbours) {
                unsigned int newDistance = current.weight + neighbour.second;
                if (newDistance < distance[neighbour.first]) {
                    distance[neighbour.first] = newDistance;
                    q.push(DijkstraNode(neighbour.first, newDistance));
                }
            }
        } else {
            const std::vector < std::pair< unsigned int, unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
            for (auto &neighbour : neighbours) {
                unsigned int newDistance = current.weight + neighbour.second;
                if (newDistance < distance[neighbour.first]) {
                    distance[neighbour.first] = newDistance;
                    q.push(DijkstraNode(neighbour.first, newDistance));
                }
            }
        }

        q.pop();

    }

    for (size_t i = 0; i < (size_t) n; ++i) {
      distanceTable[((size_t) rowID) * ((size_t) n) + ((size_t) i)] = distance[i];
    }
    delete [] distance;
}

//______________________________________________________________________________________________________________________
Graph DistanceMatrixComputorSlow::loadGraph(GraphLoader &graphLoader, int scaling_factor) {
    Graph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, scaling_factor);
    return graph;
}

//______________________________________________________________________________________________________________________
Distance_matrix_travel_time_provider * DistanceMatrixComputorSlow::getDistanceMatrixInstance() {
    auto* retval = new Distance_matrix_travel_time_provider(std::move(distanceTable), size);
    return retval;
}
