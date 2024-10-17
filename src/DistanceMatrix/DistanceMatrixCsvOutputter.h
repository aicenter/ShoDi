/******************************************************************************
* File:             DistanceMatrixCsvOutputter.h
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/

#ifndef SHORTEST_PATHS_DISTANCEMATRIXCSVOUTPUTTER_H
#define SHORTEST_PATHS_DISTANCEMATRIXCSVOUTPUTTER_H


#include "DistanceMatrixOutputter.h"

/**
 * Allows to store the distance matrix to disk in a CSV format. Comma is used as delimiter.
 * The distance matrix will be saved into a file specified by the path argument, suffix '.csv' is added automatically.
 */
template <class IntType>
class DistanceMatrixCsvOutputter : public DistanceMatrixOutputter<IntType> {
public:
    void store(Distance_matrix_travel_time_provider<IntType>& dm, const std::string &path) override;
};

#include "DistanceMatrixCsvOutputter.tpp"

#endif //SHORTEST_PATHS_DISTANCEMATRIXCSVOUTPUTTER_H
