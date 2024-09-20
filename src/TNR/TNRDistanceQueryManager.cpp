//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include "TNRDistanceQueryManager.h"

//______________________________________________________________________________________________________________________
TNRDistanceQueryManager::TNRDistanceQueryManager(TransitNodeRoutingGraph<NodeData>& graph) : graph(graph), fallbackCHmanager(graph) {

}

//______________________________________________________________________________________________________________________
unsigned int TNRDistanceQueryManager::findDistance(const unsigned int start, const unsigned int goal) {
    if(start == goal) {
        return 0;
    } else {
        if (graph.isLocalQuery(start, goal)) { // Is local query, fallback to some other distance manager, here CH
            return fallbackCHmanager.findDistance(start, goal);
        } else { // Not local query, TNR can be used.
            return graph.findTNRDistance(start, goal);
        }
    }
}
