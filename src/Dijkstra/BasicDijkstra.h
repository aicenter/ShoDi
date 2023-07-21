//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_BASICDIJKSTRA_H
#define TRANSIT_NODE_ROUTING_BASICDIJKSTRA_H


#include "../GraphBuilding/Structures/Graph.h"

/**
 * A simple implementation of a basic Dijkstra's algorithm, which we mainly use as a baseline to compare the other
 * methods with, but it is also used during the preprocessing phases of some of the more complex methods.
 */
class BasicDijkstra {
public:
    /**
     * A simple Dijkstra implementation that finds the shortest distance from start to goal.
     * This should be used with the original graph.
     *
     * @param start[in] The start node for the query.
     * @param goal[in] The goal node for the query.
     * @param graph[in] The graph we want to find the shortest distance in.
     * @return Returns the shortest distance from start to goal or 'UINT_MAX' if goal is not reachable from start.
     */
    static unsigned int run(
            const unsigned int start,
            const unsigned int goal,
            const Graph & graph);

    /**
     * Finds the shortest path from start to goal in the Graph and also outputs the actual path.
     *
     * @param start[in] The start node for the query.
     * @param goal[in] The goal node for the query.
     * @param graph[in] The graph we want to find the shortest distance in.
     * @return Returns the shortest distance from start to goal or 'UINT_MAX' if goal is not reachable from start.
     */
    static unsigned int runWithPathOutput(
            const unsigned int start,
            const unsigned int goal,
            const Graph & graph);

    /**
     * Compute distances from source to all nodes in the graph. Fills those distances into the parameter
     * std::vector distances. Can be used for example to fill the full distance matrix - each call of this function
     * will compute one row of the matrix. This is also used in some of the preprocessing functions of some of the
     * methods.
     *
     * @param source[in] The node we want to start the search from.
     * @param graph[in] The graph we are interested in.
     * @param distances[out] A std::vector that will be filled with shortest distances from 'source' to all nodes.
     * The std::vector must be resized before this function is called to be able to contain values for all nodes.
     */
    static void computeOneToAllDistances(
            const unsigned int source,
            const Graph & graph,
            std::vector<unsigned int> & distances);

    /**
     * Compute distances from source to all nodes in the reversed graph. This is useful for example when computing
     * access access nodes for the backward direction.
     *
     * @param source[in] The node we want to start the search from.
     * @param graph[in] The graph we are interested in.
     * @param distances[out] A std::vector that will be filled with shortest distances from 'source' to all nodes in the
     * reversed graph. The std::vector must be resized before this function is called to be able to contain values for
     * all nodes.
     */
    static void computeOneToAllDistancesInReversedGraph(
            const unsigned int source,
            const Graph & graph,
            std::vector<unsigned int> & distances);

private:
    /**
     * Auxiliary function used for the path output. This function reconstructs the path and then outputs it in a simple
     * human readable text format. This could be useful for manual debug on small graphs.
     *
     * @param x[in] The currently processed node.
     * @param dist[in] distances from the start node of the query to all the nodes expanded during the query.
     * @param prev[in] A std::vector containing the predecessors for each node.
     */
    static void outputPath(
            const unsigned int x,
            const unsigned int * dist,
            const std::vector < std::vector < unsigned int > > & prev);
};


#endif //TRANSIT_NODE_ROUTING_BASICDIJKSTRA_H
