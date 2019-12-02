//
// Author: Xenty (Michal Cvach)
// Created on: 02.12.19
//

#ifndef CONTRACTION_HIERARCHIES_REGIONSSTRUCTURE_H
#define CONTRACTION_HIERARCHIES_REGIONSSTRUCTURE_H

#include <vector>

using namespace std;

class RegionsStructure {
public:
    RegionsStructure(unsigned int regionsCnt);
    void addNode(unsigned int nodeId, unsigned int regionId);
private:
    vector < vector < unsigned int > > regions;
};


#endif //CONTRACTION_HIERARCHIES_REGIONSSTRUCTURE_H
