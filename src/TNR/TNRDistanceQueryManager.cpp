//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include "TNRDistanceQueryManager.h"

TNRDistanceQueryManager::TNRDistanceQueryManager(TransitNodeRoutingGraph & graph) : fallbackCHmanager(graph), graph(graph) {

}

unsigned int TNRDistanceQueryManager::findDistance(const unsigned int source, const unsigned int target) {
    if(source == target) {
        return 0;
    } else {
        if (graph.isLocalQuery(source, target)) { // Is local query, fallback to some other distance manager, here CH
            return fallbackCHmanager.findDistance(source, target);
        } else { // Not local query, TNR can be used.
            return graph.findTNRDistance(source, target);
        }
    }
}