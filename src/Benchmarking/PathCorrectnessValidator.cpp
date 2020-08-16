//
// Author: Xenty (Michal Cvach)
// Created on: 28.10.19
//

#include "PathCorrectnessValidator.h"
#include "../GraphBuilding/Structures/SimpleEdge.h"
#include "../Timer/Timer.h"

//______________________________________________________________________________________________________________________
void PathCorrectnessValidator::validateTNRPaths(Graph * originalGraph, TNRPathQueryManager & tnrQueryManager, vector< pair < unsigned int, unsigned int > > & trips) {
    for(unsigned int i = 0; i < trips.size(); i++) {
        vector<SimpleEdge> path;
        unsigned int distance = tnrQueryManager.findPath(trips[i].first, trips[i].second, path);

        if(validatePath(originalGraph, distance, path) == false) {
            printf("Path returned by TNR for trip %u is not valid!\n", i);
            return;
        }
    }

    printf("Validated %u trips. All paths computed by TNR were valid in the original graph.\n", (unsigned int) trips.size());

    Timer pathQueriesTimer = Timer("TNR path queries");
    pathQueriesTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        vector<SimpleEdge> path;
        tnrQueryManager.findPath(trips[i].first, trips[i].second, path);
    }

    pathQueriesTimer.finish();
    double time = pathQueriesTimer.getMeasuredTimeInSeconds();
    printf("Performed %u path queries using TNR in time %f (sec).\n", (unsigned int) trips.size(), time);
    printf("This means one query took %f (sec).\n", time / trips.size());
}

//______________________________________________________________________________________________________________________
void PathCorrectnessValidator::validateCHPaths(Graph * originalGraph, CHPathQueryManager & chQueryManager, vector< pair < unsigned int, unsigned int > > & trips) {
    for(unsigned int i = 0; i < trips.size(); i++) {
        vector<SimpleEdge> path;
        unsigned int distance = chQueryManager.findPath(trips[i].first, trips[i].second, path);

        if(validatePath(originalGraph, distance, path) == false) {
            printf("Path returned by CH for trip %u is not valid!\n", i);
            return;
        }
    }

    printf("Validated %u trips. All paths computed by CH were valid in the original graph.\n", (unsigned int) trips.size());

    Timer pathQueriesTimer = Timer("CH path queries");
    pathQueriesTimer.begin();

    for(unsigned int i = 0; i < trips.size(); i++) {
        vector<SimpleEdge> path;
        chQueryManager.findPath(trips[i].first, trips[i].second, path);
    }

    pathQueriesTimer.finish();
    double time = pathQueriesTimer.getMeasuredTimeInSeconds();
    printf("Performed %u path queries using CH in time %f (sec).\n", (unsigned int) trips.size(), time);
    printf("This means one query took %f (sec).\n", time / trips.size());
}

//______________________________________________________________________________________________________________________
bool PathCorrectnessValidator::validatePath(Graph * originalGraph, const unsigned int distance, vector<SimpleEdge> & path) {
    if (distance == UINT_MAX && path.empty()) {
        return true;
    }

    unsigned int edgesDistance = 0;
    for(unsigned int i = 0; i < path.size(); i++) {
        unsigned int iDistance = checkIfEdgeExists(path[i].from, path[i].to, originalGraph);
        if (iDistance == UINT_MAX) {
            printf("Path returned by TNR contains edge '%u -> %u' which does not exist in the original graph.\n", path[i].from, path[i].to);
            return false;
        }
        edgesDistance += iDistance;
    }

    if(edgesDistance != distance) {
        printf("Sum of edge weights of the path returned by TNR does not match the distance returned by TNR.\n");
        printf("Returned distance: %u, sum of weights: %u\n", distance, edgesDistance);
        return false;
    }

    return true;
}

//______________________________________________________________________________________________________________________
unsigned int PathCorrectnessValidator::checkIfEdgeExists(const unsigned int from, const unsigned int to, Graph * originalGraph) {
    const vector<pair<unsigned int, unsigned int>> & edges = originalGraph->outgoingEdges(from);
    for(unsigned int i = 0; i < edges.size(); i++) {
        if(edges[i].first == to) {
            return edges[i].second;
        }
    }

    return UINT_MAX;
}