//
// Author: Xenty (Michal Cvach)
// Created on: 29.8.18
//

#include <climits>
#include "NodeData.h"

//______________________________________________________________________________________________________________________
NodeData::NodeData() : forwardDist(ULLONG_MAX), backwardDist(ULLONG_MAX), forwardReached(false), backwardReached(false), forwardSettled(false), backwardSettled(false), forwardStalled(false), backwardStalled(false) {

}