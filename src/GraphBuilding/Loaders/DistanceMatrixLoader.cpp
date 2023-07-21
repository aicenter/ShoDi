//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include <fstream>
#include "DistanceMatrixLoader.h"
#include "../../Timer/Timer.h"

//______________________________________________________________________________________________________________________
DistanceMatrixLoader::DistanceMatrixLoader(std::string inputFile) {
    this->inputFile = inputFile;
}

//______________________________________________________________________________________________________________________
Distance_matrix_travel_time_provider * DistanceMatrixLoader::loadDistanceMatrix() {
    std::ifstream input;
    input.open(this->inputFile, std::ios::binary);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes;
    parseHeader(input, nodes);

    Distance_matrix_travel_time_provider * distanceMatrix = new Distance_matrix_travel_time_provider(nodes);

    parseDistances(input, nodes, *distanceMatrix);

    input.close();

    return distanceMatrix;
}

//______________________________________________________________________________________________________________________
void DistanceMatrixLoader::parseHeader(std::ifstream & input, unsigned int & nodes) {
    char c1, c2, c3;
    input.read(&c1, sizeof(c1));
    input.read(&c2, sizeof(c2));
    input.read(&c3, sizeof(c3));
    if (c1 != 'X' || c2 != 'D' || c3 != 'M') {
        printf("The input file is missing the Distance Matrix header.\n");
        printf("Are you sure the input file is in the correct format?\n");
        printf("The loading will proceed but the loaded distance matrix might be corrupted.\n");
    }

    input.read((char *) &nodes, sizeof(nodes));
}

//______________________________________________________________________________________________________________________
void DistanceMatrixLoader::parseDistances(std::ifstream & input, const unsigned int nodes, Distance_matrix_travel_time_provider & distanceMatrix) {
    unsigned int distance;
    for(unsigned int i = 0; i < nodes; i++) {
        for(unsigned int j = 0; j < nodes; j++) {
            input.read((char *) &distance, sizeof(distance));
            distanceMatrix.setDistance(i, j, distance);
        }
    }
}