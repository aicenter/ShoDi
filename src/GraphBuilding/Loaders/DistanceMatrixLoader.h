//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#ifndef CONTRACTION_HIERARCHIES_DISTANCEMATRIXLOADER_H
#define CONTRACTION_HIERARCHIES_DISTANCEMATRIXLOADER_H

#include <string>
#include "../../DistanceMatrix/DistanceMatrix.h"

using namespace std;

// Class used for loading a Distance Matrix. The Distance Matrix is saved in a simple binary format which is described
// in the 'DistanceMatrixComputor.cpp' file. This loader allows to load such Distance Matrices.
//______________________________________________________________________________________________________________________
class DistanceMatrixLoader {
public:
    DistanceMatrixLoader(string inputFile);
    DistanceMatrix * loadDistanceMatrix();
private:
    void parseHeader(ifstream & input, unsigned int & nodes);
    void parseDistances(ifstream & input, const unsigned int nodes, DistanceMatrix & distanceMatrix);
    string inputFile;
};


#endif //CONTRACTION_HIERARCHIES_DISTANCEMATRIXLOADER_H
