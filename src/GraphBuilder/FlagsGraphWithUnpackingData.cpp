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
void FlagsGraphWithUnpackingData::addUnpackingData(unsigned int s, unsigned int t, unsigned int m) {
    unpackingData.insert(make_pair(make_pair(s,t), m));
}

//______________________________________________________________________________________________________________________
unsigned int FlagsGraphWithUnpackingData::getMiddleNode(unsigned int s, unsigned int t) {
    if (unpackingData.count(make_pair(s, t)) == 1) {
        return unpackingData[make_pair(s, t)];
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

//______________________________________________________________________________________________________________________
void FlagsGraphWithUnpackingData::debugPrint() {
    printf("This graph has %lu middle node records!\n", unpackingData.size());
}