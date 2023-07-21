//
// Author: Xenty (Michal Cvach)
// Created on: 20.08.19
//

#include <iostream>
#include "DistanceCorrectnessValidator.h"
#include "../CH/CHPathQueryManager.h"

//______________________________________________________________________________________________________________________
void DistanceCorrectnessValidator::validateOnGivenTrips(std::vector< std::pair< unsigned int, unsigned int > > & trips, FlagsGraphWithUnpackingData & chGraph, Graph & originalGraph) {
    CHPathQueryManager qm(chGraph);

    unsigned int pathMismatches = 0;
    unsigned int distanceSumMismatches = 0;
    for(size_t i = 0; i < trips.size(); i++) {
        std::vector < std::pair< unsigned int, unsigned int > > path;
        std::vector < unsigned int > distances;
        unsigned int reportedDistance = qm.findPath(trips[i].first, trips[i].second, path, distances);

        for(size_t j = 0; j < path.size(); j++) {
            unsigned int curSource = path[j].first;
            std::vector<std::pair<unsigned int, unsigned int>> neighbours = originalGraph.outgoingEdges(curSource);

            bool found = false;
            for(size_t k = 0; k < neighbours.size(); k++) {
                if (neighbours[k].first == path[j].second) {
                    found = true;
                    if (neighbours[k].second != distances[j]) {
                        std::cout << "Found mismatch in trip '" << i << "': ";
                        std::cout << "length of edge '" << curSource  << " -> " << path[j].second << "' reported by CH was " << distances[j] << ", actual length is " << neighbours[k].second << "." << std::endl;
                        pathMismatches++;
                    }
                    break;
                }
            }

            if(! found) {
                std::cout << "Found mismatch in trip '" << i << "': ";
                std::cout << "CH reported edge '" << curSource << " -> " << path[j].second << "' which doesn't exist in the original graph." << std::endl;
                pathMismatches++;
                break;
            }

        }

        unsigned int sumDistance = 0;
        for(size_t j = 0; j < distances.size(); j++) {
            sumDistance += distances[j];
        }

        if (sumDistance != reportedDistance && reportedDistance != UINT_MAX) {
            std::cout << "Found mismatch in trip '" << i << "': ";
            std::cout << "Reported distance was " << reportedDistance << " while the sum of all the reported edges was " << sumDistance << std::endl;
            distanceSumMismatches++;
        }

    }

    std::cout << "Finished paths validation." << std::endl;
    std::cout << "Found '" << pathMismatches << "' path mismatches (" << (double) pathMismatches / (double) trips.size() * 100 << " %)" << std::endl;
    std::cout << "Found '" << distanceSumMismatches << "' distance sum mismatches (" << (double) distanceSumMismatches / (double) trips.size() * 100 << " %)" << std::endl;
}
