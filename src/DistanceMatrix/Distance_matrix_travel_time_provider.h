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
     * A simple constructor.
     *
     * @param nodes[in] The number of nodes in the graph (also the number of rows and columns in the matrix).
     */
    explicit Distance_matrix_travel_time_provider(unsigned int nodes);

    /**
     * A move constructor.
     *
     * @param distMatrix[in] A 2D std::vector that will be used as the distance matrix.
     */
    explicit Distance_matrix_travel_time_provider(std::unique_ptr<IntType[]> distMatrix, unsigned int size);

    explicit Distance_matrix_travel_time_provider(bool fast, GraphLoader& graphLoader, int scaling_factor);

    /**
     * This is basically a query algorithm. Each query is answered using a single table lookup,
     * as we already have all the shortest distances precomputed in the matrix.
     *
     * @param start[in] The start node for the query.
     * @param goal[in] The goal node for the query.
     * @return Returns the shortest distance from start to goal or 'std::numeric_limits<dist_t>::max()' if goal is not reachable from start.
     */
    [[nodiscard]] dist_t findDistance(unsigned int start, unsigned int goal) const override;

    /**
     * Auxiliary function used during the initialization to set the distances.
     *
     * @param source[in] The row of the matrix we want to set the distance for.
     * @param target[in] The column of the table we want to set the distance for.
     * @param distance[in] The value (distance) we wnat to put into the table.
     */
    void setDistance(unsigned int source, unsigned int target, dist_t distance) override;

    /**
     * Get the underlying data structure (a 1D array)
     * @return The underlying 1D array
     */
    const std::unique_ptr<IntType[]>& getRawData();

    /**
     * Get nodes count
     * @return nodes count
     */
    [[nodiscard]] unsigned int nodes() const;

    /**
     * Prints some statistics about the distance matrix. Useful mainly during debugging, might be removed later.
     * Checks how many values in the distance matrix are set to infinity (represented as 'std::numeric_limits<dist_t>::max()' in our case).
     * Too many of such values are suspicious and might mean that for example the precision of the graph is too
     * big and the values do not fit into unsigned int anymore.
     */
    void printInfo();

private:
    const unsigned int nodesCnt;
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
