//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#ifndef TRANSIT_NODE_ROUTING_GRID_H
#define TRANSIT_NODE_ROUTING_GRID_H

#include <vector>

using namespace std;

class Grid {
public:
    Grid(const unsigned int x = 128);
    void createGridFromNodes(vector < pair < int, int > > & x);
    int getMincoord1();
    int getMincoord2();
    int getMaxcoord1();
    int getMaxcoord2();
    void setMincoord1(const int x);
    void setMincoord2(const int x);
    void setMaxcoord1(const int x);
    void setMaxcoord2(const int x);
    void printSomeGridInfo();
private:
    void computeCell(const int coord1, const int coord2, unsigned int & cellRow, unsigned int & cellColumn);
    void prepareCellSizes();
    vector < pair <unsigned int, unsigned int > > nodesToGrid;
    vector < vector < vector < unsigned int > > > cellToNodes;
    int mincoord1;
    int mincoord2;
    int maxcoord1;
    int maxcoord2;
    const unsigned int cells;
    double cellSize;
    double lowestCoord1;
    double lowestCoord2;
    double difference;
};


#endif //TRANSIT_NODE_ROUTING_GRID_H
