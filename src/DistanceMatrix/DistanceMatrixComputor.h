//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#ifndef CONTRACTION_HIERARCHIES_DISTANCEMATRIXCOMPUTOR_H
#define CONTRACTION_HIERARCHIES_DISTANCEMATRIXCOMPUTOR_H

#include "../GraphBuilding/Structures/Graph.h"
#include "DistanceMatrix.h"

using namespace std;

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
     * This function will compute the full distance matrix for the given graph.
     * The matrix will be stored in the instance of the DistanceMatrixComputor class,
     * so that it can be output or used further.
     *
     * @param graph[in] The graph for which we want to compute the distance matrix.
     */
    void computeDistanceMatrix(const Graph & graph);

    /**
     * This function will compute the full distance matrix for the given graph as if the directions for all the edges
     * were switched. This is useful during the preprocessing phase of some of the methods.
     *
     * @param graph[in] The graph for which we want to compute the distance matrix. (The graph should be as normal,
     * not reversed, the 'reversion' is done inside this function.)
     */
    void computeDistanceMatrixInReversedGraph(const Graph & graph);

    /**
     * Allows us to get a DistanceMatrix instance immediately from the DistanceMatrixComputor without the need to first
     * save it into a file and then load it. This gives you an instance of the DistanceMatrix class that can immediately
     * be used to answer queries.
     *
     * @return An instance of the DistanceMatrix class that can be used to answer queries.
     */
    DistanceMatrix * getDistanceMatrixInstance();

private:
    /**
     * // This function will compute one row of the full distance matrix. This is done by running a simple Dijkstra from the
// node corresponding to the row, which is not stopped until all reachable nodes have been visited. The distances
// to all the other nodes found by this Dijkstra run are then used as values for the row.
     *
     * @param rowID
     * @param nodesCnt
     * @param graph
     * @param useReversedGraph
     */
    void fillDistanceMatrixRow(unsigned int rowID, unsigned int nodesCnt, const Graph & graph, bool useReversedGraph = false);

    vector<int> distanceTable;
};


#endif //CONTRACTION_HIERARCHIES_DISTANCEMATRIXCOMPUTOR_H
