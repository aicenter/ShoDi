//
// Author: horakj31 (Jan Horak)
// Created on: 25.07.24
//

#ifndef ASTAR_H
#define ASTAR_H

#include "../GraphBuilding/Structures/Graph.h"
#include "AstarNode.h"

/**
 * A simple implementation of A* algorithm, which we mainly use as a baseline to compare the other
 * methods with.
 */
class Astar {
public:
    /**
     * A simple A* implementation that finds the shortest distance from start to goal.
     *
     * @param start[in] The start node for the query.
     * @param goal[in] The goal node for the query.
     * @param graph[in] The graph we want to find the shortest distance in.
     * @param locations[in] List of pairs of (easting, northing) values for A* heuristics.
     * @return Returns the shortest distance from start to goal or 'UINT_MAX' if goal is not reachable from start.
     */
    static unsigned int run(
            unsigned int start,
            unsigned int goal,
            const Graph &graph,
            const std::vector<std::pair<double, double>> &locations);

private:
    static double heuristic(std::pair<double, double> node_location, std::pair<double, double> goal_location);
};

#endif //ASTAR_H
