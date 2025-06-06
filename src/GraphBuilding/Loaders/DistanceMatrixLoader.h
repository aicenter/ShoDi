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
// Created on: 05.10.19
//

#ifndef CONTRACTION_HIERARCHIES_DISTANCEMATRIXLOADER_H
#define CONTRACTION_HIERARCHIES_DISTANCEMATRIXLOADER_H

#include <string>
#include "../../DistanceMatrix/Distance_matrix_travel_time_provider.h"



/**
 * Class used for loading a Distance Matrix. The Distance Matrix is saved in a simple binary format which is described
 * in the 'DistanceMatrix.h' file. This loader allows to load such Distance Matrices.
 */
class DistanceMatrixLoader {
public:
    /**
     * A simple constructor.
     *
     * @param inputFile[in] A path towards a file that should be loaded using this loader.
     */
    explicit DistanceMatrixLoader(std::string inputFile);

    /**
     * Loads the distance matrix from the file that was given to the loader during its initialization.
     *
     * @return An instance of the DistanceMatrix class filled with distances for all pairs of nodes.
     */
    Distance_matrix_travel_time_provider<dist_t>* loadXDM();

    /**
     * Loads the distance matrix from the file that was given to the loader during its initialization.
     *
     * @return An instance of the DistanceMatrix class filled with distances for all pairs of nodes.
     */
    DistanceMatrixInterface* loadHDF();

private:
    /**
     * Auxiliary function used to parse the header (the first line) of the input ifle.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param nodes[out] The number of nodes in the graph.
     */
    void parseHeader(std::ifstream & input, unsigned int & nodes);

    /**
     * Auxiliary function used to load the distances into the distance matrix.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param nodes[in] The number of nodes in the graph.
     * @param distanceMatrix[out] The distance matrix instance that will be filled by this function.
     */
    void parseDistances(std::ifstream & input, const unsigned int nodes, Distance_matrix_travel_time_provider<dist_t>& distanceMatrix);

    std::string inputFile;
};


#endif //CONTRACTION_HIERARCHIES_DISTANCEMATRIXLOADER_H
