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
#include <stdexcept> // For std::runtime_error

#include "DistanceMatrixComputorFast.h"
#include "DistanceMatrixComputorSlow.h"
#include "../Timer/Timer.h"

//______________________________________________________________________________________________________________________
template<class IntType> Distance_matrix_travel_time_provider<IntType>::Distance_matrix_travel_time_provider(
    const unsigned int num_rows,
    const unsigned int num_cols
):
    rowsCnt(num_rows),
    colsCnt(num_cols),
    distances(std::make_unique<IntType[]>(static_cast<size_t>(num_rows) * num_cols)) {
}

//______________________________________________________________________________________________________________________
template<class IntType> Distance_matrix_travel_time_provider<IntType>::Distance_matrix_travel_time_provider(
        std::unique_ptr<IntType[]> distMatrix,
        const unsigned int num_rows,
        const unsigned int num_cols
):
    rowsCnt(num_rows),
    colsCnt(num_cols),
    distances(std::move(distMatrix)) {
}

//______________________________________________________________________________________________________________________
template<class IntType> Distance_matrix_travel_time_provider<IntType>::Distance_matrix_travel_time_provider(bool fast, GraphLoader& graphLoader, int scaling_factor):
    rowsCnt(graphLoader.nodes()),
    colsCnt(graphLoader.nodes()) // Square matrix
{
    computeDistanceMatrix(fast, graphLoader, scaling_factor, "Distance Matrix preprocessing");
}

//______________________________________________________________________________________________________________________
template<class IntType> dist_t Distance_matrix_travel_time_provider<IntType>::findDistance(const unsigned int start_row, const unsigned int goal_col) const {
    if (start_row >= rowsCnt || goal_col >= colsCnt) {
        throw std::out_of_range("Requested distance is out of bounds of the distance matrix.");
    }
    return distances[static_cast<size_t>(start_row) * colsCnt + goal_col];
}

//______________________________________________________________________________________________________________________
template<class IntType> void Distance_matrix_travel_time_provider<IntType>::setDistance(unsigned int source_row, unsigned int target_col, dist_t distance) {
     if (source_row >= rowsCnt || target_col >= colsCnt) {
        // Handle out-of-bounds access, e.g., throw error or log
        return;
    }
    distances[static_cast<size_t>(source_row) * colsCnt + target_col] = boost::numeric_cast<IntType>(distance);
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
    if (colsCnt == 0) { // Avoid division by zero if matrix is empty or has no columns
        std::cout << "Distance matrix is empty or has 0 columns. No info to print." << std::endl;
        return;
    }
    for(unsigned int i = 0; i < rowsCnt; i++) {
        for(unsigned int j = 0; j < colsCnt; j++) {
            if (distances[static_cast<size_t>(i) * colsCnt + j] == static_cast<IntType>(max)) { // Cast max to IntType for comparison
                maxCnt++;
            }
            // Be careful with comparing IntType to half of dist_t if IntType is smaller.
            // This comparison might need adjustment based on IntType's range.
            // For simplicity, we assume IntType can represent values comparable to dist_t/2 or dist_t for this info.
            if (static_cast<dist_t>(distances[static_cast<size_t>(i) * colsCnt + j]) >= half) { 
                halfCnt++;
            }
        }
    }

    const auto optCount = static_cast<size_t>(rowsCnt) * colsCnt;
    if (optCount == 0) {
         std::cout << "Distance matrix is empty. No info to print." << std::endl;
        return;
    }
    std::cout << "Computed distance matrix info." << std::endl;
    std::cout << "Distance matrix contains " << maxCnt << " UINF values. That is " << (double) maxCnt / (double) optCount * 100.0 << " %." << std::endl;
    std::cout << "Distance matrix contains " << halfCnt << " values that are at least half of std::numeric_limits<dist_t>::max(). That is "
        << (double) halfCnt / (double) optCount * 100.0 << " %." << std::endl;
}

template<class IntType> const std::unique_ptr<IntType[]> &Distance_matrix_travel_time_provider<IntType>::getRawData() {
    return distances;
}

template<class IntType> unsigned int Distance_matrix_travel_time_provider<IntType>::getRows() const {
    return rowsCnt;
}

template<class IntType> unsigned int Distance_matrix_travel_time_provider<IntType>::getCols() const {
    return colsCnt;
}

//______________________________________________________________________________________________________________________
// Constructor for a square empty matrix
template<class IntType> Distance_matrix_travel_time_provider<IntType>::Distance_matrix_travel_time_provider(const unsigned int nodes):
    Distance_matrix_travel_time_provider(nodes, nodes) { // Calls the (rows, cols) constructor
}

//______________________________________________________________________________________________________________________
// Constructor from a pre-filled 1D array (for square matrices)
template<class IntType> Distance_matrix_travel_time_provider<IntType>::Distance_matrix_travel_time_provider(
        std::unique_ptr<IntType[]> distMatrix,
        const unsigned int size
):
    Distance_matrix_travel_time_provider(std::move(distMatrix), size, size) { // Calls the (distMatrix, rows, cols) constructor
}

//______________________________________________________________________________________________________________________
template<class IntType> unsigned int Distance_matrix_travel_time_provider<IntType>::nodes() const {
    if (rowsCnt != colsCnt) {
        throw std::runtime_error("nodes() method called on a non-square Distance_matrix_travel_time_provider.");
    }
    return rowsCnt; // or colsCnt, as they are equal
}
