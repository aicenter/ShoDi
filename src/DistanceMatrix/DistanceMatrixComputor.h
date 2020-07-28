//
// Created by Jan Neumann (neumaja5@fel.cvut.cz) on 28.07.20.
//

#ifndef SHORTEST_PATHS_DISTANCEMATRIXCOMPUTOR_H
#define SHORTEST_PATHS_DISTANCEMATRIXCOMPUTOR_H


#include "../GraphBuilding/Loaders/GraphLoader.h"
#include "DistanceMatrix.h"

/**
 * This class can compute a full distance matrix for a given Graph. Meaning all possible pairs
 * of source -> target distances will be computed and stored. Finding a shortest path using a distance matrix is the
 * fastest possible option, since each query is then answered using a single table lookup. The memory requirements
 * are pretty high though, so this can not be used all the time. This particular class uses 'n' runs of Dijkstra
 * to compute the full distance matrix (where 'n' is the number of nodes). This should be fast enough for small graphs
 * especially since the road graphs are usually very sparse.
 */
class DistanceMatrixComputor {
public:

    /**
     * this function will compute the full distance matrix for the graph provided by the given GraphLoader.
     * the matrix will be stored in the instance of the DistanceMatrixcomputor class,
     * so that it can be output or used further.
     *
     * @param graphLoader[in] instance of GraphLoader that will provide the graph
     * for which we want to compute the distance matrix.
     */
    virtual void computeDistanceMatrix(GraphLoader &graphLoader) = 0;

    /**
     * Allows us to get a DistanceMatrix instance immediately from the DistanceMatrixComputor without the need to first
     * save it into a file and then load it. This gives you an instance of the DistanceMatrix class that can immediately
     * be used to answer queries.
     *
     * @return An instance of the DistanceMatrix class that can be used to answer queries.
     */
    virtual DistanceMatrix * getDistanceMatrixInstance() = 0;

    virtual ~DistanceMatrixComputor() = default;
};


#endif //SHORTEST_PATHS_DISTANCEMATRIXCOMPUTOR_H
