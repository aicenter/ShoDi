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
    Distance_matrix_travel_time_provider * loadDistanceMatrix();

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
    void parseDistances(std::ifstream & input, const unsigned int nodes, Distance_matrix_travel_time_provider & distanceMatrix);

    std::string inputFile;
};


#endif //CONTRACTION_HIERARCHIES_DISTANCEMATRIXLOADER_H
