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
 * File:             DistanceMatrixComputor.h
 *
 * Author:           Jan Neumann (neumaja5@fel.cvut.cz)
 * Created:          07/28/20
 * Description:      DistanceMatrixComputor abstract class
 *****************************************************************************/

#ifndef SHORTEST_PATHS_DISTANCEMATRIXCOMPUTOR_H
#define SHORTEST_PATHS_DISTANCEMATRIXCOMPUTOR_H

#include "../GraphBuilding/Loaders/GraphLoader.h"
#include <memory>

/**
 * This class can compute a full distance matrix for a given Graph. Meaning all
 * possible pairs of source -> target distances will be computed and stored.
 * Finding a shortest path using a distance matrix is the fastest possible
 * option, since each query is then answered using a single table lookup. The
 * memory requirements are pretty high though, so this can not be used all the
 * time. This particular class uses 'n' runs of Dijkstra to compute the full
 * distance matrix (where 'n' is the number of nodes). This should be fast
 * enough for small graphs especially since the road graphs are usually very
 * sparse.
 */
template<typename IntType>
class DistanceMatrixComputor {
public:
    /**
     * Allows us to get a DistanceMatrix instance immediately from the
     * DistanceMatrixComputor without the need to first save it into a file and
     * then load it. This gives you an instance of the DistanceMatrix class that
     * can immediately be used to answer queries.
     *
     * @return An instance of the DistanceMatrix class that can be used to answer
     * queries.
     */
    std::unique_ptr<IntType[]> getDistanceMatrixInstance() {
        return std::move(distanceTable);
    }

    virtual ~DistanceMatrixComputor() = default;

    virtual std::unique_ptr<IntType[]> compute_and_get_distance_matrix(GraphLoader& graphLoader, int scaling_factor) = 0;

    /*Distance_matrix_travel_time_provider<IntType>* compute_and_get_distance_matrix(GraphLoader &graphLoader) {
        return compute_and_get_distance_matrix(graphLoader, 1);
    }*/
protected:
    std::unique_ptr<IntType[]> distanceTable;
    unsigned int size;
};

#endif // SHORTEST_PATHS_DISTANCEMATRIXCOMPUTOR_H
