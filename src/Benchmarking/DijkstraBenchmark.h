//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H
#define TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H

#include <vector>
#include "../GraphBuilding/Structures/Graph.h"

using namespace std;

/**
 * A class that allows us to run Dijkstra's Algorithm on a vector of trips. The time of the queries is recorded
// and returned.
 */
class DijkstraBenchmark {
public:
    /**
     * Runs the basic Dijkstra's Algorithm on a set set of queries (trips).
     * Returns the cumulative time the queries took in seconds. Additionally stores the returned distances
     * into a vector so that their correctness can be later verified.
     *
     * @param trips[in] The set of queries (trips) that will be used for the benchmark.
     * @param graph[in] The graph that will be used for the benchmark.
     * @param distances[out] The vector that the results of the queries will be saved into. The values in the vector
     * can then be used to verify that the returned distances were correct.
     * @return Returns the cumulative time required to answer all the queries in seconds.
     */
    static double benchmark(
            const vector < pair < unsigned int, unsigned int> > & trips,
            const Graph & graph,
            vector < unsigned int > & distances);

    /**
     * Runs the basic Dijkstra's Algorithm on a set set of queries (trips).
     * Returns the cumulative time the queries took in seconds. Additionally stores the returned distances
     * into a vector so that their correctness can be later verified. This variants allows us to query using
     * the IDs in the original graph, because we can provide a mapping from the original IDs to IDs used
     * internally by this application.
     *
     * @param trips[in] The set of queries (trips) that will be used for the benchmark.
     * @param graph[in] The graph that will be used for the benchmark.
     * @param distances[out] The vector that the results of the queries will be saved into. The values in the vector
     * can then be used to verify that the returned distances were correct.
     * @param mappingFilePath[in] The path to the mapping file.
     * @return Returns the cumulative time required to answer all the queries in seconds.
     */
    static double benchmarkUsingMapping(
            const vector < pair < long long unsigned int, long long unsigned int> > & trips,
            const Graph & graph,
            vector < unsigned int > & distances,
            string mappingFilePath);
};


#endif //TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H
