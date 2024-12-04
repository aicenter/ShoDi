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
* File:             DistanceMatrixComputorFast.h
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/

#ifndef SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H
#define SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H

#include <vector>
#include "Distance_matrix_travel_time_provider.h"
#include "DistanceMatrixComputor.h"


template <class IntType>
class DistanceMatrixComputorFast : public DistanceMatrixComputor<IntType> {
public:

    std::vector<dist_t> loadGraph(GraphLoader &graphLoader, int scaling_factor);

    std::unique_ptr<IntType[]> compute_and_get_distance_matrix(GraphLoader& graphLoader, int scaling_factor) override;

    void computeDistanceMatrix(const std::vector<dist_t>& graphAdjMatrix);
};

#include "DistanceMatrixComputorFast.tpp"

#endif //SHORTEST_PATHS_DISTANCEMATRIXCOMPUTORFAST_H
