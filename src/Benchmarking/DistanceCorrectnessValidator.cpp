//
// Author: Xenty (Michal Cvach)
// Created on: 20.08.19
//

#include "DistanceCorrectnessValidator.h"
#include "../CH/CHPathQueryManager.h"

// Gets a vector of trips (pairs of source target queries), for each query it finds a path using Contraction Hierarchies
// and then validates if the path exists in the original graph. The amount of wrong paths is counted. This does not
// check if the returned distances are correct, only validates if the paths exist (those paths can still be suboptimal).
// With that in mind, this should be always used after we are sure that the returned distances are correct, for example
// by validating those using the 'CorrectnessValidator' to compare the distances with the distances computed
// by Dijkstra.
//______________________________________________________________________________________________________________________
void DistanceCorrectnessValidator::validateOnGivenTrips(vector< pair < unsigned int, unsigned int > > & trips, FlagsGraphWithUnpackingData & chGraph, Graph & originalGraph) {
    CHPathQueryManager qm(chGraph);

    unsigned int pathMismatches = 0;
    unsigned int distanceSumMismatches = 0;
    for(unsigned int i = 0; i < trips.size(); i++) {
        vector < pair < unsigned int, unsigned int > > path;
        vector < unsigned int > distances;
        unsigned int reportedDistance = qm.findPath(trips[i].first, trips[i].second, path, distances);

        for(unsigned int j = 0; j < path.size(); j++) {
            unsigned int curSource = path[j].first;
            vector<pair<unsigned int, unsigned int>> neighbours = originalGraph.outgoingEdges(curSource);

            bool found = false;
            for(unsigned int k = 0; k < neighbours.size(); k++) {
                if (neighbours[k].first == path[j].second) {
                    found = true;
                    if (neighbours[k].second != distances[j]) {
                        printf("Found mismatch in trip '%u': length of edge '%u -> %u' reported by CH was %u, actual length is %u.\n", i, curSource, path[j].second, distances[j], neighbours[k].second);
                        pathMismatches++;
                    }
                    break;
                }
            }

            if(! found) {
                printf("Found mismatch in trip '%u': CH reported edge '%u -> %u' which doesn't exist in the original graph.\n", i, curSource, path[j].second);
                pathMismatches++;
                break;
            }

        }

        unsigned int sumDistance = 0;
        for(unsigned int j = 0; j < distances.size(); j++) {
            sumDistance += distances[j];
        }

        if (sumDistance != reportedDistance && reportedDistance != UINT_MAX) {
            printf("Found mismatch in trip '%u': Reported distance was %u while the sum of all the reported edges was %u\n", i, reportedDistance, sumDistance);
            distanceSumMismatches++;
        }

    }

    printf("Finished paths validation.\n"
           "Found %u path mismatches (%f %%)\n"
           "Found %u distance sum mismatches (%f %%)\n", pathMismatches, (double) pathMismatches / trips.size() * 100, distanceSumMismatches, (double) distanceSumMismatches / trips.size() * 100);
}