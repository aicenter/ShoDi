//
// Author: Xenty (Michal Cvach)
// Created on: 06.12.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRAFBENCHMARK_H
#define CONTRACTION_HIERARCHIES_TNRAFBENCHMARK_H

#include "../GraphBuilding/Structures/TransitNodeRoutingArcFlagsGraph.h"



/**
 * Allows the user to benchmark the Transit Node Routing with Arc Flags using a given set of queries (trips).
 */
class TNRAFBenchmark {
public:
    /**
     * Runs the Transit Node Routing with Arc Flags query algorithm on a set set of queries (trips).
     * Returns the cumulative time the queries took in seconds. Additionally stores the returned distances
     * into a std::vector so that their correctness can be later verified.
     *
     * @param trips[in] The set of queries (trips) that will be used for the benchmark.
     * @param graph[in] The Transit Node Routing data structure that will be used for the benchmark.
     * @param distances[out] The std::vector that the results of the queries will be saved into. The values in the std::vector
     * can then be used to verify that the returned distances were correct.
     * @return Returns the cumulative time required to answer all the queries in seconds.
     */
    static double benchmark(
            const std::vector < std::pair< unsigned int, unsigned int> > & trips,
            TransitNodeRoutingArcFlagsGraph & graph,
            std::vector < unsigned int > & distances);

    /**
     * Runs the Transit Node Routing with Arc Flags query algorithm on a set set of queries (trips).
     * Returns the cumulative time the queries took in seconds. Additionally stores the returned distances
     * into a std::vector so that their correctness can be later verified. This variants allows us to query using
     * the IDs in the original graph, because we can provide a mapping from the original IDs to IDs used
     * internally in the query algorithms.
     *
     * @param trips[in] The set of queries (trips) that will be used for the benchmark.
     * @param graph[in] The Transit Node Routing data structure that will be used for the benchmark.
     * @param distances[out] The std::vector that the results of the queries will be saved into. The values in the std::vector
     * can then be used to verify that the returned distances were correct.
     * @param mappingFilePath[in] The path to the mapping file.
     * @return Returns the cumulative time required to answer all the queries in seconds.
     */
    static double benchmarkWithMapping(
            const std::vector < std::pair< long long unsigned int, long long unsigned int> > & trips,
            TransitNodeRoutingArcFlagsGraph & graph,
            std::vector < unsigned int > & distances,
            std::string mappingFilePath);
};


#endif //CONTRACTION_HIERARCHIES_TNRAFBENCHMARK_H
