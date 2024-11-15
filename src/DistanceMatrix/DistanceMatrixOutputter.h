/******************************************************************************
* File:             DistanceMatrixOutputter.h
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/

#ifndef SHORTEST_PATHS_DISTANCEMATRIXOUTPUTTER_H
#define SHORTEST_PATHS_DISTANCEMATRIXOUTPUTTER_H

#include <string>
#include "Distance_matrix_travel_time_provider.h"

/**
 * Allows to store the distance matrix to disk.
 */
template <class IntType>
class DistanceMatrixOutputter {

public:
 virtual ~DistanceMatrixOutputter() = default;

 /**
 * Store the distance matrix to disk. The distance matrix will be saved into a file
 * specified by the path argument. The appropriate file extension is added automatically.
 *
 * @param dm[in] The distance matrix to store.
 * @param path[in] The desired output file path where the distance matrix should be output.
 */
virtual void store(Distance_matrix_travel_time_provider<IntType>& dm, const std::string &path) = 0;
};


#endif //SHORTEST_PATHS_DISTANCEMATRIXOUTPUTTER_H
