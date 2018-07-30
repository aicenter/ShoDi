//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#include <climits>
#include "MinMaxCoordsFinder.h"

void MinMaxCoordsFinder::setMinMaxCoords(vector < pair < int, int > > & x, Grid & grid) {
    int maxcoord1 = INT_MIN;
    int maxcoord2 = INT_MIN;
    int mincoord1 = INT_MAX;
    int mincoord2 = INT_MAX;
    for (unsigned int i = 0; i < x.size(); i++ ) {
        if (x.at(i).first < mincoord1) {
            mincoord1 = x.at(i).first;
        }
        if (x.at(i).first > maxcoord1) {
            maxcoord1 = x.at(i).first;
        }
        if (x.at(i).second < mincoord2) {
            mincoord2 = x.at(i).second;
        }
        if (x.at(i).second > maxcoord2) {
            maxcoord2 = x.at(i).second;
        }
    }

    grid.setMincoord1(mincoord1);
    grid.setMaxcoord1(maxcoord1);
    grid.setMincoord2(mincoord2);
    grid.setMaxcoord2(maxcoord2);

}