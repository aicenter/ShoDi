//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#ifndef CONTRACTION_HIERARCHIES_INTEGERDISTANCEMATRIXLOADER_H
#define CONTRACTION_HIERARCHIES_INTEGERDISTANCEMATRIXLOADER_H

#include <string>
#include "../../DistanceMatrix/IntegerDistanceMatrix.h"

using namespace std;

// Class used for loading a Distance Matrix. The Distance Matrix is saved in a simple binary format which is described
// in the 'IntegerDistanceMatrixComputor.cpp' file. This loader allows to load such Distance Matrices.
//______________________________________________________________________________________________________________________
class IntegerDistanceMatrixLoader {
public:
    IntegerDistanceMatrixLoader(string inputFile);
    IntegerDistanceMatrix * loadDistanceMatrix();
private:
    void parseHeader(ifstream & input, unsigned int & nodes);
    void parseDistances(ifstream & input, const unsigned int nodes, IntegerDistanceMatrix & distanceMatrix);
    string inputFile;
};


#endif //CONTRACTION_HIERARCHIES_INTEGERDISTANCEMATRIXLOADER_H
