//
// Author: Xenty (Michal Cvach)
// Created on: 02.12.19
//

#include "TNRAFDistanceQueryManager.h"

//______________________________________________________________________________________________________________________
TNRAFDistanceQueryManager::TNRAFDistanceQueryManager(TransitNodeRoutingArcFlagsGraph & graph) : graph(graph), fallbackCHmanager(graph) {

}

//______________________________________________________________________________________________________________________
unsigned int TNRAFDistanceQueryManager::findDistance(const unsigned int start, const unsigned int goal) {
    if(start == goal) {
        return 0;
    } else {
        if (graph.isLocalQuery(start, goal)) { // Is local query, fallback to some other distance manager, here CH
            return fallbackCHmanager.findDistance(start, goal);
        } else { // Not local query, TNR can be used.
            return graph.findTNRAFDistance(start, goal);
        }
    }
}
