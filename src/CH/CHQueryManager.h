//
// Author: Xenty (Michal Cvach)
// Created on: 6.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHQUERYMANAGER_H
#define TRANSIT_NODE_ROUTING_CHQUERYMANAGER_H

#include <vector>
#include "../GraphBuilder/Graph.h"

using namespace std;

class CHQueryManager {
public:
    CHQueryManager(vector<unsigned int> & x);
    long long unsigned int findDistance(const unsigned int source, const unsigned int target, const Graph & graph);
protected:
    vector<unsigned int> & ranks;
};


#endif //TRANSIT_NODE_ROUTING_CHQUERYMANAGER_H
