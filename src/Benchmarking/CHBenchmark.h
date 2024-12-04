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
// Created on: 23.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H
#define TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H

#include "../GraphBuilding/Structures/FlagsGraph.h"
#include "../GraphBuilding/Structures/Graph.h"

/**
 * This class is responsible for running a CH Benchmark and outputing the time. The only method
 * 'runAndMeasureFlagsGraphOutputAndRetval()' just takes a std::vector of trips as an argument and computes shortest
 * distance for all of them, while also computing the time it spent doing it.
 */
class CHBenchmark {
public:
    /**
     * Runs the given set of queries, records the time required for those queries, and puts the results
     * inside the provided std::vector. Returns the time it took to answer all queries in seconds.
     * The distances are not validated inside this function, user is advised to validate that the results
     * are correct somewhere outside this function.
     *
     * @param trips[in] The set of queries (trips) that will be used for the benchmark.
     * @param graph[in] The Contraction Hierarchies data structure that will be used for the benchmark.
     * @param distances[out] The std::vector that the results of the queries will be saved into. The values in the std::vector
     * can then be used to verify that the returned distances were correct.
     * @return Returns the cumulative time required to answer all the queries in seconds.
     */
    static double benchmark(
            const std::vector < std::pair< unsigned int, unsigned int> > & trips,
            FlagsGraph<NodeData>& graph,
            std::vector < unsigned int > & distances);

    /**
     * Runs the given set of queries, records the time required for those queries, and puts the results
     * inside the provided std::vector. Returns the time it took to answer all queries in seconds.
     * The distances are not validated inside this function, user is advised to validate that the results
     * are correct somewhere outside this function. This variants allows us to query using the IDs in the original
     * graph, because we can provide a mapping from the original IDs to IDs used internally in the query algorithms.
     *
     * @param trips[in] The set of queries (trips) that will be used for the benchmark.
     * @param graph[in] The Contraction Hierarchies structure that will be used for the benchmark.
     * @param distances[out] The std::vector that the results of the queries will be saved into. The values in the std::vector
     * can then be used to verify that the returned distances were correct.
     * @param mappingFilePath[in] The path to the mapping file.
     * @return Returns the cumulative time required to answer all the queries in seconds.
     */
    static double benchmarkUsingMapping(
            const std::vector < std::pair< long long unsigned int, long long unsigned int> > & trips,
            FlagsGraph<NodeData> & graph,
            std::vector < unsigned int > & distances,
            std::string mappingFilePath);
};


#endif //TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H
