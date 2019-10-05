//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include <fstream>
#include "IntegerDistanceMatrixLoader.h"
#include "../../Timer/Timer.h"

//______________________________________________________________________________________________________________________
IntegerDistanceMatrixLoader::IntegerDistanceMatrixLoader(string inputFile) {
    this->inputFile = inputFile;
}

//______________________________________________________________________________________________________________________
IntegerDistanceMatrix * IntegerDistanceMatrixLoader::loadDistanceMatrix() {
    ifstream input;
    input.open(this->inputFile, ios::binary);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    printf("Started loading distance matrix!\n");

    Timer graphLoadTimer("IntegerDistanceMatrix loading");
    graphLoadTimer.begin();

    unsigned int nodes;
    parseHeader(input, nodes);

    IntegerDistanceMatrix * distanceMatrix = new IntegerDistanceMatrix(nodes);

    parseDistances(input, nodes, *distanceMatrix);

    graphLoadTimer.finish();
    graphLoadTimer.printMeasuredTime();

    input.close();

    return distanceMatrix;
}

//______________________________________________________________________________________________________________________
void IntegerDistanceMatrixLoader::parseHeader(ifstream & input, unsigned int & nodes) {
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
void IntegerDistanceMatrixLoader::parseDistances(ifstream & input, const unsigned int nodes, IntegerDistanceMatrix & distanceMatrix) {
    unsigned int distance;
    for(unsigned int i = 0; i < nodes; i++) {
        for(unsigned int j = 0; j < nodes; j++) {
            input.read((char *) &distance, sizeof(distance));
            distanceMatrix.setDistance(i, j, distance);
        }
    }
}