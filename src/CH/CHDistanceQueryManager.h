//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHFLAGSGRAPHQUERYMANAGER_H
#define TRANSIT_NODE_ROUTING_CHFLAGSGRAPHQUERYMANAGER_H


#include <vector>
#include <queue>
#include "../GraphBuilding/Structures/FlagsGraph.h"



/**
 * This class is responsible for the Contraction Hierarchies 'distance' queries - when we only require the 'distance'
 * between two points and do not care about the actual path.
 */
class CHDistanceQueryManager {
public:
    /**
     * A simple constructor.
     *
     * @param g[in] The FlagsGraph instance we will be using to answer shortest distance queries.
     */
    CHDistanceQueryManager(FlagsGraph & g);

    /**
     * We use the query algorithm that was described in the "Contraction Hierarchies: Faster and Simpler Hierarchical
     * Routing in Road Networks" article by Robert Geisberger, Peter Sanders, Dominik Schultes, and Daniel Delling.
     * Basically, the query is a modified bidirectional Dijkstra query, where from the start node we only expand
     * following nodes with higher contraction rank than the current node and from the goal we only expand previous
     * nodes with higher contraction rank than the current node. Both scopes will eventually meet in the node with the
     * highest contraction rank from all nodes in the path.
     *
     * @param start[in] The start node for the query.
     * @param goal[in] The goal node for the query.
     * @return Returns the shortest distance from start to goal or 'UINT_MAX' if goal is not reachable from start.
     */
    unsigned int findDistance(const unsigned int start, const unsigned int goal);

protected:
    /**
     * Reset information for the nodes that were changed in the current query. This is required so that following
     * queries are not influenced by the current query. Using this function ensures that following queries also
     * give correct results.
     */
    void prepareStructuresForNextQuery();

    FlagsGraph & graph;
    unsigned int upperbound;
    std::vector<unsigned int> forwardChanged;
    std::vector<unsigned int> backwardChanged;
    std::vector<unsigned int> forwardStallChanged;
    std::vector<unsigned int> backwardStallChanged;
};

#endif //TRANSIT_NODE_ROUTING_CHFLAGSGRAPHQUERYMANAGER_H
