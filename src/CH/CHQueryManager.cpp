//
// Author: Xenty (Michal Cvach)
// Created on: 6.8.18
//

#include <queue>
#include <climits>
#include <cstdio>
#include <unordered_set>
#include "CHQueryManager.h"
#include "../Dijkstra/DijkstraNode.h"

// We use the query algorithm that was described in the "Contraction Hierarchies: Faster and Simpler Hierarchical
// Routing in Road Networks" article by Robert Geisberger, Peter Sanders, Dominik Schultes, and Daniel Delling.
// Basically, the query is a modified bidirectional Dijkstra query, where from the source node we only expand following
// nodes with higher contraction rank than the current node and from the target we only expand previous nodes with
// higher contraction rank than the current node. Both scopes will eventually meet in the node with the highest
// contraction rank from all nodes in the path.
//______________________________________________________________________________________________________________________
long long unsigned int CHQueryManager::findDistance(const unsigned int source, const unsigned int target, const Graph & graph) {
    unsigned int n = graph.nodes();

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> fromQueue(cmp);
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> toQueue(cmp);
    unordered_set<unsigned int> forwardStalled;
    unordered_set<unsigned int> backwardStalled;

    long long unsigned int * fromDistance = new long long unsigned int[n];
    long long unsigned int * toDistance = new long long unsigned int[n];
    bool * fromClosed = new bool[n];
    bool * toClosed = new bool[n];

    for(unsigned int i = 0; i < n; i++) {
        fromDistance[i] = ULLONG_MAX;
        toDistance[i] = ULLONG_MAX;
        fromClosed[i] = false;
        toClosed[i] = false;
    }

    fromDistance[source] = 0;
    toDistance[target] = 0;

    fromQueue.push(DijkstraNode(source, 0));
    toQueue.push(DijkstraNode(target, 0));

    long long unsigned int shortestFound = ULLONG_MAX;
    while (! fromQueue.empty() || ! toQueue.empty()) {
        //printf("From queue size: %lu, to queue size: %lu\n", fromQueue.size(), toQueue.size());

        if (! fromQueue.empty()) {
            unsigned int current = fromQueue.top().ID;
            long long unsigned int currentDist = fromQueue.top().weight;
            fromQueue.pop();
            if (forwardStalled.count(current) == 1) {
                continue;
            }

            if( currentDist < shortestFound ) {
                if (toClosed[current]) {
                    if (currentDist + toDistance[current] < shortestFound) {
                        shortestFound = currentDist + toDistance[current];
                    }
                }

                const vector < pair < unsigned int, long long unsigned int > > & following = graph.outgoingEdges(current);
                for (unsigned int i = 0; i < following.size(); i++) {
                        long long unsigned int newDist = currentDist + following.at(i).second;

                        if (newDist < fromDistance[following.at(i).first]) {
                            fromDistance[following.at(i).first] = newDist;
                            fromQueue.push(DijkstraNode(following.at(i).first, newDist));
                            forwardStalled.erase(following.at(i).first);
                        }
                        long long unsigned int complen = existsBackwardEdge(following.at(i).first, current, graph);
                        if ( complen != ULLONG_MAX && fromDistance[current] + complen < fromDistance[following.at(i).first] ) {
                            forwardStall(current, fromDistance[current] + complen, forwardStalled, graph, fromDistance);
                            break;
                        }
                }
            } else {
                while (! fromQueue.empty()) {
                    fromQueue.pop();
                }
            }
            fromClosed[current] = true;
        }

        if (! toQueue.empty()) {
            unsigned int current = toQueue.top().ID;
            long long unsigned int currentDist = toQueue.top().weight;
            toQueue.pop();
            if (backwardStalled.count(current) == 1) {
                continue;
            }

            if( currentDist < shortestFound ) {
                if (fromClosed[current]) {
                    if (currentDist + fromDistance[current] < shortestFound) {
                        shortestFound = currentDist + fromDistance[current];
                    }
                }

                const vector < pair < unsigned int, long long unsigned int > > & previous = graph.incomingEdges(current);
                for (unsigned int i = 0; i < previous.size(); i++) {
                        long long unsigned int newDist = currentDist + previous.at(i).second;

                        if (newDist < toDistance[previous.at(i).first]) {
                            toDistance[previous.at(i).first] = newDist;
                            toQueue.push(DijkstraNode(previous.at(i).first, newDist));
                        }
                        long long unsigned int complen = existsForwardEdge(previous.at(i).first, current, graph);
                        if ( complen != ULLONG_MAX && toDistance[current] + complen < toDistance[previous.at(i).first] ) {
                            backwardStall(current, toDistance[current] + complen, backwardStalled, graph, toDistance);
                            break;
                        }
                }


            } else {
                while (! toQueue.empty()) {
                    toQueue.pop();
                }
            }
            toClosed[current] = true;
        }


    }

    delete [] fromDistance;
    delete [] toDistance;
    delete [] fromClosed;
    delete [] toClosed;
    return shortestFound;
}

