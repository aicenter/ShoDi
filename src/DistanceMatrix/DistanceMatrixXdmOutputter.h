/******************************************************************************
* File:             DistanceMatrixXdmOutputter.h
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/

#ifndef SHORTEST_PATHS_DISTANCEMATRIXXDMOUTPUTTER_H
#define SHORTEST_PATHS_DISTANCEMATRIXXDMOUTPUTTER_H


#include "DistanceMatrixOutputter.h"

/**
 * Allows to store the distance matrix to disk in a simple binary format. The distance matrix will be saved into a file
 * specified by the path argument, suffix '.xdm' is added automatically.
 *
 * The format is as follows: first three bytes should contain the characters 'x', 'd' and 'm' respectively,
 * following is one unsigned int denoting the number of nodes 'n', and following are n*n unsigned int values
 * representing the actual distances for the matrix.
 *
 */
class DistanceMatrixXdmOutputter : public DistanceMatrixOutputter {
public:
    void store(DistanceMatrix &dm, const string &path) override;
};


#endif //SHORTEST_PATHS_DISTANCEMATRIXXDMOUTPUTTER_H
