//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#ifndef CONTRACTION_HIERARCHIES_CHDISTANCEQUERYMANAGERWITHMAPPING_H
#define CONTRACTION_HIERARCHIES_CHDISTANCEQUERYMANAGERWITHMAPPING_H


#include "../GraphBuilding/Structures/FlagsGraph.h"
#include "CHDistanceQueryManager.h"
#include <string>
#include <unordered_map>



/**
 * Allows us to answer distance queries using the original indices of the nodes. Internally, nodes are indexed from
 * 0 to n-1. Since in the original graph, the nodes indices might be any numbers, this class allows us to load a mapping
 * from the original indices to our indices and then answer queries using the original indices. This uses the standard
 * CHDistanceQueryManager internally.
 */
class CHDistanceQueryManagerWithMapping {
public:
    /**
     * Initializes the query manager. Here, the mapping from the original indices to our indices is loaded.
     *
     * @param g[in] The Contraction Hierarchies data structure that will be used to answer queries.
     * @param mappingFilepath[in] The path to the file that contains the mapping from original indices to indices
     * in the data structure.
     */
    CHDistanceQueryManagerWithMapping(FlagsGraph<NodeData>& g, std::string mappingFilepath);

    /**
     * Used to find the shortest distance from start to goal where start and goal are the original indices.
     *
     * @param start[in] The original ID of the start node of the query.
     * @param goal[in] The original ID of the goal node of the query.
     * @return Returns the shortest distance from start to goal in the graph or 'UINT_MAX' if goal can not be reached
     * from start.
     */
    unsigned int findDistance(const long long unsigned int start, const long long unsigned int goal);

private:
    CHDistanceQueryManager<NodeData> qm;
    std::unordered_map<long long unsigned int, unsigned int> mapping;
};


#endif //CONTRACTION_HIERARCHIES_CHDISTANCEQUERYMANAGERWITHMAPPING_H
