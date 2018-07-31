//
// Author: Xenty (Michal Cvach)
// Created on: 30.7.18
//

#include <climits>
#include <cstdio>
#include "Grid.h"

//______________________________________________________________________________________________________________________
void Grid::printSomeGridInfo() {
    printf("~~~ GRID INFO: ~~~\n");
    printf("Working with %u x %u grid (%u cells)\n", cells, cells, cells*cells);
    unsigned int nodesVerification = 0;
    for (unsigned int i = 0; i < cells; i++) {
        for (unsigned int j = 0; j < cells; j++) {
            printf("Cell %u x %u (%u): %lu nodes\n", i, j, cells*i+j, cellToNodes.at(i).at(j).size());
            nodesVerification += cellToNodes.at(i).at(j).size();
        }
    }
    printf("In total %u nodes\n", nodesVerification);
    printf("~~~ END OF GRID INFO ~~~\n");
}

//______________________________________________________________________________________________________________________
Grid::Grid(const unsigned int x) : cells(x) {
    maxcoord1 = INT_MIN;
    maxcoord2 = INT_MIN;
    mincoord1 = INT_MAX;
    mincoord2 = INT_MAX;
}

//______________________________________________________________________________________________________________________
void Grid::createGridFromNodes(vector < pair < int, int > > & x) {
    unsigned int nodes = x.size();
    nodesToGrid.resize(nodes);
    cellToNodes.resize(cells);
    for (unsigned int i = 0; i < cells; i++) {
        cellToNodes.at(i).resize(cells);
    }

    prepareCellSizes();

    for (unsigned int i = 0; i < nodes; i++) {
        unsigned int cellRow, cellColumn;
        computeCell(x.at(i).first, x.at(i).second, cellRow, cellColumn);
        nodesToGrid.at(i).first = cellRow;
        nodesToGrid.at(i).second = cellColumn;
        cellToNodes.at(cellRow).at(cellColumn).push_back(i);
    }
}

//______________________________________________________________________________________________________________________
void Grid::computeCell(const int coord1, const int coord2, unsigned int & cellRow, unsigned int & cellColumn) {
    double relativeCoord1 = ((double) coord1) - lowestCoord1;
    double relativeCoord2 = ((double) coord2) - lowestCoord2;
    cellRow = (unsigned int) (relativeCoord1 / difference);
    cellColumn = (unsigned int) (relativeCoord2 / difference);
    //printf("Computed cell for a node: %u x %u, from coords %i, %i\n", cellRow, cellColumn, coord1, coord2);
    //printf("Maxcoords are: %i, %i\n", maxcoord1, maxcoord2);
}

//______________________________________________________________________________________________________________________
void Grid::prepareCellSizes() {
    double middleCoord1 = ((double) (mincoord1 + maxcoord1)) / 2;
    double middleCoord2 = ((double) (mincoord2 + maxcoord2)) / 2;
    double distance1 = (double) (maxcoord1 - mincoord1);
    double distance2 = (double) (maxcoord2 - mincoord2);
    double maxdistance = (distance1 > distance2) ? distance1 : distance2;
    maxdistance += 2;
    double halfmaxdistance = maxdistance / 2;
    lowestCoord1 = middleCoord1 - halfmaxdistance;
    lowestCoord2 = middleCoord2 - halfmaxdistance;
    difference = maxdistance / cells;
}


//______________________________________________________________________________________________________________________
int Grid::getMincoord1() {
    return mincoord1;
}

//______________________________________________________________________________________________________________________
int Grid::getMincoord2() {
    return mincoord2;
}

//______________________________________________________________________________________________________________________
int Grid::getMaxcoord1() {
    return maxcoord1;
}

//______________________________________________________________________________________________________________________
int Grid::getMaxcoord2() {
    return maxcoord2;
}

//______________________________________________________________________________________________________________________
void Grid::setMincoord1(const int x) {
    mincoord1 = x;
}

//______________________________________________________________________________________________________________________
void Grid::setMincoord2(const int x) {
    mincoord2 = x;
}

//______________________________________________________________________________________________________________________
void Grid::setMaxcoord1(const int x) {
    maxcoord1 = x;
}

//______________________________________________________________________________________________________________________
void Grid::setMaxcoord2(const int x) {
    maxcoord2 = x;
}