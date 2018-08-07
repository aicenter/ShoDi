//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#include <climits>
#include <queue>
#include <cstdio>
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

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(source, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        if (current.ID == target) {
            delete [] distance;
            return current.weight;
        }

        const vector < pair < unsigned int, long long unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
        for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
            long long unsigned int newDistance = current.weight + neighbours.at(i).second;
            if (newDistance < distance[neighbours.at(i).first]) {
                distance[neighbours.at(i).first] = newDistance;
                q.push(DijkstraNode(neighbours.at(i).first, newDistance));
            }
        }

        q.pop();

    }

    delete [] distance;
    return ULLONG_MAX;

}

//______________________________________________________________________________________________________________________
long long unsigned int BasicDijkstra::runWithPathOutput(const unsigned int source, const unsigned int target, const Graph & graph) {
    unsigned int n = graph.nodes();
    long long unsigned int * distance = new long long unsigned int[n];
    vector < vector < unsigned int > > previous(n);

    for(unsigned int i = 0; i < n; i++) {
        distance[i] = ULLONG_MAX;
    }

    distance[source] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(source, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        if (current.ID == target) {
            outputPath(current.ID, distance, previous);
            delete [] distance;
            return current.weight;
        }

        const vector < pair < unsigned int, long long unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
        for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
            long long unsigned int newDistance = current.weight + neighbours.at(i).second;
            if (newDistance < distance[neighbours.at(i).first]) {
                distance[neighbours.at(i).first] = newDistance;
                q.push(DijkstraNode(neighbours.at(i).first, newDistance));
                previous[neighbours.at(i).first].push_back(current.ID);
            }
        }

        q.pop();

    }

    delete [] distance;
    return ULLONG_MAX;

}

//______________________________________________________________________________________________________________________
void BasicDijkstra::outputPath(const unsigned int x, const long long unsigned int * dist, const vector < vector < unsigned int > > & prev) {
    vector<pair<pair<unsigned int, unsigned int>, long long unsigned int> > path;
    unsigned int current = x;
    while (prev[current].size() > 0) {
        long long unsigned int distance = dist[prev[current].at(0)];
        unsigned int pos = 0;
        for (unsigned int i = 1; i < prev[current].size(); i++) {
            if (dist[prev[current].at(i)] < distance) {
                distance = dist[prev[current].at(i)];
                pos = i;
            }
        }
        path.push_back(make_pair(make_pair(prev[current].at(pos), current), dist[current] - dist[prev[current].at(pos)]));
        current = prev[current].at(pos);
    }

    printf("~~~ Outputting path from %u to %u (distance %llu) ~~~\n", path[path.size()-1].first.first, x, dist[x]);
    for(int i = path.size()-1; i >= 0; i--) {
        printf("%u -> %u (%llu)\n", path[i].first.first, path[i].first.second, path[i].second);
    }
    printf("~~~ End of path ~~~\n");

}