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
#include "Distance_matrix_travel_time_provider.h"

//______________________________________________________________________________________________________________________
Distance_matrix_travel_time_provider::Distance_matrix_travel_time_provider(const unsigned int nodes):
    nodesCnt(nodes),
    distances(std::make_unique<dist_t[]>(static_cast<size_t>(nodes) * nodes)) {
}

//______________________________________________________________________________________________________________________
Distance_matrix_travel_time_provider::Distance_matrix_travel_time_provider(
        std::unique_ptr<dist_t[]> distMatrix,
        const unsigned int size
):
    nodesCnt(size),
    distances(std::move(distMatrix)) {
}

//______________________________________________________________________________________________________________________
dist_t Distance_matrix_travel_time_provider::findDistance(const unsigned int start, const unsigned int goal) const {
    return distances[((size_t) start) * ((size_t) nodesCnt) + ((size_t) goal)];
}

//______________________________________________________________________________________________________________________
void Distance_matrix_travel_time_provider::setDistance(unsigned int source, unsigned int target, dist_t distance) {
    distances[((size_t) source) * ((size_t) nodesCnt) + ((size_t) target)] = distance;
}

//______________________________________________________________________________________________________________________
void Distance_matrix_travel_time_provider::printInfo() {
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

    const auto optCount = nodesCnt * nodesCnt;
    cout << "Computed distance matrix info." << endl;
    cout << "Distance matrix contains " << maxCnt << " UINF values. That is " << (double) maxCnt / (double) optCount << " %." << endl;
    cout << "Distance matrix contains " << halfCnt << " values that are at least half of std::numeric_limits<dist_t>::max(). That is "
        << (double) halfCnt / (double) optCount << " %." << endl;
}

const std::unique_ptr<dist_t[]> &Distance_matrix_travel_time_provider::getRawData() {
    return distances;
}

unsigned int Distance_matrix_travel_time_provider::nodes() const {
    return nodesCnt;
}
