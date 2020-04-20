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
 * 'runAndMeasureFlagsGraphOutputAndRetval()' just takes a vector of trips as an argument and computes shortest
 * distance for all of them, while also computing the time it spent doing it.
 */
class CHBenchmark {
public:
    /**
     * Runs the given set of queries, records the time required for those queries, and puts the results
     * inside the provided vector. Returns the time it took to answer all queries in seconds.
     * The distances are not validated inside this function, user is advised to validate that the results
     * are correct somewhere outside this function.
     *
     * @param trips[in] The set of queries (trips) that will be used for the benchmark.
     * @param graph[in] The Contraction Hierarchies data structure that will be used for the benchmark.
     * @param distances[out] The vector that the results of the queries will be saved into. The values in the vector
     * can then be used to verify that the returned distances were correct.
     * @return Returns the cumulative time required to answer all the queries in seconds.
     */
    static double benchmark(
            const vector < pair < unsigned int, unsigned int> > & trips,
            FlagsGraph & graph,
            vector < unsigned int > & distances);

    /**
     * Runs the given set of queries, records the time required for those queries, and puts the results
     * inside the provided vector. Returns the time it took to answer all queries in seconds.
     * The distances are not validated inside this function, user is advised to validate that the results
     * are correct somewhere outside this function. This variants allows us to query using the IDs in the original
     * graph, because we can provide a mapping from the original IDs to IDs used internally in the query algorithms.
     *
     * @param trips[in] The set of queries (trips) that will be used for the benchmark.
     * @param graph[in] The Contraction Hierarchies structure that will be used for the benchmark.
     * @param distances[out] The vector that the results of the queries will be saved into. The values in the vector
     * can then be used to verify that the returned distances were correct.
     * @param mappingFilePath[in] The path to the mapping file.
     * @return Returns the cumulative time required to answer all the queries in seconds.
     */
    static double benchmarkUsingMapping(
            const vector < pair < long long unsigned int, long long unsigned int> > & trips,
            FlagsGraph & graph,
            vector < unsigned int > & distances,
            string mappingFilePath);
};


#endif //TRANSIT_NODE_ROUTING_CHBENCHMARKWITHRANKS_H
