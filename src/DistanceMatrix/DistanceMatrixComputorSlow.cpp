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
#include "constants.h"

//______________________________________________________________________________________________________________________
void DistanceMatrixComputorSlow::computeDistanceMatrix(const Graph & graph) {
    const unsigned int nodesCnt = graph.nodes();

    distanceTable.resize(((size_t) nodesCnt) * ((size_t) nodesCnt));

    for (unsigned int i = 0; i < nodesCnt; ++i) {
        if (i % 100 == 0) {
            cout << "\rComputed " << i << '/' << nodesCnt << " rows of the distance matrix.";
        }
        fillDistanceMatrixRow(i, graph);
    }

    cout << "\rComputed " << nodesCnt << '/' << nodesCnt << " rows of the distance matrix." << endl;
}

//______________________________________________________________________________________________________________________
void DistanceMatrixComputorSlow::computeDistanceMatrixInReversedGraph(const Graph & graph) {
    const unsigned int nodesCnt = graph.nodes();

    distanceTable.resize(((size_t) nodesCnt) * ((size_t) nodesCnt));

    for (unsigned int i = 0; i < nodesCnt; ++i) {
        if (i % 100 == 0) {
            cout << "\rComputed " << i << '/' << nodesCnt << " rows of the distance matrix.";
        }
        fillDistanceMatrixRow(i, graph, true);
    }

    cout << "\rComputed " << nodesCnt << '/' << nodesCnt << " rows of the distance matrix." << endl;
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
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(rowID, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        if (useReversedGraph) {
            const vector < pair < unsigned int, unsigned int > > & neighbours = graph.incomingEdges(current.ID);
            for (auto &neighbour : neighbours) {
                unsigned int newDistance = current.weight + neighbour.second;
                if (newDistance < distance[neighbour.first]) {
                    distance[neighbour.first] = newDistance;
                    q.push(DijkstraNode(neighbour.first, newDistance));
                }
            }
        } else {
            const vector < pair < unsigned int, unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
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
Graph DistanceMatrixComputorSlow::loadGraph(GraphLoader &graphLoader, unsigned int precisionLoss) {
    Graph graph(graphLoader.nodes());
    graphLoader.loadGraph(graph, precisionLoss);
    return graph;
}

//______________________________________________________________________________________________________________________
DistanceMatrix * DistanceMatrixComputorSlow::getDistanceMatrixInstance() {
    DistanceMatrix * retval = new DistanceMatrix(std::move(distanceTable));
    return retval;
}
