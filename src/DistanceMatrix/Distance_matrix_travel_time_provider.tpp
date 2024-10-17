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

#include "DistanceMatrixComputorFast.h"
#include "DistanceMatrixComputorSlow.h"
#include "../Timer/Timer.h"

//______________________________________________________________________________________________________________________
template<class IntType> Distance_matrix_travel_time_provider<IntType>::Distance_matrix_travel_time_provider(const unsigned int nodes):
    nodesCnt(nodes),
    distances(std::make_unique<dist_t[]>(static_cast<size_t>(nodes) * nodes)) {
}

//______________________________________________________________________________________________________________________
template<class IntType> Distance_matrix_travel_time_provider<IntType>::Distance_matrix_travel_time_provider(
        std::unique_ptr<IntType[]> distMatrix,
        const unsigned int size
):
    nodesCnt(size),
    distances(std::move(distMatrix)) {
}

//______________________________________________________________________________________________________________________
template<class IntType> Distance_matrix_travel_time_provider<IntType>::Distance_matrix_travel_time_provider(bool fast, GraphLoader& graphLoader, int scaling_factor):
    nodesCnt(graphLoader.nodes())
{
    computeDistanceMatrix(fast, graphLoader, scaling_factor, "Distance Matrix preprocessing");
}

//______________________________________________________________________________________________________________________
template<class IntType> dist_t Distance_matrix_travel_time_provider<IntType>::findDistance(const unsigned int start, const unsigned int goal) const {
    return distances[((size_t) start) * ((size_t) nodesCnt) + ((size_t) goal)];
}

//______________________________________________________________________________________________________________________
template<class IntType> void Distance_matrix_travel_time_provider<IntType>::setDistance(unsigned int source, unsigned int target, IntType distance) {
    distances[((size_t) source) * ((size_t) nodesCnt) + ((size_t) target)] = distance;
}


//______________________________________________________________________________________________________________________
template<class IntType> void Distance_matrix_travel_time_provider<IntType>::computeDistanceMatrix(bool fast, GraphLoader &graphLoader, int scaling_factor, const std::string& timerName) {
    Timer timer(timerName);
    if (fast) {
        DistanceMatrixComputorFast<IntType> computor;

        timer.begin();
        distances = computor.compute_and_get_distance_matrix(graphLoader, scaling_factor);
        timer.finish();
    } else {
        DistanceMatrixComputorSlow<IntType> computor;

        timer.begin();
        distances = computor.compute_and_get_distance_matrix(graphLoader, scaling_factor);
        timer.finish();

    }
    timer.printMeasuredTime();
}

//______________________________________________________________________________________________________________________
template<class IntType> void Distance_matrix_travel_time_provider<IntType>::printInfo() {
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

template<class IntType> const std::unique_ptr<IntType[]> &Distance_matrix_travel_time_provider<IntType>::getRawData() {
    return distances;
}

template<class IntType> unsigned int Distance_matrix_travel_time_provider<IntType>::nodes() const {
    return nodesCnt;
}
