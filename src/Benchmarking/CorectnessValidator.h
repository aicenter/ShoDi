//
// Author: Xenty (Michal Cvach)
// Created on: 6.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CORECTNESSVALIDATOR_H
#define TRANSIT_NODE_ROUTING_CORECTNESSVALIDATOR_H

#include <vector>

using namespace std;

class CorectnessValidator {
public:
    static bool validate(vector < long long unsigned int > & a, vector < long long unsigned int > & b);
    static bool validateVerbose(vector < long long unsigned int > & a, vector < long long unsigned int > & b);
private:
};


#endif //TRANSIT_NODE_ROUTING_CORECTNESSVALIDATOR_H
