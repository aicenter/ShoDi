//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#ifndef SHORTEST_PATHS_CHDISTANCEQUERYMANAGERAPI_H
#define SHORTEST_PATHS_CHDISTANCEQUERYMANAGERAPI_H


#include "../CH/CHDistanceQueryManagerWithMapping.h"
#include <string>



/**
 * This class server as the API which will be used to call the Contraction Hierarchies functionality from Java.
 * It should be enough to generate JNI code for this class using SWIG and then use the generated code in Java.
 * Using this API, one can load a Contraction Hierarchy (basically prepare all the structures for queries),
 * then realize the actual queries, and then clean the structures when they are no longer needed.
 * Please note, that if you do not call the 'clearStructures' function manually from Java, memory leaks will occur.
 */
class CHDistanceQueryManagerAPI {
public:
    /**
     * Initializes all the structures needed to then answer queries. Loads the Contraction Hierarchies data structure
     * from a given file and also loads the mapping.
     *
     * @param chFile[in] The path to the Contraction Hierarchies data structure file.
     * @param mappingFile[in] The path to the mapping file.
     */
    void initializeCH(std::string chFile, std::string mappingFile);

    /**
     * This function will answer a query using the Contraction Hierarchies query algorithm.
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
    CHDistanceQueryManagerWithMapping * qm;
    FlagsGraph * graph;
};


#endif //SHORTEST_PATHS_CHDISTANCEQUERYMANAGERAPI_H
