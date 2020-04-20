//
// Author: Xenty (Michal Cvach)
// Created on: 20.08.19
//

#ifndef CONTRACTION_HIERARCHIES_DISTANCECORRECTNESSVALIDATOR_H
#define CONTRACTION_HIERARCHIES_DISTANCECORRECTNESSVALIDATOR_H


#include "../GraphBuilding/Structures/FlagsGraphWithUnpackingData.h"
#include "../GraphBuilding/Structures/Graph.h"


/**
 * Auxiliary class used for validating that the paths computed by Contraction Hierarchies also exist in the original
 * graph.
 */
class DistanceCorrectnessValidator {
public:
    /**
     * Gets a vector of trips (pairs of source target queries), for each query it finds a path using
     * Contraction Hierarchies and then validates if the path exists in the original graph. The amount of wrong
     * paths is counted. This does not check if the returned distances are correct, only validates if the paths exist
     * (those paths can still be suboptimal). With that in mind, this should be always used after we are sure that
     * the returned distances are correct, for example by validating those using the 'CorrectnessValidator' to compare
     * the distances with the distances computed by Dijkstra.
     *
     * @param trips[in] The set of queries used for the validation.
     * @param chGraph[in] The Contraction Hierarchies data-structure we are validating.
     * @param originalGraph[in] The original graph that will be used for the validation.
     */
    static void validateOnGivenTrips(
            vector< pair < unsigned int, unsigned int > > & trips,
            FlagsGraphWithUnpackingData & chGraph,
            Graph & originalGraph);
};


#endif //CONTRACTION_HIERARCHIES_DISTANCECORRECTNESSVALIDATOR_H
