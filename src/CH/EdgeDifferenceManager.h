//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#ifndef TRANSIT_NODE_ROUTING_EDGEDIFFERENCEMANAGER_H
#define TRANSIT_NODE_ROUTING_EDGEDIFFERENCEMANAGER_H

#include <vector>

using namespace std;

class EdgeDifferenceManager {
public:
    static void init(const unsigned int nodes);
    static int difference(const unsigned int contractedNode, const unsigned int x, const unsigned int possibleShortcuts, const unsigned int degree);
private:
    static vector<unsigned int> neighboursContracted;
    static vector<unsigned int> previousContracted;
};


#endif //TRANSIT_NODE_ROUTING_EDGEDIFFERENCEMANAGER_H
