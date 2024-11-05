/******************************************************************************
* File:             DistanceMatrixInterface.h
*
* Author:           Jan Horak (horakj31@fel.cvut.cz)
*****************************************************************************/

#ifndef DISTANCEMATRIXINTERFACE_H
#define DISTANCEMATRIXINTERFACE_H

#include "../constants.h"


class DistanceMatrixInterface {
public:
    virtual ~DistanceMatrixInterface() = default;

    /**
         * This is basically a query algorithm. Each query is answered using a single table lookup,
         * as we already have all the shortest distances precomputed in the matrix.
         *
         * @param start[in] The start node for the query.
         * @param goal[in] The goal node for the query.
         * @return Returns the shortest distance from start to goal or 'std::numeric_limits<dist_t>::max()' if goal is not reachable from start.
         */
    [[nodiscard]] virtual dist_t findDistance(unsigned int start, unsigned int goal) const = 0;

    /**
         * Auxiliary function used during the initialization to set the distances.
         *
         * @param source[in] The row of the matrix we want to set the distance for.
         * @param target[in] The column of the table we want to set the distance for.
         * @param distance[in] The value (distance) we wnat to put into the table.
         */
    virtual void setDistance(unsigned int source, unsigned int target, dist_t distance) = 0;
};



#endif //DISTANCEMATRIXINTERFACE_H
