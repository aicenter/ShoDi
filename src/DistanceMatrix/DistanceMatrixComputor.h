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
    //typedef T GraphType;

    /**
     * This function will retrieve the graph data required for the
     * distance matrix computation algorithm using a GraphLoader.
     *
     * @param graphLoader[in] instance of GraphLoader that will load the data
     * for which we want to compute the distance matrix.
     * @param[in] scaling_factor This parameter allows us to lose some precision
     * of the weight values. Each loaded weight will be divided by this value
     * before rounding.
     */
    //virtual void loadGraph(GraphLoader &graphLoader, int scaling_factor) = 0;

    /**
     * This function will compute the full distance matrix for the graph data
     * loaded using `DistanceMatrixComputor::loadGraph`. The computed distance
     * matrix will be stored in the DistanceMatrixComputor instance, so that it
     * can be used further.
     *
     * @param[in] graphData Graph data required for the distance matrix
     * calculation
     */
    //virtual void computeDistanceMatrix(const T &graphData) = 0;

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
