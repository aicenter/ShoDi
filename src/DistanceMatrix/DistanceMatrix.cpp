//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include <cstdio>
#include <climits>
#include <fstream>
#include <cmath>
#include "DistanceMatrix.h"

//______________________________________________________________________________________________________________________
DistanceMatrix::DistanceMatrix(const unsigned int nodes) : nodesCnt(nodes), distances(nodes) {

}

//______________________________________________________________________________________________________________________
DistanceMatrix::DistanceMatrix(vector<int> &&distMatrix) : nodesCnt(sqrt(distMatrix.size())),
                                                           distances(move(distMatrix)) {

}

//______________________________________________________________________________________________________________________
unsigned int DistanceMatrix::findDistance(const unsigned int start, const unsigned int goal) {
    return distances[start * nodesCnt + goal];
}

//______________________________________________________________________________________________________________________
void DistanceMatrix::setDistance(unsigned int source, unsigned int target, unsigned int distance) {
    distances[source * nodesCnt + target] = distance;
}

//______________________________________________________________________________________________________________________
void DistanceMatrix::printInfo() {
    int half = INT_MAX / 2;
    unsigned int halfCnt = 0;
    unsigned int maxCnt = 0;
    for (unsigned int i = 0; i < nodesCnt; ++i) {
        for (unsigned int j = 0; j < nodesCnt; ++j) {
            if (distances[i * nodesCnt + j] == INT_MAX) {
                maxCnt++;
            }
            if (distances[i * nodesCnt + j] >= half) {
                halfCnt++;
            }
        }
    }

    const unsigned int optCount = distances.size();
    printf("Computed distance matrix info.\n");
    printf("Distance matrix contains %u INF values. That is %f %%.\n", maxCnt, (double) maxCnt / optCount);
    printf("Distance matrix contains %u values that are at least half of INT_MAX. That is %f %%.\n", halfCnt,
           (double) halfCnt / optCount);
}

const vector<int> &DistanceMatrix::getRawData() {
    return distances;
}

unsigned int DistanceMatrix::nodes() {
    return nodesCnt;
}
