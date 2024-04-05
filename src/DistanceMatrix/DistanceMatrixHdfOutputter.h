/******************************************************************************
* File:             DistanceMatrixHdfOutputter.h
*
* Author:           Jan Horák (horakj31@fel.cvut.cz)  
* Created:          03/26/24
*****************************************************************************/

#ifndef SHORTEST_PATHS_DISTANCEMATRIXHDFOUTPUTTER_H
#define SHORTEST_PATHS_DISTANCEMATRIXHDFOUTPUTTER_H


#include "DistanceMatrixOutputter.h"

/**
 * Allows to store the distance matrix to disk in a CSV format. Comma is used as delimiter. #TODO
 * The distance matrix will be saved into a file specified by the path argument, suffix '.csv' is added automatically.
 */
class DistanceMatrixHdfOutputter : public DistanceMatrixOutputter {
public:
    void store(Distance_matrix_travel_time_provider &dm, const std::string &path) override;
};


#endif //SHORTEST_PATHS_DISTANCEMATRIXHDFOUTPUTTER_H
