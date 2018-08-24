//
// Author: Xenty (Michal Cvach)
// Created on: 23.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHQUERYMANAGERWITHRANKS_H
#define TRANSIT_NODE_ROUTING_CHQUERYMANAGERWITHRANKS_H

#include <vector>
#include "../GraphBuilder/Graph.h"
#include "CHQueryManager.h"

using namespace std;

class CHQueryManagerWithRanks : public CHQueryManager {
public:
    CHQueryManagerWithRanks(vector<unsigned int> & x);
    long long unsigned int findDistance(const unsigned int source, const unsigned int target, const Graph & graph);
protected:
    vector<unsigned int> & ranks;

};


#endif //TRANSIT_NODE_ROUTING_CHQUERYMANAGERWITHRANKS_H
