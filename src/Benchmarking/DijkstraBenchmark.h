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
     * Runs the basic Dijkstra's Algorithm on a set set of queries (trips). The time needed to answer all the queries
     * is printed.
     *
     * @param trips[in] The set of queries (trips) that will be used for the benchmark.
     * @param graph[in] The graph that will be used for the benchmark.
     */
    static void runAndMeasure(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph);

    /**
     * Runs the basic Dijkstra's Algorithm on a set set of queries (trips). The time needed to answer all the queries
     * is printed and the returned distances are saved into a vector so that their correctness can be later verified.
     *
     * @param trips[in] The set of queries (trips) that will be used for the benchmark.
     * @param graph[in] The graph that will be used for the benchmark.
     * @param distances[out] The vector that the results of the queries will be saved into. The values in the vector
     * can then be used to verify that the returned distances were correct.
     */
    static void runAndMeasureWithOutput(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < unsigned int > & distances);

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
    static double runAndMeasureOutputAndRetval(const vector < pair < unsigned int, unsigned int> > & trips, const Graph & graph, vector < unsigned int > & distances);
};


#endif //TRANSIT_NODE_ROUTING_DIJKSTRABENCHMARK_H
