//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#include "FPointFlagsGraphWithUnpackingData.h"
#include <climits>

FPointFlagsGraphWithUnpackingData::FPointFlagsGraphWithUnpackingData(unsigned int n) : FPointFlagsGraph(n) {
    forwardPrev.resize(n, UINT_MAX);
    backwardPrev.resize(n, UINT_MAX);
}

//______________________________________________________________________________________________________________________
void FPointFlagsGraphWithUnpackingData::addForwardUnpackingData(unsigned int s, unsigned int t, unsigned int m) {
    forwardUnpackingData.insert(make_pair(make_pair(s,t), m));
}

//______________________________________________________________________________________________________________________
void FPointFlagsGraphWithUnpackingData::addBackwardUnpackingData(unsigned int s, unsigned int t, unsigned int m) {
    backwardUnpackingData.insert(make_pair(make_pair(s,t), m));
}


//______________________________________________________________________________________________________________________
unsigned int FPointFlagsGraphWithUnpackingData::getForwardMiddleNode(unsigned int s, unsigned int t) {
    if (forwardUnpackingData.count(make_pair(s, t)) == 1) {
        return forwardUnpackingData[make_pair(s, t)];
    }
    return UINT_MAX;
}

//______________________________________________________________________________________________________________________
unsigned int FPointFlagsGraphWithUnpackingData::getBackwardMiddleNode(unsigned int s, unsigned int t) {
    if (backwardUnpackingData.count(make_pair(s, t)) == 1) {
        return backwardUnpackingData[make_pair(s, t)];
    }
    return UINT_MAX;
}

//______________________________________________________________________________________________________________________
void FPointFlagsGraphWithUnpackingData::setForwardPrev(unsigned int x, unsigned int y) {
    forwardPrev[x] = y;
}

//______________________________________________________________________________________________________________________
void FPointFlagsGraphWithUnpackingData::setBackwardPrev(unsigned int x, unsigned int y) {
    backwardPrev[x] = y;
}

//______________________________________________________________________________________________________________________
unsigned int FPointFlagsGraphWithUnpackingData::getForwardPrev(unsigned int x) {
    return forwardPrev[x];
}

//______________________________________________________________________________________________________________________
unsigned int FPointFlagsGraphWithUnpackingData::getBackwardPrev(unsigned int x) {
    return backwardPrev[x];
}

//______________________________________________________________________________________________________________________
void FPointFlagsGraphWithUnpackingData::resetForwardPrev(unsigned int x) {
    forwardPrev[x] = UINT_MAX;
}

//______________________________________________________________________________________________________________________
void FPointFlagsGraphWithUnpackingData::resetBackwardPrev(unsigned int x) {
    backwardPrev[x] = UINT_MAX;
}
