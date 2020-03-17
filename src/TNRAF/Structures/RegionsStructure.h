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
    RegionsStructure(unsigned int nodesCnt, unsigned int regionsCnt);
    void addNode(unsigned int nodeId, unsigned int regionId);
    vector < unsigned int > & nodesInRegion(unsigned int regionId);
    unsigned int getRegion(unsigned int nodeId);
    unsigned int getRegionsCnt();
private:
    vector < vector < unsigned int > > regions;
    vector < unsigned int > mapping;
    unsigned int regionsCnt;
};


#endif //CONTRACTION_HIERARCHIES_REGIONSSTRUCTURE_H