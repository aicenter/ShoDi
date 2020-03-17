//
// Author: Xenty (Michal Cvach)
// Created on: 28.10.19
//

#ifndef CONTRACTION_HIERARCHIES_PATHCORRECTNESSVALIDATOR_H
#define CONTRACTION_HIERARCHIES_PATHCORRECTNESSVALIDATOR_H

#include "../GraphBuilding/Structures/IntegerGraph.h"
#include "../TNR/TNRPathQueryManager.h"

using namespace std;

class PathCorrectnessValidator {
public:
    static void validateTNRPaths(IntegerGraph * originalGraph, TNRPathQueryManager & tnrQueryManager, vector< pair < unsigned int, unsigned int > > & trips);
    static void validateCHPaths(IntegerGraph * originalGraph, IntegerCHPathQueryManager & chQueryManager, vector< pair < unsigned int, unsigned int > > & trips);
private:
    static bool validatePath(IntegerGraph * originalGraph, const unsigned int distance, vector<SimpleEdge> & tnrPath);
    static unsigned int checkIfEdgeExists(const unsigned int from, const unsigned int to, IntegerGraph * originalGraph);
};


#endif //CONTRACTION_HIERARCHIES_PATHCORRECTNESSVALIDATOR_H
