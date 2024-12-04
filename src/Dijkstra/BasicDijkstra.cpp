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

    for(size_t i = 0; i < n; i++) {
        distance[i] = UINT_MAX;
    }

    distance[start] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(start, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        if (current.ID == goal) {
            delete [] distance;
            return current.weight;
        }

        const std::vector < std::pair< unsigned int, unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
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
    std::vector < std::vector < unsigned int > > previous(n);

    for(size_t i = 0; i < n; i++) {
        distance[i] = UINT_MAX;
    }

    distance[start] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(start, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        if (current.ID == goal) {
            outputPath(current.ID, distance, previous);
            delete [] distance;
            return current.weight;
        }

        const std::vector < std::pair< unsigned int, unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
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
void BasicDijkstra::outputPath(const unsigned int x, const unsigned int * dist, const std::vector < std::vector < unsigned int > > & prev) {
    std::vector<std::pair<std::pair<unsigned int, unsigned int>, unsigned int> > path;
    unsigned int current = x;
    while (prev[current].size() > 0) {
        unsigned int distance = dist[prev[current].at(0)];
        size_t pos = 0;
        for(size_t i = 1; i < prev[current].size(); i++) {
            if (dist[prev[current].at(i)] < distance) {
                distance = dist[prev[current].at(i)];
                pos = i;
            }
        }
        path.push_back(std::make_pair(std::make_pair(prev[current].at(pos), current), dist[current] - dist[prev[current].at(pos)]));
        current = prev[current].at(pos);
    }

    printf("~~~ Outputting path from %u to %u (distance %u) ~~~\n", path[path.size()-1].first.first, x, dist[x]);
    for(long long i = (long long) path.size()-1; i >= 0; i--) {
        printf("%u -> %u (%u)\n", path[(size_t) i].first.first, path[(size_t) i].first.second, path[(size_t) i].second);
    }
    printf("~~~ End of path ~~~\n");

}

//______________________________________________________________________________________________________________________
void BasicDijkstra::computeOneToAllDistances(const unsigned int source, const Graph & graph, std::vector<unsigned int> & distances) {
    size_t n = (size_t) graph.nodes();

    for(size_t i = 0; i < (size_t) n; i++) {
        distances[i] = UINT_MAX;
    }

    distances[source] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(source, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        const std::vector < std::pair< unsigned int, unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
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
void BasicDijkstra::computeOneToAllDistancesInReversedGraph(const unsigned int source, const Graph & graph, std::vector<unsigned int> & distances) {
    size_t n = (size_t) graph.nodes();

    for(size_t i = 0; i < (size_t) n; i++) {
        distances[i] = UINT_MAX;
    }

    distances[source] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(source, 0));

    while(! q.empty() ) {
        const DijkstraNode current = q.top();

        const std::vector < std::pair< unsigned int, unsigned int > > & neighbours = graph.incomingEdges(current.ID);
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