//______________________________________________________________________________________________________________________
long long unsigned int CHQueryManager::existsBackwardEdge(const unsigned int x, const unsigned int y, const Graph & graph) {
    const vector < pair < unsigned int, long long unsigned int > > & neighbours = graph.incomingEdges(x);
    for(unsigned int i = 0; i < neighbours.size(); i++) {
        if(y == neighbours.at(i).first) {
            return neighbours.at(i).second;
        }
    }
    return ULLONG_MAX;
}

//______________________________________________________________________________________________________________________
long long unsigned int CHQueryManager::existsForwardEdge(const unsigned int x, const unsigned int y, const Graph & graph) {
    const vector < pair < unsigned int, long long unsigned int > > & neighbours = graph.outgoingEdges(x);
    for(unsigned int i = 0; i < neighbours.size(); i++) {
        if(y == neighbours.at(i).first) {
            return neighbours.at(i).second;
        }
    }
    return ULLONG_MAX;
}

//______________________________________________________________________________________________________________________
void CHQueryManager::forwardStall(const unsigned int node, const long long unsigned int weight, unordered_set<unsigned int> & forwardStalled, const Graph & graph, const long long unsigned int * fromDistance) {
    queue<pair<unsigned int, long long unsigned int> > bfs_q;
    bfs_q.push(make_pair(node, weight));
    forwardStalled.insert(node);

    while(! bfs_q.empty()) {
        unsigned int curnode = bfs_q.front().first;
        long long unsigned int curweight = bfs_q.front().second;
        bfs_q.pop();

        const vector < pair < unsigned int, long long unsigned int > > & neighbours = graph.outgoingEdges(curnode);
        for(unsigned int i = 0; i < neighbours.size(); i++) {
            unsigned int neighnode = neighbours.at(i).first;
            if (forwardStalled.count(neighnode) == 1) {
                continue;
            }

            long long unsigned int opositeEdge = existsBackwardEdge(neighnode, curnode, graph);
            if (opositeEdge == ULLONG_MAX) {
                continue;
            }
            unsigned int neighweight = opositeEdge + curweight;
            if (neighweight < fromDistance[neighnode]) {
                forwardStalled.insert(neighnode);
                bfs_q.push(make_pair(neighnode, neighweight));
            }
        }

    }
}

//______________________________________________________________________________________________________________________
void CHQueryManager::backwardStall(const unsigned int node, const long long unsigned int weight, unordered_set<unsigned int> & backwardStalled, const Graph & graph, const long long unsigned int * toDistance) {
    queue<pair<unsigned int, long long unsigned int> > bfs_q;
    bfs_q.push(make_pair(node, weight));
    backwardStalled.insert(node);

    while(! bfs_q.empty()) {
        unsigned int curnode = bfs_q.front().first;
        long long unsigned int curweight = bfs_q.front().second;
        bfs_q.pop();

        const vector < pair < unsigned int, long long unsigned int > > & neighbours = graph.incomingEdges(curnode);
        for(unsigned int i = 0; i < neighbours.size(); i++) {
            unsigned int neighnode = neighbours.at(i).first;
            if (backwardStalled.count(neighnode) == 1) {
                continue;
            }

            long long unsigned int opositeEdge = existsForwardEdge(neighnode, curnode, graph);
            if (opositeEdge == ULLONG_MAX) {
                continue;
            }
            unsigned int neighweight = opositeEdge + curweight;
            if (neighweight < toDistance[neighnode]) {
                backwardStalled.insert(neighnode);
                bfs_q.push(make_pair(neighnode, neighweight));
            }
        }

    }
}