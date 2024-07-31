//
// Author: horakj31 (Jan Horak)
// Created on: 25.07.24
//

#include <climits>
#include <queue>
#include <cstdio>
#include <unordered_map>
#include <cmath>
#include "Astar.h"
#include "AstarNode.h"

double Astar::heuristic(std::pair<double, double> node_location, std::pair<double, double> goal_location) {
    auto dx = goal_location.first - node_location.first;
    auto dy = goal_location.second - node_location.second;
    return sqrt(dx*dx + dy*dy);
}

//______________________________________________________________________________________________________________________
unsigned int Astar::run(const unsigned int start,
                        const unsigned int goal,
                        const Graph & graph,
                        const std::vector<std::pair<double, double>>& locations) {
    unsigned int n = graph.nodes();

    auto cmp = [](AstarNode left, AstarNode right) { return (left.weight) > (right.weight);};
    std::priority_queue<AstarNode, std::vector<AstarNode>, decltype(cmp)> q(cmp);

    auto *g_cost = new double[n];
    auto *f_cost = new double[n];
    auto *visited = new bool[n];
    for (size_t i = 0; i < n; i++) {
        g_cost[i] = UINT_MAX;
        f_cost[i] = UINT_MAX;
        visited[i] = false;
    }

    // Initialize start node
    g_cost[start] = 0;
    f_cost[start] = heuristic(locations[start], locations[goal]);
    q.emplace(start, f_cost[start]);

    while (!q.empty()) {
        unsigned int current_node = q.top().ID;
        q.pop();

        // Goal check
        if (current_node == goal) {
            auto ret = static_cast<unsigned int>(g_cost[goal]);

            delete [] g_cost;
            delete [] f_cost;
            delete [] visited;

            return ret;
        }

        // Mark the node as visited
        if (visited[current_node]) {
            continue;
        }
        visited[current_node] = true;

        // Process neighbors
        for (const auto& neighbor : graph.outgoingEdges(current_node)) {
            unsigned int neighbor_node = neighbor.first;
            double weight = neighbor.second;

            if (visited[neighbor_node]) {
                continue;
            }

            double tentative_g_cost = g_cost[current_node] + weight;
            if (g_cost[neighbor_node] == g_cost[n] || tentative_g_cost < g_cost[neighbor_node]) {
                g_cost[neighbor_node] = tentative_g_cost;
                double h = heuristic(locations[neighbor_node], locations[goal]);
                f_cost[neighbor_node] = tentative_g_cost + h;
                q.emplace(neighbor_node, f_cost[neighbor_node]);
            }
        }
    }

    delete [] g_cost;
    delete [] f_cost;
    delete [] visited;

    return UINT_MAX;
}
