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
unsigned int BasicDijkstra::run(const unsigned int start, const unsigned int goal, const Graph & graph) {
    unsigned int n = graph.nodes();
    unsigned int * distance = new unsigned int[n];

    for(unsigned int i = 0; i < n; i++) {
        distance[i] = UINT_MAX;
    }

    distance[start] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(start, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        if (current.ID == goal) {
            delete [] distance;
            return current.weight;
        }

        const vector < pair < unsigned int, unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
        for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
            unsigned int newDistance = current.weight + neighbours.at(i).second;
            if (newDistance < distance[neighbours.at(i).first]) {
                distance[neighbours.at(i).first] = newDistance;
                q.push(DijkstraNode(neighbours.at(i).first, newDistance));
            }
        }

        q.pop();

    }

    delete [] distance;
    return UINT_MAX;

}

//______________________________________________________________________________________________________________________
unsigned int BasicDijkstra::runWithPathOutput(const unsigned int start, const unsigned int goal, const Graph & graph) {
    unsigned int n = graph.nodes();
    unsigned int * distance = new unsigned int[n];
    vector < vector < unsigned int > > previous(n);

    for(unsigned int i = 0; i < n; i++) {
        distance[i] = UINT_MAX;
    }

    distance[start] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(start, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        if (current.ID == goal) {
            outputPath(current.ID, distance, previous);
            delete [] distance;
            return current.weight;
        }

        const vector < pair < unsigned int, unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
        for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
            unsigned int newDistance = current.weight + neighbours.at(i).second;
            if (newDistance < distance[neighbours.at(i).first]) {
                distance[neighbours.at(i).first] = newDistance;
                q.push(DijkstraNode(neighbours.at(i).first, newDistance));
                previous[neighbours.at(i).first].push_back(current.ID);
            }
        }

        q.pop();

    }

    printf("Did not find path from %u to %u.\n", start, goal);
    delete [] distance;
    return UINT_MAX;



}

//______________________________________________________________________________________________________________________
void BasicDijkstra::outputPath(const unsigned int x, const unsigned int * dist, const vector < vector < unsigned int > > & prev) {
    vector<pair<pair<unsigned int, unsigned int>, unsigned int> > path;
    unsigned int current = x;
    while (prev[current].size() > 0) {
        unsigned int distance = dist[prev[current].at(0)];
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

    printf("~~~ Outputting path from %u to %u (distance %u) ~~~\n", path[path.size()-1].first.first, x, dist[x]);
    for(int i = path.size()-1; i >= 0; i--) {
        printf("%u -> %u (%u)\n", path[i].first.first, path[i].first.second, path[i].second);
    }
    printf("~~~ End of path ~~~\n");

}

//______________________________________________________________________________________________________________________
void BasicDijkstra::computeOneToAllDistances(const unsigned int source, const Graph & graph, vector<unsigned int> & distances) {
    unsigned int n = graph.nodes();

    for(unsigned int i = 0; i < n; i++) {
        distances[i] = UINT_MAX;
    }

    distances[source] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(source, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        const vector < pair < unsigned int, unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
        for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
            unsigned int newDistance = current.weight + neighbours.at(i).second;
            if (newDistance < distances[neighbours.at(i).first]) {
                distances[neighbours.at(i).first] = newDistance;
                q.push(DijkstraNode(neighbours.at(i).first, newDistance));
            }
        }

        q.pop();

    }
}

//______________________________________________________________________________________________________________________
void BasicDijkstra::computeOneToAllDistancesInReversedGraph(const unsigned int source, const Graph & graph, vector<unsigned int> & distances) {
    unsigned int n = graph.nodes();

    for(unsigned int i = 0; i < n; i++) {
        distances[i] = UINT_MAX;
    }

    distances[source] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(source, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        const vector < pair < unsigned int, unsigned int > > & neighbours = graph.incomingEdges(current.ID);
        for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
            unsigned int newDistance = current.weight + neighbours.at(i).second;
            if (newDistance < distances[neighbours.at(i).first]) {
                distances[neighbours.at(i).first] = newDistance;
                q.push(DijkstraNode(neighbours.at(i).first, newDistance));
            }
        }

        q.pop();

    }
}