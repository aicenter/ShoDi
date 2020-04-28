//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include "TNRDistanceQueryManager.h"

//______________________________________________________________________________________________________________________
TNRDistanceQueryManager::TNRDistanceQueryManager(TransitNodeRoutingGraph & graph) : graph(graph), fallbackCHmanager(graph), globalQueries(0), localQueries(0) {

}

//______________________________________________________________________________________________________________________
unsigned int TNRDistanceQueryManager::findDistance(const unsigned int start, const unsigned int goal) {
    if(start == goal) {
        return 0;
    } else {
        if (graph.isLocalQuery(start, goal)) { // Is local query, fallback to some other distance manager, here CH
            //printf("Local query.\n");
            localQueries++;
            return fallbackCHmanager.findDistance(start, goal);
        } else { // Not local query, TNR can be used.
            //printf("Global query.\n");
            globalQueries++;
            return graph.findTNRDistance(start, goal);
        }
    }
}

//______________________________________________________________________________________________________________________
void TNRDistanceQueryManager::printQueriesAnalysis() {
    printf("Total queries answered: '%u', from that global: '%u', local: '%u', local percentage: '%f'.\n", globalQueries + localQueries, globalQueries, localQueries, (double) localQueries / (globalQueries + localQueries));
}