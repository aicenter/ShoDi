//
// Author: Xenty (Michal Cvach)
// Created on: 30.8.18
//

#include <climits>
#include "FlagsGraphWithUnpackingData.h"

FlagsGraphWithUnpackingData::FlagsGraphWithUnpackingData(unsigned int n) : FlagsGraph(n) {
    forwardPrev.resize(n, UINT_MAX);
    backwardPrev.resize(n, UINT_MAX);
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::addForwardUnpackingData(unsigned int s, unsigned int t, unsigned int m) {
    forwardUnpackingData.insert(make_pair(make_pair(s,t), m));
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::addBackwardUnpackingData(unsigned int s, unsigned int t, unsigned int m) {
    backwardUnpackingData.insert(make_pair(make_pair(s,t), m));
}


//______________________________________________________________________________________________________________________
unsigned int FlagsGraphWithUnpackingData::getForwardMiddleNode(unsigned int s, unsigned int t) {
    if (forwardUnpackingData.count(make_pair(s, t)) == 1) {
        return forwardUnpackingData[make_pair(s, t)];
    }
    return UINT_MAX;
}

//______________________________________________________________________________________________________________________
unsigned int FlagsGraphWithUnpackingData::getBackwardMiddleNode(unsigned int s, unsigned int t) {
    if (backwardUnpackingData.count(make_pair(s, t)) == 1) {
        return backwardUnpackingData[make_pair(s, t)];
    }
    return UINT_MAX;
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::setForwardPrev(unsigned int x, unsigned int y) {
    forwardPrev[x] = y;
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::setBackwardPrev(unsigned int x, unsigned int y) {
    backwardPrev[x] = y;
}

//______________________________________________________________________________________________________________________
unsigned int FlagsGraphWithUnpackingData::getForwardPrev(unsigned int x) {
    return forwardPrev[x];
}

//______________________________________________________________________________________________________________________
unsigned int FlagsGraphWithUnpackingData::getBackwardPrev(unsigned int x) {
    return backwardPrev[x];
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::resetForwardPrev(unsigned int x) {
    forwardPrev[x] = UINT_MAX;
}

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::resetBackwardPrev(unsigned int x) {
    backwardPrev[x] = UINT_MAX;
}
