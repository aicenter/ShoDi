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
template <class IntType>
class DistanceMatrixXdmOutputter : public DistanceMatrixOutputter<IntType> {
public:
    void store(Distance_matrix_travel_time_provider<IntType>& dm, const std::string &path) override;
};

#include "DistanceMatrixXdmOutputter.tpp"

#endif //SHORTEST_PATHS_DISTANCEMATRIXXDMOUTPUTTER_H
