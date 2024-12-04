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
