/******************************************************************************
* File:             DistanceMatrixHdfOutputter.h
*
* Author:           Jan Horak (horakj31@fel.cvut.cz)
* Created:          03/26/24
*****************************************************************************/

#ifndef SHORTEST_PATHS_DISTANCEMATRIXHDFOUTPUTTER_H
#define SHORTEST_PATHS_DISTANCEMATRIXHDFOUTPUTTER_H

#include "DistanceMatrixOutputter.h"

/**
 * Allows to store the distance matrix to disk in HDF5 format.
 * The distance matrix will be saved into a file specified by the path argument, suffix '.hdf5' is added automatically.
 */
template <class IntType>
class DistanceMatrixHdfOutputter : public DistanceMatrixOutputter<IntType> {

public:
    void store(Distance_matrix_travel_time_provider<IntType>& dm, const std::string &path) override;
};

#include "DistanceMatrixHdfOutputter.tpp"

#endif //SHORTEST_PATHS_DISTANCEMATRIXHDFOUTPUTTER_H
