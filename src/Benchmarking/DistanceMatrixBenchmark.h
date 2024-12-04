/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#ifndef CONTRACTION_HIERARCHIES_DISTANCEMATRIXBENCHMARK_H
#define CONTRACTION_HIERARCHIES_DISTANCEMATRIXBENCHMARK_H

#include <string>
#include <utility>
#include <vector>
#include "../DistanceMatrix/DistanceMatrixInterface.h"
#include "../constants.h"



/**
 * Allows the user to benchmark the Distance Matrix approach with a given set of queries (trips).
 */
class DistanceMatrixBenchmark {
public:
    /**
     * Runs the given set of queries, records the time required for those queries, and puts the results inside the
     * provided std::vector. Returns the time it took to answer all queries in seconds. The distances are not validated
     * inside this function, user is advised to validate that the results are correct somewhere outside this function.
     *
     * @param trips[in] The set of queries (trips) that will be used for the benchmark.
     * @param matrix[in] The distance matrix that will be used for the benchmark.
     * @param distances[out] The std::vector that the results of the queries will be saved into. The values in the std::vector
     * can then be used to verify that the returned distances were correct.
     * @return Returns the cumulative time required to answer all the queries in seconds.
     */
    static double benchmark(
            const std::vector < std::pair< unsigned int, unsigned int> > & trips,
            DistanceMatrixInterface& matrix,
            std::vector < dist_t > & distances);
    
    /**
     * Runs the given set of queries, records the time required for those queries, and puts the results inside the
     * provided std::vector. Returns the time it took to answer all queries in seconds. The distances are not validated
     * inside this function, user is advised to validate that the results are correct somewhere outside this function. This variant
      allows us to query using the IDs in the original graph, because we can provide a mapping from the original IDs to IDs used
     * internally by this application.
     *
     * @param trips[in] The set of queries (trips) that will be used for the benchmark.
     * @param matrix[in] The distance matrix that will be used for the benchmark.
     * @param distances[out] The std::vector that the results of the queries will be saved into. The values in the std::vector
     * can then be used to verify that the returned distances were correct.
     * @param mappingFilePath[in] The path to the mapping file.
     * @return Returns the cumulative time required to answer all the queries in seconds.
     */
    static double benchmarkUsingMapping(
            const std::vector < std::pair< long long unsigned int, long long unsigned int> > & trips,
            DistanceMatrixInterface& matrix,
            std::vector < dist_t > & distances,
            std::string mappingFilePath);
};


#endif //CONTRACTION_HIERARCHIES_DISTANCEMATRIXBENCHMARK_H
