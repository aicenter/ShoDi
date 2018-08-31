//
// Author: Xenty (Michal Cvach)
// Created on: 30.8.18
//

#ifndef TRANSIT_NODE_ROUTING_FLAGSGRAPHWITHUNPACKINGDATA_H
#define TRANSIT_NODE_ROUTING_FLAGSGRAPHWITHUNPACKINGDATA_H

#include <map>
#include "FlagsGraph.h"

class FlagsGraphWithUnpackingData : public FlagsGraph {
protected:
    map<pair<unsigned int, unsigned int>, unsigned int> unpackingData;
    vector<unsigned int> forwardPrev;
    vector<unsigned int> backwardPrev;
public:
    FlagsGraphWithUnpackingData(unsigned int n);
    void addUnpackingData(unsigned int s, unsigned int t, unsigned int m);
    unsigned int getMiddleNode(unsigned int s, unsigned int t);
    void setForwardPrev(unsigned int x, unsigned int y);
    void setBackwardPrev(unsigned int x, unsigned int y);
    unsigned int getForwardPrev(unsigned int x);
    unsigned int getBackwardPrev(unsigned int x);
    void resetForwardPrev(unsigned int x);
    void resetBackwardPrev(unsigned int x);
    void debugPrint();
};


#endif //TRANSIT_NODE_ROUTING_FLAGSGRAPHWITHUNPACKINGDATA_H
