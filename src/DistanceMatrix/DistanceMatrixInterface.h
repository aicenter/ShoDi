/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

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
