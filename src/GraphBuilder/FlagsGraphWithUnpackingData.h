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
    map<pair<unsigned int, unsigned int>, unsigned int> forwardUnpackingData;
    map<pair<unsigned int, unsigned int>, unsigned int> backwardUnpackingData;
    vector<unsigned int> forwardPrev;
    vector<unsigned int> backwardPrev;
public:
    FlagsGraphWithUnpackingData(unsigned int n);
    void addForwardUnpackingData(unsigned int s, unsigned int t, unsigned int m);
    void addBackwardUnpackingData(unsigned int s, unsigned int t, unsigned int m);
    unsigned int getForwardMiddleNode(unsigned int s, unsigned int t);
    unsigned int getBackwardMiddleNode(unsigned int s, unsigned int t);
    void setForwardPrev(unsigned int x, unsigned int y);
    void setBackwardPrev(unsigned int x, unsigned int y);
    unsigned int getForwardPrev(unsigned int x);
    unsigned int getBackwardPrev(unsigned int x);
    void resetForwardPrev(unsigned int x);
    void resetBackwardPrev(unsigned int x);
};


#endif //TRANSIT_NODE_ROUTING_FLAGSGRAPHWITHUNPACKINGDATA_H
