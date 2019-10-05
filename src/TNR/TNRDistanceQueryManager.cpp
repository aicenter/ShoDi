//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include "TNRDistanceQueryManager.h"

//______________________________________________________________________________________________________________________
TNRDistanceQueryManager::TNRDistanceQueryManager(TransitNodeRoutingGraph & graph) : fallbackCHmanager(graph), graph(graph), localQueries(0), globalQueries(0) {

}

// Actually finds the distance between two targets. If source != target, this function first invokes the locality
// filter to determine whether the query is local. If it is local, we fallback to the Contraction Hierarchies query
// algorithm. This is slightly slower, but the local queries already guarantee that the source and target nodes will
// be reasonably close to each other and in that case the query should take relatively short time. If the query is not
// local, Transit Node Routing structure can be used to determine the shortest distance, and in that case it is invoked
// to do so.
//______________________________________________________________________________________________________________________
unsigned int TNRDistanceQueryManager::findDistance(const unsigned int source, const unsigned int target) {
    if(source == target) {
        return 0;
    } else {
        if (graph.isLocalQuery(source, target)) { // Is local query, fallback to some other distance manager, here CH
            //printf("Local query.\n");
            localQueries++;
            return fallbackCHmanager.findDistance(source, target);
        } else { // Not local query, TNR can be used.
            //printf("Global query.\n");
            globalQueries++;
            return graph.findTNRDistance(source, target);
        }
    }
}

// Auxiliary function only printing some info about the queries done since this manager was created. This is used
// currently for debugging purposes and should be removed later. FIXME
//______________________________________________________________________________________________________________________
void TNRDistanceQueryManager::printQueriesAnalysis() {
    printf("Total queries answered: '%u', from that global: '%u', local: '%u', local percentage: '%f'.\n", globalQueries + localQueries, globalQueries, localQueries, (double) localQueries / (globalQueries + localQueries));
}