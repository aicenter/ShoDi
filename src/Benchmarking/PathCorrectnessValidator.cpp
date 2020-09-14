//
// Author: Xenty (Michal Cvach)
// Created on: 28.10.19
//

#include "PathCorrectnessValidator.h"
#include "../GraphBuilding/Structures/SimpleEdge.h"
#include "../Timer/Timer.h"
#include <iostream>

//______________________________________________________________________________________________________________________
void PathCorrectnessValidator::validateTNRPaths(Graph * originalGraph, TNRPathQueryManager & tnrQueryManager, vector< pair < unsigned int, unsigned int > > & trips) {
    for(size_t i = 0; i < trips.size(); i++) {
        vector<SimpleEdge> path;
        dist_t distance = tnrQueryManager.findPath(trips[i].first, trips[i].second, path);

        if(validatePath(originalGraph, distance, path) == false) {
            cout << "Path returned by TNR for trip " << i << " is not valid!" << endl;
            return;
        }
    }

    cout << "Validated " << trips.size() << " trips. All paths computed by TNR were valid in the original graph." << endl;

    Timer pathQueriesTimer = Timer("TNR path queries");
    pathQueriesTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        vector<SimpleEdge> path;
        tnrQueryManager.findPath(trips[i].first, trips[i].second, path);
    }

    pathQueriesTimer.finish();
    double time = pathQueriesTimer.getRealTimeSeconds();
    cout << "Performed " << trips.size() << " path queries using TNR in " << time << " seconds." << endl;
    cout << "One query took " << time / ((double) trips.size()) << " seconds." << endl;
}

//______________________________________________________________________________________________________________________
void PathCorrectnessValidator::validateCHPaths(Graph * originalGraph, CHPathQueryManager & chQueryManager, vector< pair < unsigned int, unsigned int > > & trips) {
    for(size_t i = 0; i < trips.size(); i++) {
        vector<SimpleEdge> path;
        dist_t distance = chQueryManager.findPath(trips[i].first, trips[i].second, path);

        if(validatePath(originalGraph, distance, path) == false) {
            cout << "Path returned by CH for trip " << i << " is not valid!" << endl;
            return;
        }
    }

    cout << "Validated " << trips.size() << " trips. All paths computed by CH were valid in the original graph." << endl;

    Timer pathQueriesTimer = Timer("CH path queries");
    pathQueriesTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        vector<SimpleEdge> path;
        chQueryManager.findPath(trips[i].first, trips[i].second, path);
    }

    pathQueriesTimer.finish();
    double time = pathQueriesTimer.getRealTimeSeconds();
    cout << "Performed " << trips.size() << " path queries using CH in " << time << " seconds." << endl;
    cout << "One query took " << ((float) time) / ((float) trips.size()) << " seconds." << endl;
}

//______________________________________________________________________________________________________________________
bool PathCorrectnessValidator::validatePath(Graph * originalGraph, const dist_t distance, vector<SimpleEdge> & path) {
    if (distance == UINT_MAX && path.empty()) {
        return true;
    }

    dist_t edgesDistance = 0;
    for(size_t i = 0; i < path.size(); i++) {
        dist_t iDistance = checkIfEdgeExists(path[i].from, path[i].to, originalGraph);
        if (iDistance == UINT_MAX) {
            cout << "Path returned by TNR contains edge '" << path[i].from << " -> " << path[i].to << "' which does not exist in the original graph." << endl;
            return false;
        }
        edgesDistance += iDistance;
    }

    if(edgesDistance != distance) {
        cout << "Sum of edge weights of the path returned by TNR does not match the distance returned by TNR." << endl;
        cout << "Returned distance: " << distance << ", sum of weights: " << edgesDistance << endl;
        return false;
    }

    return true;
}

//______________________________________________________________________________________________________________________
unsigned int PathCorrectnessValidator::checkIfEdgeExists(const unsigned int from, const unsigned int to, Graph * originalGraph) {
    const vector<pair<unsigned int, unsigned int>> & edges = originalGraph->outgoingEdges(from);
    for(size_t i = 0; i < edges.size(); i++) {
        if(edges[i].first == to) {
            return edges[i].second;
        }
    }

    return UINT_MAX;
}
