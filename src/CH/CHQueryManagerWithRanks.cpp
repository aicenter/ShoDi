//
// Author: Xenty (Michal Cvach)
// Created on: 23.8.18
//

#include "CHQueryManagerWithRanks.h"

#include <queue>
#include <climits>
#include <cstdio>
#include "CHQueryManager.h"
#include "../Dijkstra/DijkstraNode.h"
#include "Structures/QueryPriorityQueue.h"

//______________________________________________________________________________________________________________________
CHQueryManagerWithRanks::CHQueryManagerWithRanks(vector<unsigned int> & x) : ranks(x) {

}


//______________________________________________________________________________________________________________________
long long unsigned int CHQueryManagerWithRanks::findDistance(const unsigned int source, const unsigned int target, const Graph & graph) {
    unsigned int n = graph.nodes();

    //auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    //priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> fromQueue(cmp);
    //priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> toQueue(cmp);
    QueryPriorityQueue fromQueue;
    QueryPriorityQueue toQueue;

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

    fromQueue.push(source, 0);
    toQueue.push(target, 0);

    long long unsigned int shortestFound = ULLONG_MAX;
    while (! fromQueue.empty() || ! toQueue.empty()) {
        //printf("From queue size: %lu, to queue size: %lu\n", fromQueue.size(), toQueue.size());

        if (! fromQueue.empty()) {
            unsigned int current = fromQueue.top().ID;
            long long unsigned int currentDist = fromQueue.top().weight;
            fromClosed[current] = true;

            if( currentDist < shortestFound ) {
                if (toClosed[current]) {
                    if (currentDist + toDistance[current] < shortestFound) {
                        shortestFound = currentDist + toDistance[current];
                    }
                }

                const vector < pair < unsigned int, long long unsigned int > > & following = graph.outgoingEdges(current);
                for (unsigned int i = 0; i < following.size(); i++) {
                    if (ranks[following.at(i).first] > ranks[current]) {
                        long long unsigned int newDist = currentDist + following.at(i).second;

                        if (newDist < fromDistance[following.at(i).first]) {
                            if (fromDistance[following.at(i).first == ULLONG_MAX]) {
                                fromQueue.push(following.at(i).first, newDist);
                            } else {
                                fromQueue.decreaseKey(following.at(i).first, newDist);
                            }
                            fromDistance[following.at(i).first] = newDist;
                        }
                    }
                }

                fromQueue.pop();
            } else {
                while (! fromQueue.empty()) {
                    fromQueue.pop();
                }
            }
        }

        if (! toQueue.empty()) {
            unsigned int current = toQueue.top().ID;
            long long unsigned int currentDist = toQueue.top().weight;
            toClosed[current] = true;

            if( currentDist < shortestFound ) {
                if (fromClosed[current]) {
                    if (currentDist + fromDistance[current] < shortestFound) {
                        shortestFound = currentDist + fromDistance[current];
                    }
                }

                const vector < pair < unsigned int, long long unsigned int > > & previous = graph.incomingEdges(current);
                for (unsigned int i = 0; i < previous.size(); i++) {
                    if (ranks[previous.at(i).first] > ranks[current]) {
                        long long unsigned int newDist = currentDist + previous.at(i).second;

                        if (newDist < toDistance[previous.at(i).first]) {
                            if (toDistance[previous.at(i).first] == ULLONG_MAX) {
                                toQueue.push(previous.at(i).first, newDist);
                            } else {
                                toQueue.decreaseKey(previous.at(i).first, newDist);
                            }
                            toDistance[previous.at(i).first] = newDist;
                        }
                    }
                }

                toQueue.pop();
            } else {
                while (! toQueue.empty()) {
                    toQueue.pop();
                }
            }
        }

    }

    delete [] fromDistance;
    delete [] toDistance;
    delete [] fromClosed;
    delete [] toClosed;
    return shortestFound;
}
