//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include <climits>
#include <fstream>
#include <queue>
#include "DistanceMatrixComputor.h"
#include "../Dijkstra/DijkstraNode.h"

//______________________________________________________________________________________________________________________
void DistanceMatrixComputor::computeDistanceMatrix(const Graph & graph) {
    unsigned int nodesCnt = graph.nodes();
    distanceTable.resize(nodesCnt * nodesCnt);

    for(unsigned int i = 0; i < nodesCnt; i++) {
        if (i % 100 == 0) {
            printf("\rComputed %u rows of the distance matrix.", i);
        }
        fillDistanceMatrixRow(i, nodesCnt, graph);
    }

    printf("\rComputed %u rows of the distance matrix.\n", nodesCnt);
}

//______________________________________________________________________________________________________________________
void DistanceMatrixComputor::computeDistanceMatrixInReversedGraph(const Graph & graph) {
    unsigned int nodesCnt = graph.nodes();
    distanceTable.resize(nodesCnt * nodesCnt);

    for(unsigned int i = 0; i < nodesCnt; i++) {
        if (i % 100 == 0) {
            printf("\rComputed %u rows of the distance matrix.", i);
        }
        fillDistanceMatrixRow(i, nodesCnt, graph, true);
    }

    printf("\rComputed %u rows of the distance matrix.\n", nodesCnt);
}

//______________________________________________________________________________________________________________________
void DistanceMatrixComputor::fillDistanceMatrixRow(const unsigned int rowID, const unsigned int nodesCnt, const Graph & graph, bool useReversedGraph) {
    unsigned int n = graph.nodes();
    auto * distance = new int[n];

    for(unsigned int i = 0; i < n; i++) {
        distance[i] = INT_MAX;
    }

    distance[rowID] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(rowID, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        if (useReversedGraph) {
            const vector < pair < unsigned int, unsigned int > > & neighbours = graph.incomingEdges(current.ID);
            for (auto neighbour : neighbours) {
                long long int newDistance = current.weight + neighbour.second;
                if (newDistance < distance[neighbour.first]) {
                    distance[neighbour.first] = newDistance;
                    q.push(DijkstraNode(neighbour.first, newDistance));
                }
            }
        } else {
            const vector < pair < unsigned int, unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
            for (auto neighbour : neighbours) {
                long long int newDistance = current.weight + neighbour.second;
                if (newDistance < distance[neighbour.first]) {
                    distance[neighbour.first] = newDistance;
                    q.push(DijkstraNode(neighbour.first, newDistance));
                }
            }
        }

        q.pop();

    }

    for(unsigned int i = 0; i < n; i++) {
        distanceTable[rowID * nodesCnt + i] = distance[i];
    }
    delete [] distance;
}

//______________________________________________________________________________________________________________________
DistanceMatrix * DistanceMatrixComputor::getDistanceMatrixInstance() {
    DistanceMatrix * retval = new DistanceMatrix(move(distanceTable));
    return retval;
}