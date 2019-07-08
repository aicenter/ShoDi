//
// Created by xenty on 8.7.19.
//

#include <climits>
#include <queue>
#include <cstdio>
#include <limits>
#include "BasicFPointDijkstra.h"
#include "FPointDijkstraNode.h"

//______________________________________________________________________________________________________________________
double BasicFPointDijkstra::run(const unsigned int source, const unsigned int target, const FPointGraph & graph) {
    unsigned int n = graph.nodes();
    double * distance = new double[n];

    for(unsigned int i = 0; i < n; i++) {
        distance[i] = numeric_limits<double>::max();
    }

    distance[source] = 0;

    auto cmp = [](FPointDijkstraNode left, FPointDijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<FPointDijkstraNode, vector<FPointDijkstraNode>, decltype(cmp)> q(cmp);
    q.push(FPointDijkstraNode(source, 0));

    while(! q.empty() ) {
        const FPointDijkstraNode current = q.top();

        if (current.ID == target) {
            delete [] distance;
            return current.weight;
        }

        const vector < pair < unsigned int, double > > & neighbours = graph.outgoingEdges(current.ID);
        for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
            double newDistance = current.weight + neighbours.at(i).second;
            if (newDistance < distance[neighbours.at(i).first]) {
                distance[neighbours.at(i).first] = newDistance;
                q.push(FPointDijkstraNode(neighbours.at(i).first, newDistance));
            }
        }

        q.pop();

    }

    delete [] distance;
    return numeric_limits<double>::max();

}

//______________________________________________________________________________________________________________________
double BasicFPointDijkstra::runWithPathOutput(const unsigned int source, const unsigned int target, const FPointGraph & graph) {
    unsigned int n = graph.nodes();
    double * distance = new double[n];
    vector < vector < unsigned int > > previous(n);

    for(unsigned int i = 0; i < n; i++) {
        distance[i] = numeric_limits<double>::max();;
    }

    distance[source] = 0;

    auto cmp = [](FPointDijkstraNode left, FPointDijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<FPointDijkstraNode, vector<FPointDijkstraNode>, decltype(cmp)> q(cmp);
    q.push(FPointDijkstraNode(source, 0));

    while(! q.empty() ) {
        const FPointDijkstraNode current = q.top();

        if (current.ID == target) {
            outputPath(current.ID, distance, previous);
            delete [] distance;
            return current.weight;
        }

        const vector < pair < unsigned int, double > > & neighbours = graph.outgoingEdges(current.ID);
        for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
            double newDistance = current.weight + neighbours.at(i).second;
            if (newDistance < distance[neighbours.at(i).first]) {
                distance[neighbours.at(i).first] = newDistance;
                q.push(FPointDijkstraNode(neighbours.at(i).first, newDistance));
                previous[neighbours.at(i).first].push_back(current.ID);
            }
        }

        q.pop();

    }

    delete [] distance;
    return numeric_limits<double>::max();

}

//______________________________________________________________________________________________________________________
void BasicFPointDijkstra::outputPath(const unsigned int x, double * dist, const vector < vector < unsigned int > > & prev) {
    vector<pair<pair<unsigned int, unsigned int>, double> > path;
    unsigned int current = x;
    while (prev[current].size() > 0) {
        double distance = dist[prev[current].at(0)];
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

    printf("~~~ Outputting path from %u to %u (distance %f) ~~~\n", path[path.size()-1].first.first, x, dist[x]);
    for(int i = path.size()-1; i >= 0; i--) {
        printf("%u -> %u (%f)\n", path[i].first.first, path[i].first.second, path[i].second);
    }
    printf("~~~ End of path ~~~\n");

}