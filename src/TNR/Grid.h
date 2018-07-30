//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_GRID_H
#define TRANSIT_NODE_ROUTING_GRID_H


class Grid {
public:
    Grid();
    int getMincoord1();
    int getMincoord2();
    int getMaxcoord1();
    int getMaxcoord2();
    void setMincoord1(const int x);
    void setMincoord2(const int x);
    void setMaxcoord1(const int x);
    void setMaxcoord2(const int x);
private:
    int mincoord1;
    int mincoord2;
    int maxcoord1;
    int maxcoord2;
};


#endif //TRANSIT_NODE_ROUTING_GRID_H
