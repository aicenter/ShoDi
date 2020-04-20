//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include <cstdio>
#include <climits>
#include "DistanceMatrix.h"

//______________________________________________________________________________________________________________________
DistanceMatrix::DistanceMatrix(const unsigned int nodes) : distances(nodes, vector<unsigned int>(nodes)) {

}

//______________________________________________________________________________________________________________________
unsigned int DistanceMatrix::findDistance(const unsigned int start, const unsigned int goal) {
    return distances[start][goal];
}

//______________________________________________________________________________________________________________________
void DistanceMatrix::setDistance(unsigned int source, unsigned int target, unsigned int distance) {
    distances[source][target] = distance;
}

//______________________________________________________________________________________________________________________
void DistanceMatrix::printInfo() {
    unsigned int half = UINT_MAX / 2;
    unsigned int halfCnt = 0;
    unsigned int maxCnt = 0;
    for(unsigned int i = 0; i < distances.size(); ++i) {
        for(unsigned int j = 0; j < distances.size(); ++j) {
            if(distances[i][j] == UINT_MAX) {
                maxCnt++;
            }
            if(distances[i][j] >= half) {
                halfCnt++;
            }
        }
    }

    unsigned int optCount = distances.size() * distances.size();
    printf("Computed distance matrix info.\n");
    printf("Distance matrix contains %u INF values. That is %f %%.\n", maxCnt, (double) maxCnt / optCount);
    printf("Distance matrix contains %u values that are at least half of UINT_MAX. That is %f %%.\n", halfCnt, (double) halfCnt / optCount);
}