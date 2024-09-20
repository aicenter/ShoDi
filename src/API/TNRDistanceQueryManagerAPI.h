//
// Author: Xenty (Michal Cvach)
// Created on: 21.03.20
//

#ifndef SHORTEST_PATHS_TNRDISTANCEQUERYMANAGERAPI_H
#define SHORTEST_PATHS_TNRDISTANCEQUERYMANAGERAPI_H


#include "../TNR/TNRDistanceQueryManagerWithMapping.h"



/**
 * This class server as the API which will be used to call the Transit Node Routing functionality from Java.
 * It should be enough to generate JNI code for this class using SWIG and then use the generated code in Java.
 * Using this API, one can load a Transit Node Routing data structure (to prepare all the structures for queries),
 * then realize the actual queries, and then clean the structures when they are no longer needed.
 * Please note, that if you do not call the 'clearStructures' function manually from Java, memory leaks will occur.
 */
class TNRDistanceQueryManagerAPI {
public:
    /**
     * Initializes all the structures needed to then answer queries. Loads the Transit Node Routing data structure from
     * a given file and also loads the mapping.
     *
     * @param tnrFile[in] The path to the Transit Node Routing data structure file.
     * @param mappingFile[in] The path to the mapping file.
     */
    void initializeTNR(std::string tnrFile, std::string mappingFile);

    /**
     * This function will answer a query using the Transit Node Routing query algorithm.
     *
     * @param start[in] The start node of the query.
     * @param goal[in] The goal node of the query.
     * @return Returns the shortest distance from 'start' to 'goal' or 'UINT_MAX' if goal is not reachable from start.
     */
    unsigned int distanceQuery(long long unsigned int start, long long unsigned int goal);

    /**
     * Clears all the memory required by the structures. This needs to be called explicitly when using those managers,
     * otherwise memory leaks will occur.
     */
    void clearStructures();

private:
    TNRDistanceQueryManagerWithMapping * qm;
    TransitNodeRoutingGraph<NodeData>* graph;
};


#endif //SHORTEST_PATHS_TNRDISTANCEQUERYMANAGERAPI_H
