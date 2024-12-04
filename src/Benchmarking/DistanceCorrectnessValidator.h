/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

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
     * Gets a std::vector of trips (pairs of source target queries), for each query it finds a path using
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
            std::vector< std::pair< unsigned int, unsigned int > > & trips,
            FlagsGraphWithUnpackingData & chGraph,
            Graph & originalGraph);
};


#endif //CONTRACTION_HIERARCHIES_DISTANCECORRECTNESSVALIDATOR_H
