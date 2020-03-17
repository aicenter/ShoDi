//
// Author: Xenty (Michal Cvach)
// Created on: 6.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CORECTNESSVALIDATOR_H
#define TRANSIT_NODE_ROUTING_CORECTNESSVALIDATOR_H

#include <vector>

using namespace std;

// This class is used mainly for validation whether the Dijkstra and Contraction Hierarchies provide the same results.
// Since Dijkstra is pretty simple, it serves us as a reference - if Contraction Hierarchies results differ from the
// Dijkstra, something is probably wrong with the Contraction Hierarchies implementation. This class can be also used
// to compare results of two Contaction Hierarchies implementation (or any algorithms for that matter), but in that
// case you should always be sure that at least one of the compared implementations gives correct results.
//______________________________________________________________________________________________________________________
class CorrectnessValidator {
public:
    static bool validate(vector < unsigned int > & a, vector < unsigned int > & b);
    static bool validateVerbose(vector < unsigned int > & a, vector < unsigned int > & b);
private:
};


#endif //TRANSIT_NODE_ROUTING_CORECTNESSVALIDATOR_H
