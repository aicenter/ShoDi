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
    distanceTable.resize(nodesCnt, vector<unsigned int>(nodesCnt));

    for(unsigned int i = 0; i < nodesCnt; i++) {
        if (i % 100 == 0) {
            printf("\rComputed %u rows of the distance matrix.", i);
        }
        fillDistanceMatrixRow(i, graph);
    }

    printf("\rComputed %u rows of the distance matrix.\n", nodesCnt);
}

//______________________________________________________________________________________________________________________
void DistanceMatrixComputor::computeDistanceMatrixInReversedGraph(const Graph & graph) {
    unsigned int nodesCnt = graph.nodes();
    distanceTable.resize(nodesCnt, vector<unsigned int>(nodesCnt));

    for(unsigned int i = 0; i < nodesCnt; i++) {
        if (i % 100 == 0) {
            printf("\rComputed %u rows of the distance matrix.", i);
        }
        fillDistanceMatrixRow(i, graph, true);
    }

    printf("\rComputed %u rows of the distance matrix.\n", nodesCnt);
}

//______________________________________________________________________________________________________________________
void DistanceMatrixComputor::fillDistanceMatrixRow(const unsigned int rowID, const Graph & graph, bool useReversedGraph) {
    unsigned int n = graph.nodes();
    unsigned int * distance = new unsigned int[n];

    for(unsigned int i = 0; i < n; i++) {
        distance[i] = UINT_MAX;
    }

    distance[rowID] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(rowID, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        if (useReversedGraph) {
            const vector < pair < unsigned int, unsigned int > > & neighbours = graph.incomingEdges(current.ID);
            for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
                long long unsigned int newDistance = current.weight + neighbours.at(i).second;
                if (newDistance < distance[neighbours.at(i).first]) {
                    distance[neighbours.at(i).first] = newDistance;
                    q.push(DijkstraNode(neighbours.at(i).first, newDistance));
                }
            }
        } else {
            const vector < pair < unsigned int, unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
            for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
                long long unsigned int newDistance = current.weight + neighbours.at(i).second;
                if (newDistance < distance[neighbours.at(i).first]) {
                    distance[neighbours.at(i).first] = newDistance;
                    q.push(DijkstraNode(neighbours.at(i).first, newDistance));
                }
            }
        }

        q.pop();

    }

    for(unsigned int i = 0; i < n; i++) {
        distanceTable[rowID][i] = distance[i];
    }
    delete [] distance;
}

//______________________________________________________________________________________________________________________
void DistanceMatrixComputor::outputDistanceMatrixToFile(string path) {
    printf("Outputting the distance matrix.\n");
    ofstream output;
    output.open ( path + ".xdm", ios::binary );
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (path + ".xdm").c_str());
    }

    char c1, c2, c3;
    c1 = 'X';
    c2 = 'D';
    c3 = 'M';
    output.write(&c1, sizeof (c1));
    output.write(&c2, sizeof (c2));
    output.write(&c3, sizeof (c3));
    unsigned int nodes = distanceTable.size();
    output.write((char *) &nodes, sizeof(nodes));

    for(unsigned int i = 0; i < nodes; i++) {
        for(unsigned int j = 0; j < nodes; j++) {
            output.write((char *) &distanceTable[i][j], sizeof(distanceTable[i][j]));
        }
    }

    output.close();
}

//______________________________________________________________________________________________________________________
DistanceMatrix * DistanceMatrixComputor::getDistanceMatrixInstance() {
    unsigned int n = distanceTable.size();
    DistanceMatrix * retval = new DistanceMatrix(n);
    for(unsigned int i = 0; i < n; ++i) {
        for(unsigned int j = 0; j < n; ++j) {
            retval->setDistance(i, j, distanceTable[i][j]);
        }
    }
    return retval;
}