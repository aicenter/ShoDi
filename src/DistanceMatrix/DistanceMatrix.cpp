//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include <cstdio>
#include <climits>
#include <fstream>
#include <cmath>
#include <limits>
#include <iostream>
#include <boost/numeric/conversion/cast.hpp>
#include "DistanceMatrix.h"

//______________________________________________________________________________________________________________________
DistanceMatrix::DistanceMatrix(const unsigned int nodes) : nodesCnt(nodes), distances(((size_t) nodes) * ((size_t) nodes)) {

}

//______________________________________________________________________________________________________________________
DistanceMatrix::DistanceMatrix(vector<dist_t> &&distMatrix) : nodesCnt(boost::numeric_cast<unsigned int>(sqrt(distMatrix.size()))),
                                                           distances(move(distMatrix)) {

}

//______________________________________________________________________________________________________________________
dist_t DistanceMatrix::findDistance(const unsigned int start, const unsigned int goal) {
    return distances[((size_t) start) * ((size_t) nodesCnt) + ((size_t) goal)];
}

//______________________________________________________________________________________________________________________
void DistanceMatrix::setDistance(unsigned int source, unsigned int target, dist_t distance) {
    distances[((size_t) source) * ((size_t) nodesCnt) + ((size_t) target)] = distance;
}

//______________________________________________________________________________________________________________________
void DistanceMatrix::printInfo() {
    const dist_t max = std::numeric_limits<dist_t>::max();
    const dist_t half = max / 2;
    unsigned int halfCnt = 0;
    unsigned int maxCnt = 0;
    for(unsigned int i = 0; i < nodesCnt; i++) {
        for(unsigned int j = 0; j < nodesCnt; j++) {
            if (distances[((size_t) i) * ((size_t) nodesCnt) + ((size_t) j)] == max) {
                maxCnt++;
            }
            if (distances[((size_t) i) * ((size_t) nodesCnt) + ((size_t) j)] >= half) {
                halfCnt++;
            }
        }
    }

    const auto optCount = distances.size();
    cout << "Computed distance matrix info." << endl;
    cout << "Distance matrix contains " << maxCnt << " UINF values. That is " << (double) maxCnt / optCount << " %." << endl;
    cout << "Distance matrix contains " << halfCnt << " values that are at least half of std::numeric_limits<dist_t>::max(). That is "
        << (double) halfCnt / optCount << " %." << endl;
}

const vector<dist_t> &DistanceMatrix::getRawData() {
    return distances;
}

unsigned int DistanceMatrix::nodes() {
    return nodesCnt;
}
