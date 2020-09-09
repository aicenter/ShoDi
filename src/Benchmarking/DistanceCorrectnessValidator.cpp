//
// Author: Xenty (Michal Cvach)
// Created on: 20.08.19
//

#include <iostream>
#include "DistanceCorrectnessValidator.h"
#include "../CH/CHPathQueryManager.h"

//______________________________________________________________________________________________________________________
void DistanceCorrectnessValidator::validateOnGivenTrips(vector< pair < unsigned int, unsigned int > > & trips, FlagsGraphWithUnpackingData & chGraph, Graph & originalGraph) {
    CHPathQueryManager qm(chGraph);

    unsigned int pathMismatches = 0;
    unsigned int distanceSumMismatches = 0;
    for(size_t i = 0; i < trips.size(); i++) {
        vector < pair < unsigned int, unsigned int > > path;
        vector < unsigned int > distances;
        unsigned int reportedDistance = qm.findPath(trips[i].first, trips[i].second, path, distances);

        for(size_t j = 0; j < path.size(); j++) {
            unsigned int curSource = path[j].first;
            vector<pair<unsigned int, unsigned int>> neighbours = originalGraph.outgoingEdges(curSource);

            bool found = false;
            for(size_t k = 0; k < neighbours.size(); k++) {
                if (neighbours[k].first == path[j].second) {
                    found = true;
                    if (neighbours[k].second != distances[j]) {
                        cout << "Found mismatch in trip '" << i << "': ";
                        cout << "length of edge '" << curSource  << " -> " << path[j].second << "' reported by CH was " << distances[j] << ", actual length is " << neighbours[k].second << "." << endl;
                        pathMismatches++;
                    }
                    break;
                }
            }

            if(! found) {
                cout << "Found mismatch in trip '" << i << "': ";
                cout << "CH reported edge '" << curSource << " -> " << path[j].second << "' which doesn't exist in the original graph." << endl;
                pathMismatches++;
                break;
            }

        }

        unsigned int sumDistance = 0;
        for(size_t j = 0; j < distances.size(); j++) {
            sumDistance += distances[j];
        }

        if (sumDistance != reportedDistance && reportedDistance != UINT_MAX) {
            cout << "Found mismatch in trip '" << i << "': ";
            cout << "Reported distance was " << reportedDistance << " while the sum of all the reported edges was " << sumDistance << endl;
            distanceSumMismatches++;
        }

    }

    cout << "Finished paths validation." << endl;
    cout << "Found '" << pathMismatches << "' path mismatches (" << (double) pathMismatches / trips.size() * 100 << " %)" << endl;
    cout << "Found '" << distanceSumMismatches << "' distance sum mismatches (" << (double) distanceSumMismatches / trips.size() * 100 << " %)" << endl;
}