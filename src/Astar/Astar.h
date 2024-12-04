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
