//
// Author: Xenty (Michal Cvach)
// Created on: 20.08.19
//

#include "TNRPathQueryManager.h"

//______________________________________________________________________________________________________________________
TNRPathQueryManager::TNRPathQueryManager(TransitNodeRoutingGraphForPathQueries & graph) : fallbackCHmanager(graph), graph(graph) {

}

// Actually finds the distance between two targets. If source != target, this function first invokes the locality
// filter to determine whether the query is local. If it is local, we fallback to the Contraction Hierarchies query
// algorithm. This is slightly slower, but the local queries already guarantee that the source and target nodes will
// be reasonably close to each other and in that case the query should take relatively short time. If the query is not
// local, Transit Node Routing structure can be used to determine the shortest distance, and in that case it is invoked
// to do so.
//______________________________________________________________________________________________________________________
unsigned int TNRPathQueryManager::findDistance(const unsigned int source, const unsigned int target) {
    unsigned int distance = 0;
    if(source == target) {
        return 0;
    } else {
        if (graph.isLocalQuery(source, target)) { // Is local query, fallback to some other distance manager, here CH
            return fallbackCHmanager.findDistanceOutputPath(source, target);
        } else { // Not local query, TNR can be used.
            distance = graph.findTNRDistance(source, target);
        }
    }

    //printf("Source: %u (rank %u), target: %u (rank %u)\n", source, graph.data(source).rank, target, graph.data(target).rank);
    //printf("Rank of %u is %u, rank of %u is %u.\n", 1870, graph.data(1870).rank, 21257, graph.data(21257).rank);
    if (distance == UINT_MAX) {
        printf("Couldn't find path from source to target. Nothing to output.\n");
        return UINT_MAX;
    }

    /*vector< QueryEdgeWithUnpackingData > nextNodes = graph.nextNodes(source);
    //printf("Current distance left: %u, we have %lu neighbours to try.\n", distance, nextNodes.size());
    for(unsigned int i = 0; i < nextNodes.size(); i++) {
        //printf("Trying %u (rank %u) as middle node. Distances: %llu and %u, that is: %llu\n", nextNodes[i].targetNode, graph.data(nextNodes[i].targetNode).rank, nextNodes[i].weight, quickFindDistance(nextNodes[i].targetNode, target), quickFindDistance(nextNodes[i].targetNode, target) + nextNodes[i].weight);
        if(nextNodes[i].forward && distance - nextNodes[i].weight == quickFindDistance(nextNodes[i].targetNode, target)) {
            fallbackCHmanager.printEdgesForwardShortcut(source, nextNodes[i].targetNode);
            printf("%u -> %u (%llu) - remainings: %llu\n", source, nextNodes[i].targetNode, nextNodes[i].weight, distance - nextNodes[i].weight);
            findDistance(nextNodes[i].targetNode, target);
            return distance;
        }
    }*/

    vector < pair < unsigned int, unsigned int > > additionalNextNodes = graph.unpackingNeighbours(source);
    for (unsigned int i = 0; i < additionalNextNodes.size(); i++) {
        //printf("Trying %u (rank %u) as middle node. Distances: %u and %u, that is: %u\n", additionalNextNodes[i].first, graph.data(additionalNextNodes[i].first).rank, additionalNextNodes[i].second, quickFindDistance(additionalNextNodes[i].first, target), quickFindDistance(additionalNextNodes[i].first, target) + unpackingNeighbours[i].second);
        if(distance - additionalNextNodes[i].second == quickFindDistance(additionalNextNodes[i].first, target)) {
            //fallbackCHmanager.printEdgesForwardShortcut(source, additionalNextNodes[i].first);
            printf("%u -> %u (%u) - remainings: %u\n", source, additionalNextNodes[i].first, additionalNextNodes[i].second, distance - additionalNextNodes[i].second);
            findDistance(additionalNextNodes[i].first, target);
            return distance;
        }
    }

    printf("Oops, something went wrong. Didn't find a valid next node when reconstructing the path.\n");
    return UINT_MAX;
}


//______________________________________________________________________________________________________________________
unsigned int TNRPathQueryManager::findPath(const unsigned int source, const unsigned int target, vector<SimpleEdge> & path) {
    unsigned int distance = 0;
    if(source == target) {
        return 0;
    } else {
        if (graph.isLocalQuery(source, target)) { // Is local query, fallback to some other distance manager, here CH
            //return fallbackCHmanager.findPath(source, target, path);
            distance = fallbackCHmanager.findDistanceOnly(source, target);
        } else { // Not local query, TNR can be used.
            distance = graph.findTNRDistance(source, target);
        }
    }

    //printf("Source: %u (rank %u), target: %u (rank %u)\n", source, graph.data(source).rank, target, graph.data(target).rank);
    //printf("Rank of %u is %u, rank of %u is %u.\n", 1870, graph.data(1870).rank, 21257, graph.data(21257).rank);
    if (distance == UINT_MAX) {
        //printf("Couldn't find path from source to target. Nothing to output.\n");
        return UINT_MAX;
    }

    vector < pair < unsigned int, unsigned int > > additionalNextNodes = graph.unpackingNeighbours(source);
    for (unsigned int i = 0; i < additionalNextNodes.size(); i++) {
        //printf("Trying %u (rank %u) as middle node. Distances: %u and %u, that is: %u\n", additionalNextNodes[i].first, graph.data(additionalNextNodes[i].first).rank, additionalNextNodes[i].second, quickFindDistance(additionalNextNodes[i].first, target), quickFindDistance(additionalNextNodes[i].first, target) + unpackingNeighbours[i].second);
        if(distance - additionalNextNodes[i].second == quickFindDistance(additionalNextNodes[i].first, target)) {
            //fallbackCHmanager.printEdgesForwardShortcut(source, additionalNextNodes[i].first);
            //printf("%u -> %u (%u) - remainings: %u\n", source, additionalNextNodes[i].first, additionalNextNodes[i].second, distance - additionalNextNodes[i].second);
            path.push_back(SimpleEdge(source, additionalNextNodes[i].first));
            findPath(additionalNextNodes[i].first, target, path);
            return distance;
        }
    }

    printf("Oops, something went wrong. Didn't find a valid next node when reconstructing the path.\n");
    return UINT_MAX;
}

//______________________________________________________________________________________________________________________
unsigned int TNRPathQueryManager::quickFindDistance(const unsigned int source, const unsigned int target) {
    if (source == target) {
        return 0;
    } else {
        if (graph.isLocalQuery(source, target)) { // Is local query, fallback to some other distance manager, here CH
            return fallbackCHmanager.findDistanceOnly(source, target);
        } else { // Not local query, TNR can be used.
            return graph.findTNRDistance(source, target);
        }
    }
}