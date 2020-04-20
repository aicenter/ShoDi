//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRDISTANCEQUERYMANAGER_H
#define CONTRACTION_HIERARCHIES_TNRDISTANCEQUERYMANAGER_H


#include "../GraphBuilding/Structures/TransitNodeRoutingGraph.h"
#include "../CH/CHDistanceQueryManager.h"

/**
 * This class handles distance queries over the TNR data structure (that means queries where we don't want to find the
 * actual shortest path, but instead we are only interested in its distance).
 */
class TNRDistanceQueryManager {
public:
    /**
     * Initializes the query manager.
     *
     * @param graph[in] The Transit Node Routing data structure that will be used to answer queries.
     */
    TNRDistanceQueryManager(
            TransitNodeRoutingGraph & graph);

    /**
     * Actually finds the distance between two targets. If start != goal, this function first invokes the locality
     * filter to determine whether the query is local. If it is local, we fallback to the Contraction Hierarchies query
     * algorithm. This is slightly slower, but the local queries already guarantee that the start and goal nodes will
     * be reasonably close to each other and in that case the query should take relatively short time. If the query
     * is not local, Transit Node Routing structure can be used to determine the shortest distance, and in that case
     * it is invoked to do so.
     *
     * @param start[in] The ID of the start node of the query.
     * @param goal[in] The ID of the goal node of the query.
     * @return Returns the shortest distance from start to goal in the graph or UINT_MAX if goal can not be reached
     * from start.
     */
    unsigned int findDistance(
            const unsigned int start,
            const unsigned int goal);

    /**
     * Auxiliary function only printing some info about the queries done since this manager was created. This is used
     * currently for debugging purposes and should be removed later. FIXME
     */
    void printQueriesAnalysis();
protected:
    TransitNodeRoutingGraph & graph;
    CHDistanceQueryManager fallbackCHmanager;
    unsigned int globalQueries;
    unsigned int localQueries;
};


#endif //CONTRACTION_HIERARCHIES_TNRDISTANCEQUERYMANAGER_H
