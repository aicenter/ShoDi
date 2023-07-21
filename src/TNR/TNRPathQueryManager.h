//
// Author: Xenty (Michal Cvach)
// Created on: 20.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRPATHQUERYMANAGER_H
#define CONTRACTION_HIERARCHIES_TNRPATHQUERYMANAGER_H

#include "../GraphBuilding/Structures/TransitNodeRoutingGraph.h"
#include "../CH/CHDistanceQueryManager.h"
#include "../CH/CHPathQueryManager.h"
#include "../GraphBuilding/Structures/TransitNodeRoutingGraphForPathQueries.h"
#include "../GraphBuilding/Structures/SimpleEdge.h"

/**
 * This class handles Path queries over the TNR data structure (that means queries where we are interested in the actual
 * shortest path and not just the shortest distance).
 */
class TNRPathQueryManager {
public:
    /**
     *
     * @param graph
     */
    TNRPathQueryManager(
            TransitNodeRoutingGraphForPathQueries & graph);

    /**
     * Actually finds the distance between two targets. If start != goal, this function first invokes the locality
     * filter to determine whether the query is local. If it is local, we fallback to the Contraction Hierarchies query
     * algorithm. This is slightly slower, but the local queries already guarantee that the start and goal nodes will
     * be reasonably close to each other and in that case the query should take relatively short time.
     * If the query is not local, Transit Node Routing structure can be used to determine the shortest distance,
     * and in that case it is invoked to do so.
     *
     * @param start
     * @param goal
     * @return
     */
    unsigned int findDistance(
            const unsigned int start,
            const unsigned int goal);

    /**
     *
     * @param start
     * @param goal
     * @param path
     * @return
     */
    unsigned int findPath(
            const unsigned int start,
            const unsigned int goal,
            std::vector<SimpleEdge> & path);

    /**
     *
     * @param start
     * @param goal
     * @return
     */
    unsigned int quickFindDistance(
            const unsigned int start,
            const unsigned int goal);
private:
    TransitNodeRoutingGraphForPathQueries & graph;
    CHPathQueryManager fallbackCHmanager;
};


#endif //CONTRACTION_HIERARCHIES_TNRPATHQUERYMANAGER_H
