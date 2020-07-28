//
// Created by Jan Neumann (neumaja5@fel.cvut.cz) on 27.07.20.
//

#ifndef SHORTEST_PATHS_GRAPHLOADER_H
#define SHORTEST_PATHS_GRAPHLOADER_H


#include "../Structures/Graph.h"
#include "../Structures/UpdateableGraph.h"

class GraphLoader {
public:
    /**
     * Function used to load a graph for the Dijkstra's algorithm. One minor improvement is that we first load the edges
     * into an 'SimpleGraph' instance, which automatically removes multiple (parallel) edges and then construct an
     * 'Graph' from that.
     *
     * @return Returns an instance of the Graph class containing the graph loaded from the file. The graph will not
     * contain any parallel edges.
     */
    virtual Graph *loadGraph() = 0;

    /**
     * Function used to load a graph in the form of an adjacency matrix.
     * Values for edges that are not present are expected to be INT_MAX.
     */
    virtual vector<int> loadAdjacencyMatrix() = 0;

    /**
     * Function used to load a graph in a way that is practical for preprocessing using one of the methods.
     *
     * @return Returns an instance of the UpdateableGraph class containing the graph loaded from the file.
     * The graph will not contain any parallel edges.
     */
    virtual UpdateableGraph *loadUpdateableGraph() = 0;

    /**
     * This function is used to reinsert all the original edges into the graph after the preprocessing has been
     * finished. Since during preprocessing edges are deleted during the contraction process, this puts the original
     * edges back into the graph.
     *
     * @param graph[in, out] An existing graph we will insert the edges into.
     */
    virtual void putAllEdgesIntoUpdateableGraph(UpdateableGraph &graph) = 0;

    virtual ~GraphLoader() = default;
};


#endif //SHORTEST_PATHS_GRAPHLOADER_H
