//
// Author: Xenty (Michal Cvach)
// Created on: 28.10.19
//

#ifndef CONTRACTION_HIERARCHIES_PATHCORRECTNESSVALIDATOR_H
#define CONTRACTION_HIERARCHIES_PATHCORRECTNESSVALIDATOR_H

#include "../GraphBuilding/Structures/Graph.h"
#include "../TNR/TNRPathQueryManager.h"

using namespace std;

// This class can check whether a path found using some more complex method is valid in the original graph.
// Since Transit Node Routing or Contraction Hierarchies work with a graph that contains shortcut edges, we need to
// ensure that those methods can also reconstruct the path in the original graph. This is only required if we are
// interested in the actual shortest paths. Often we only need to know the shortest distance.
//______________________________________________________________________________________________________________________
class PathCorrectnessValidator {
public:
    static void validateTNRPaths(Graph * originalGraph, TNRPathQueryManager & tnrQueryManager, vector< pair < unsigned int, unsigned int > > & trips);
    static void validateCHPaths(Graph * originalGraph, CHPathQueryManager & chQueryManager, vector< pair < unsigned int, unsigned int > > & trips);
private:
    static bool validatePath(Graph * originalGraph, const unsigned int distance, vector<SimpleEdge> & tnrPath);
    static unsigned int checkIfEdgeExists(const unsigned int from, const unsigned int to, Graph * originalGraph);
};


#endif //CONTRACTION_HIERARCHIES_PATHCORRECTNESSVALIDATOR_H
