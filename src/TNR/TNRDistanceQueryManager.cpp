//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include "TNRDistanceQueryManager.h"

//______________________________________________________________________________________________________________________
TNRDistanceQueryManager::TNRDistanceQueryManager(TransitNodeRoutingGraph & graph) : fallbackCHmanager(graph), graph(graph), localQueries(0), globalQueries(0) {

}

//______________________________________________________________________________________________________________________
unsigned int TNRDistanceQueryManager::findDistance(const unsigned int source, const unsigned int target) {
    if(source == target) {
        return 0;
    } else {
        if (graph.isLocalQuery(source, target)) { // Is local query, fallback to some other distance manager, here CH
            printf("Local query.\n");
            localQueries++;
            return fallbackCHmanager.findDistance(source, target);
        } else { // Not local query, TNR can be used.
            //printf("Global query.\n");
            globalQueries++;
            return graph.findTNRDistance(source, target);
        }
    }
}

//______________________________________________________________________________________________________________________
void TNRDistanceQueryManager::printQueriesAnalysis() {
    printf("Total queries answered: '%u', from that global: '%u', local: '%u', local percentage: '%f'.\n", globalQueries + localQueries, globalQueries, localQueries, (double) localQueries / (globalQueries + localQueries));
}