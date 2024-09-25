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
template<class T> Distance_matrix_travel_time_provider<T>::Distance_matrix_travel_time_provider(const unsigned int nodes):
    nodesCnt(nodes),
    distances(std::make_unique<dist_t[]>(static_cast<size_t>(nodes) * nodes)) {
}

//______________________________________________________________________________________________________________________
template<class T> Distance_matrix_travel_time_provider<T>::Distance_matrix_travel_time_provider(
        std::unique_ptr<T[]> distMatrix,
        const unsigned int size
):
    nodesCnt(size),
    distances(std::move(distMatrix)) {
}

//______________________________________________________________________________________________________________________
template<class T> dist_t Distance_matrix_travel_time_provider<T>::findDistance(const unsigned int start, const unsigned int goal) const {
    return distances[((size_t) start) * ((size_t) nodesCnt) + ((size_t) goal)];
}

//______________________________________________________________________________________________________________________
template<class T> void Distance_matrix_travel_time_provider<T>::setDistance(unsigned int source, unsigned int target, T distance) {
    distances[((size_t) source) * ((size_t) nodesCnt) + ((size_t) target)] = distance;
}

//______________________________________________________________________________________________________________________
template<class T> void Distance_matrix_travel_time_provider<T>::printInfo() {
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
    std::cout << "Computed distance matrix info." << std::endl;
    std::cout << "Distance matrix contains " << maxCnt << " UINF values. That is " << (double) maxCnt / (double) optCount << " %." << std::endl;
    std::cout << "Distance matrix contains " << halfCnt << " values that are at least half of std::numeric_limits<dist_t>::max(). That is "
        << (double) halfCnt / (double) optCount << " %." << std::endl;
}

template<class T> const std::unique_ptr<T[]> &Distance_matrix_travel_time_provider<T>::getRawData() {
    return distances;
}

template<class T> unsigned int Distance_matrix_travel_time_provider<T>::nodes() const {
    return nodesCnt;
}
