//
// Author: Xenty (Michal Cvach)
// Created on: 30.8.18
//

#include <climits>
#include "IntegerFlagsGraphWithUnpackingData.h"

IntegerFlagsGraphWithUnpackingData::IntegerFlagsGraphWithUnpackingData(unsigned int n) : IntegerFlagsGraph(n) {
    forwardPrev.resize(n, UINT_MAX);
    backwardPrev.resize(n, UINT_MAX);
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::addForwardUnpackingData(unsigned int s, unsigned int t, unsigned int m) {
    forwardUnpackingData.insert(make_pair(make_pair(s,t), m));
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::addBackwardUnpackingData(unsigned int s, unsigned int t, unsigned int m) {
    backwardUnpackingData.insert(make_pair(make_pair(s,t), m));
}


//______________________________________________________________________________________________________________________
unsigned int IntegerFlagsGraphWithUnpackingData::getForwardMiddleNode(unsigned int s, unsigned int t) {
    if (forwardUnpackingData.count(make_pair(s, t)) == 1) {
        return forwardUnpackingData[make_pair(s, t)];
    }
    return UINT_MAX;
}

//______________________________________________________________________________________________________________________
unsigned int IntegerFlagsGraphWithUnpackingData::getBackwardMiddleNode(unsigned int s, unsigned int t) {
    if (backwardUnpackingData.count(make_pair(s, t)) == 1) {
        return backwardUnpackingData[make_pair(s, t)];
    }
    return UINT_MAX;
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::setForwardPrev(unsigned int x, unsigned int y) {
    forwardPrev[x] = y;
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::setBackwardPrev(unsigned int x, unsigned int y) {
    backwardPrev[x] = y;
}

//______________________________________________________________________________________________________________________
unsigned int IntegerFlagsGraphWithUnpackingData::getForwardPrev(unsigned int x) {
    return forwardPrev[x];
}

//______________________________________________________________________________________________________________________
unsigned int IntegerFlagsGraphWithUnpackingData::getBackwardPrev(unsigned int x) {
    return backwardPrev[x];
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::resetForwardPrev(unsigned int x) {
    forwardPrev[x] = UINT_MAX;
}

//______________________________________________________________________________________________________________________
void IntegerFlagsGraphWithUnpackingData::resetBackwardPrev(unsigned int x) {
    backwardPrev[x] = UINT_MAX;
}
