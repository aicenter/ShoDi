//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#include <climits>
#include <queue>
#include "BasicDijkstra.h"
#include "DijkstraNode.h"

//______________________________________________________________________________________________________________________
long long unsigned int BasicDijkstra::run(const unsigned int source, const unsigned int target, const Graph & graph) {
    unsigned int n = graph.nodes();
    long long unsigned int * distance = new long long unsigned int[n];

    for(unsigned int i = 0; i < n; i++) {
        distance[i] = ULLONG_MAX;
    }

    distance[source] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) < (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(source, 0));

    while(! q.empty() ) {
        const DijkstraNode & current = q.top();

        if (current.ID == target) {
            return current.weight;
        }

        const vector < pair < unsigned int, unsigned int > > & neighbours = graph.neighnours(current.ID);
        for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
            long long unsigned int newDistance = current.weight + neighbours.at(i).second;
            if (newDistance < distance[neighbours.at(i).first]) {
                distance[neighbours.at(i).first] = newDistance;
                q.push(DijkstraNode(neighbours.at(i).first, newDistance));
            }
        }

        q.pop();

    }

    return ULLONG_MAX;

}