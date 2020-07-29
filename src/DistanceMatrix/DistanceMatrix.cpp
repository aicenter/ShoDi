//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include <bits/c++config.h>
#include <cstdio>
#include <climits>
#include <fstream>
#include <cmath>
#include <limits>
#include "DistanceMatrix.h"

//______________________________________________________________________________________________________________________
DistanceMatrix::DistanceMatrix(const size_t nodes) : nodesCnt(nodes), distances(nodes * nodes) {

}

//______________________________________________________________________________________________________________________
DistanceMatrix::DistanceMatrix(vector<dist_t> &&distMatrix) : nodesCnt(sqrt(distMatrix.size())),
                                                           distances(move(distMatrix)) {

}

//______________________________________________________________________________________________________________________
dist_t DistanceMatrix::findDistance(const unsigned int start, const unsigned int goal) {
    return distances[start * nodesCnt + goal];
}

//______________________________________________________________________________________________________________________
void DistanceMatrix::setDistance(unsigned int source, unsigned int target, dist_t distance) {
    distances[source * nodesCnt + target] = distance;
}

//______________________________________________________________________________________________________________________
void DistanceMatrix::printInfo() {
    const dist_t max = std::numeric_limits<dist_t>::max();
    const dist_t half = max / 2;
    unsigned int halfCnt = 0;
    unsigned int maxCnt = 0;
    for (unsigned int i = 0; i < nodesCnt; ++i) {
        for (unsigned int j = 0; j < nodesCnt; ++j) {
            if (distances[i * nodesCnt + j] == max) {
                maxCnt++;
            }
            if (distances[i * nodesCnt + j] >= half) {
                halfCnt++;
            }
        }
    }

    const unsigned int optCount = distances.size();
    printf("Computed distance matrix info.\n");
    printf("Distance matrix contains %u UINF values. That is %f %%.\n", maxCnt, (double) maxCnt / optCount);
    printf("Distance matrix contains %u values that are at least half of std::numeric_limist<dist_t>::max(). That is %f %%.\n", halfCnt,
           (double) halfCnt / optCount);
}

const vector<dist_t> &DistanceMatrix::getRawData() {
    return distances;
}

size_t DistanceMatrix::nodes() {
    return nodesCnt;
}
