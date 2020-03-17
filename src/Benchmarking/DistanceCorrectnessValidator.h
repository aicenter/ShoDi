//
// Author: Xenty (Michal Cvach)
// Created on: 20.08.19
//

#ifndef CONTRACTION_HIERARCHIES_DISTANCECORRECTNESSVALIDATOR_H
#define CONTRACTION_HIERARCHIES_DISTANCECORRECTNESSVALIDATOR_H


#include "../GraphBuilding/Structures/IntegerFlagsGraphWithUnpackingData.h"
#include "../GraphBuilding/Structures/IntegerGraph.h"

// Auxiliary class used for validating that the paths computed by Contraction Hierarchies also exist in the original
// graph.
//______________________________________________________________________________________________________________________
class DistanceCorrectnessValidator {
public:
    static void validateOnGivenTrips(vector< pair < unsigned int, unsigned int > > & trips, IntegerFlagsGraphWithUnpackingData & chGraph, IntegerGraph & originalGraph);
};


#endif //CONTRACTION_HIERARCHIES_DISTANCECORRECTNESSVALIDATOR_H
