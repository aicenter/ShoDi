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

//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#ifndef CONTRACTION_HIERARCHIES_DISTANCEMATRIX_H
#define CONTRACTION_HIERARCHIES_DISTANCEMATRIX_H

#include <vector>
#include <string>
#include <memory>

#include "../constants.h"
#include "DistanceMatrixInterface.h"
#include "../GraphBuilding/Loaders/GraphLoader.h"



/**
 * Represents a Distance Matrix for a given graph. The Distance Matrix is simply a n*n table ('n' is the number of nodes
 * in the graph) containing precomputed distances between all pairs of nodes. The Distance Matrix is the fastest way
 * to obtain shortest paths, as each query is then only a simple table lookup. The price for this is the memory
 * complexity, which is really high (n^2). This means, that for bigger graphs, we are not able to store the Distance
 * Matrix in memory at once, making it impossible to use it. Here, the Distance Matrix is used for comparison to get an
 * idea of how much slower the other methods are in comparison with the Distance Matrix approach.
 */
template <class IntType>
class Distance_matrix_travel_time_provider : public DistanceMatrixInterface {
public:
    /**
     * A simple constructor for a square empty matrix.
     *
     * @param nodes[in] The number of nodes (rows and columns) in the square matrix.
     */
    explicit Distance_matrix_travel_time_provider(unsigned int nodes);

    /**
     * A simple constructor for an empty matrix.
     *
     * @param num_rows[in] The number of rows in the matrix.
     * @param num_cols[in] The number of columns in the matrix.
     */
    explicit Distance_matrix_travel_time_provider(unsigned int num_rows, unsigned int num_cols);

	/**
	 * Constructor that takes a pre-filled 1D distance matrix (assumes square matrix from size).
	 *
	 * @param distMatrix[in] A 1D std::unique_ptr<IntType[]> representing the distances.
	 * @param num_rows[in] The number of rows in the matrix.
	 * @param num_cols[in] The number of columns in the matrix.
	 */
	explicit Distance_matrix_travel_time_provider(std::unique_ptr<IntType[]> distMatrix, unsigned int num_rows, unsigned int num_cols);

    /**
     * Constructor that takes a pre-filled 1D distance matrix (assumes square matrix from size).
     *
     * @param distMatrix[in] A 1D std::unique_ptr<IntType[]> representing the distances.
     * @param size[in] The number of rows and columns (size x size matrix).
     */
    explicit Distance_matrix_travel_time_provider(std::unique_ptr<IntType[]> distMatrix, unsigned int size);

    /**
     * Constructor that takes a 2D std::vector of distances (e.g., from TNRAFPreprocessor)
     * and converts it to the internal 1D representation.
     * The transit_node_ids_for_columns vector is used to determine the number of columns.
     *
     * @param inputMatrix[in] Pointer to a 2D std::vector holding the distances.
     *                        inputMatrix[row_idx][col_idx] is the distance.
     * @param num_rows[in] The number of rows (typically total graph nodes).
     * @param transit_node_ids_for_columns[in] Vector of original transit node IDs.
     *                                         Its size determines the number of columns.
     */
    // explicit Distance_matrix_travel_time_provider(
    //     std::vector<std::vector<IntType>>* inputMatrix,
    //     unsigned int num_rows,
    //     const std::vector<unsigned int>& transit_node_ids_for_columns);


    explicit Distance_matrix_travel_time_provider(bool fast, GraphLoader& graphLoader, int scaling_factor);

    /**
     * This is basically a query algorithm.
     *
     * @param start_row[in] The start row index for the query.
     * @param goal_col[in] The goal column index for the query.
     * @return Returns the shortest distance or 'std::numeric_limits<dist_t>::max()'.
     */
    [[nodiscard]] dist_t findDistance(unsigned int start_row, unsigned int goal_col) const override;

    /**
     * Auxiliary function used during initialization to set the distances.
     *
     * @param source_row[in] The row of the matrix.
     * @param target_col[in] The column of the matrix.
     * @param distance[in] The value (distance) to put into the table.
     */
    void setDistance(unsigned int source_row, unsigned int target_col, dist_t distance) override;

    /**
     * Get the underlying data structure (a 1D array)
     * @return The underlying 1D array
     */
    const std::unique_ptr<IntType[]>& getRawData();

    /**
     * Get rows count
     * @return rows count
     */
    [[nodiscard]] unsigned int getRows() const;

    /**
     * Get columns count
     * @return columns count
     */
    [[nodiscard]] unsigned int getCols() const;

    /**
     * Returns the size of the matrix if it is square (rows == cols).
     * Throws a std::runtime_error if the matrix is not square.
     * @return The number of rows/columns if the matrix is square.
     */
    [[nodiscard]] unsigned int nodes() const;

    /**
     * Prints some statistics about the distance matrix.
     */
    void printInfo();

private:
    const unsigned int rowsCnt;
    const unsigned int colsCnt;
    std::unique_ptr<IntType[]> distances;

   /**
    *
    *
    * @param fast[in] Specifies whether to use fast computing algorithm or not.
    * @param graphLoader[in] instance of GraphLoader that will load the data for which we want to compute
    * the distance matrix.
    * @param scaling_factor[in] This parameter allows us to lose some precision
    * of the weight values. Each loaded weight will be divided by this value before rounding.
    * @param timerName[in] Name of the timer to be used for the computation
    */
    void computeDistanceMatrix(bool fast, GraphLoader& graphLoader, int scaling_factor, const std::string& timerName);
};

#include "Distance_matrix_travel_time_provider.tpp"

#endif //CONTRACTION_HIERARCHIES_DISTANCEMATRIX_H
