//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#include "FPointNodeData.h"
#include <limits>

using namespace std;

//______________________________________________________________________________________________________________________
FPointNodeData::FPointNodeData() : forwardDist(numeric_limits<double>::max()), backwardDist(numeric_limits<double>::max()), forwardReached(false), backwardReached(false), forwardSettled(false), backwardSettled(false), forwardStalled(false), backwardStalled(false) {

}