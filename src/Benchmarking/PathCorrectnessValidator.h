//
// Author: Xenty (Michal Cvach)
// Created on: 28.10.19
//

#ifndef CONTRACTION_HIERARCHIES_PATHCORRECTNESSVALIDATOR_H
#define CONTRACTION_HIERARCHIES_PATHCORRECTNESSVALIDATOR_H

#include "../GraphBuilding/Structures/Graph.h"
#include "../TNR/TNRPathQueryManager.h"

using namespace std;

class PathCorrectnessValidator {
public:
    static void validateTNRPaths(Graph * originalGraph, TNRPathQueryManager & tnrQueryManager, vector< pair < unsigned int, unsigned int > > & trips);
    static void validateCHPaths(Graph * originalGraph, CHPathQueryManager & chQueryManager, vector< pair < unsigned int, unsigned int > > & trips);
private:
    static bool validatePath(Graph * originalGraph, const unsigned int distance, vector<SimpleEdge> & tnrPath);
    static unsigned int checkIfEdgeExists(const unsigned int from, const unsigned int to, Graph * originalGraph);
};


#endif //CONTRACTION_HIERARCHIES_PATHCORRECTNESSVALIDATOR_H
