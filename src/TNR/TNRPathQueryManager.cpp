//
// Author: Xenty (Michal Cvach)
// Created on: 20.08.19
//

#include "TNRPathQueryManager.h"

//______________________________________________________________________________________________________________________
TNRPathQueryManager::TNRPathQueryManager(TransitNodeRoutingGraphForPathQueries & graph) : graph(graph), fallbackCHmanager(graph) {

}

//______________________________________________________________________________________________________________________
unsigned int TNRPathQueryManager::findDistance(const unsigned int start, const unsigned int goal) {
    unsigned int distance = 0;
    if(start == goal) {
        return 0;
    } else {
        if (graph.isLocalQuery(start, goal)) { // Is local query, fallback to some other distance manager, here CH
            return fallbackCHmanager.findDistanceOutputPath(start, goal);
        } else { // Not local query, TNR can be used.
            distance = graph.findTNRDistance(start, goal);
        }
    }

    //printf("Source: %u (rank %u), goal: %u (rank %u)\n", start, graph.data(start).rank, goal, graph.data(goal).rank);
    //printf("Rank of %u is %u, rank of %u is %u.\n", 1870, graph.data(1870).rank, 21257, graph.data(21257).rank);
    if (distance == UINT_MAX) {
        printf("Couldn't find path from start to goal. Nothing to output.\n");
        return UINT_MAX;
    }

    vector < pair < unsigned int, unsigned int > > additionalNextNodes = graph.unpackingNeighbours(start);
    for(size_t i = 0; i < additionalNextNodes.size(); i++) {
        //printf("Trying %u (rank %u) as middle node. Distances: %u and %u, that is: %u\n", additionalNextNodes[i].first, graph.data(additionalNextNodes[i].first).rank, additionalNextNodes[i].second, quickFindDistance(additionalNextNodes[i].first, goal), quickFindDistance(additionalNextNodes[i].first, goal) + unpackingNeighbours[i].second);
        if(distance - additionalNextNodes[i].second == quickFindDistance(additionalNextNodes[i].first, goal)) {
            //fallbackCHmanager.printEdgesForwardShortcut(start, additionalNextNodes[i].first);
            printf("%u -> %u (%u) - remainings: %u\n", start, additionalNextNodes[i].first, additionalNextNodes[i].second, distance - additionalNextNodes[i].second);
            findDistance(additionalNextNodes[i].first, goal);
            return distance;
        }
    }

    printf("Oops, something went wrong. Didn't find a valid next node when reconstructing the path.\n");
    return UINT_MAX;
}


//______________________________________________________________________________________________________________________
unsigned int TNRPathQueryManager::findPath(const unsigned int start, const unsigned int goal, vector<SimpleEdge> & path) {
    unsigned int distance = 0;
    if(start == goal) {
        return 0;
    } else {
        if (graph.isLocalQuery(start, goal)) { // Is local query, fallback to some other distance manager, here CH
            //return fallbackCHmanager.findPath(start, goal, path);
            distance = fallbackCHmanager.findDistanceOnly(start, goal);
        } else { // Not local query, TNR can be used.
            distance = graph.findTNRDistance(start, goal);
        }
    }

    //printf("Source: %u (rank %u), goal: %u (rank %u)\n", start, graph.data(start).rank, goal, graph.data(goal).rank);
    //printf("Rank of %u is %u, rank of %u is %u.\n", 1870, graph.data(1870).rank, 21257, graph.data(21257).rank);
    if (distance == UINT_MAX) {
        //printf("Couldn't find path from start to goal. Nothing to output.\n");
        return UINT_MAX;
    }

    vector < pair < unsigned int, unsigned int > > additionalNextNodes = graph.unpackingNeighbours(start);
    for(size_t i = 0; i < additionalNextNodes.size(); i++) {
        //printf("Trying %u (rank %u) as middle node. Distances: %u and %u, that is: %u\n", additionalNextNodes[i].first, graph.data(additionalNextNodes[i].first).rank, additionalNextNodes[i].second, quickFindDistance(additionalNextNodes[i].first, goal), quickFindDistance(additionalNextNodes[i].first, goal) + unpackingNeighbours[i].second);
        if(distance - additionalNextNodes[i].second == quickFindDistance(additionalNextNodes[i].first, goal)) {
            //fallbackCHmanager.printEdgesForwardShortcut(start, additionalNextNodes[i].first);
            //printf("%u -> %u (%u) - remainings: %u\n", start, additionalNextNodes[i].first, additionalNextNodes[i].second, distance - additionalNextNodes[i].second);
            path.push_back(SimpleEdge(start, additionalNextNodes[i].first));
            findPath(additionalNextNodes[i].first, goal, path);
            return distance;
        }
    }

    printf("Oops, something went wrong. Didn't find a valid next node when reconstructing the path.\n");
    return UINT_MAX;
}

//______________________________________________________________________________________________________________________
unsigned int TNRPathQueryManager::quickFindDistance(const unsigned int start, const unsigned int goal) {
    if (start == goal) {
        return 0;
    } else {
        if (graph.isLocalQuery(start, goal)) { // Is local query, fallback to some other distance manager, here CH
            return fallbackCHmanager.findDistanceOnly(start, goal);
        } else { // Not local query, TNR can be used.
            return graph.findTNRDistance(start, goal);
        }
    }
}
