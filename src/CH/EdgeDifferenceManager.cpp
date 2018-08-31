//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#include <climits>
#include <cstdio>
#include "EdgeDifferenceManager.h"

//______________________________________________________________________________________________________________________
vector<unsigned int> EdgeDifferenceManager::neighboursContracted(0);
vector<unsigned int> EdgeDifferenceManager::previousContracted(0);

//______________________________________________________________________________________________________________________
void EdgeDifferenceManager::init(const unsigned int nodes) {
    previousContracted.resize(nodes, UINT_MAX);
    neighboursContracted.resize(nodes, 0);
}

//______________________________________________________________________________________________________________________
int EdgeDifferenceManager::difference(const unsigned int contractedNode, const unsigned int x, const unsigned int possibleShortcuts, const unsigned int degree) {
    if (contractedNode != UINT_MAX) {
        if (contractedNode != previousContracted[x]) {
            neighboursContracted[x]++;
            previousContracted[x] = contractedNode;
        }
    }

    return neighboursContracted.at(x) + possibleShortcuts - degree;
}