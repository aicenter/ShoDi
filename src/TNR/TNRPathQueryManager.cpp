//
// Author: Xenty (Michal Cvach)
// Created on: 20.08.19
//

#include "TNRPathQueryManager.h"

//______________________________________________________________________________________________________________________
TNRPathQueryManager::TNRPathQueryManager(TransitNodeRoutingGraph & graph) : fallbackCHmanager(graph), graph(graph) {

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
            distance = fallbackCHmanager.findDistance(source, target);
        } else { // Not local query, TNR can be used.
            distance = graph.findTNRDistance(source, target);
        }
    }

    if (distance == UINT_MAX) {
        printf("Couldn't find path from source to target. Nothing to output.\n");
        return UINT_MAX;
    }

    vector< IntegerQueryEdge > nextNodes = graph.nextNodes(source);
    for(unsigned int i = 0; i < nextNodes.size(); i++) {
        if(distance - nextNodes[i].weight == quickFindDistance(nextNodes[i].targetNode, target)) {
            printf("%u -> %u (%llu) - remainings: %llu\n", source, nextNodes[i].targetNode, nextNodes[i].weight, distance - nextNodes[i].weight);
            findDistance(nextNodes[i].targetNode, target);
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
            return fallbackCHmanager.findDistance(source, target);
        } else { // Not local query, TNR can be used.
            return graph.findTNRDistance(source, target);
        }
    }
}