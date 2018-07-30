//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_MINMAXCOORDSFINDER_H
#define TRANSIT_NODE_ROUTING_MINMAXCOORDSFINDER_H

#include <vector>
#include "../TNR/Grid.h"

using namespace std;

class MinMaxCoordsFinder {
public:
    static void setMinMaxCoords(vector < pair < int, int > > & x, Grid & grid);
private:
};


#endif //TRANSIT_NODE_ROUTING_MINMAXCOORDSFINDER_H
