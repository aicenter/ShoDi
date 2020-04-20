//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#ifndef CONTRACTION_HIERARCHIES_XENGRAPHLOADER_H
#define CONTRACTION_HIERARCHIES_XENGRAPHLOADER_H

#include <string>
#include <map>
#include "../Structures/Graph.h"
#include "../Structures/SimpleGraph.h"
#include "../Structures/UpdateableGraph.h"

/**
 * Loader used for loading integer graphs in the XenGraph format. The format is a really simple text format and is
 * described in the README.md of this project. This loader allows the loading into an Graph, which can be used
 * for the Dijkstra implementation, UpdateableGraph, which is used to create a Contraction Hierarchy,
 * and additionally also can read the XenGraph indices mapping files, which must be used when the user wants to query
 * using the original indices from the .geojson files.
 */
class XenGraphLoader {
private:
    string inputFile;

    /**
     * Auxiliary function used to parse the first (header) line of the input file.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param nodes[out] The number of nodes in the graph.
     * @param edges[out] The number of edges in the graph.
     */
    void parseFirstLine(ifstream & input, unsigned int & nodes, unsigned int & edges);

    /**
     * Auxiliary function used to parse the edges when loading the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] The graph instance that the edges will be inserted into.
     * @param edges[in] The number of edges that need to be loaded.
     */
    void parseEdges(ifstream & input, SimpleGraph & graph, unsigned int edges);

    /**
     * Auxiliary function used to parse the edges when loading the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] The graph instance that the edges will be inserted into.
     * @param edges[in] The number of edges that need to be loaded.
     */
    void parseEdges(ifstream & input, UpdateableGraph & graph, unsigned int edges);

    /**
     * Auxiliary function used to parse the mapping.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param mapping[out] An unordered map that will be filled with the mapping data.
     */
    void parseNodesMapping(ifstream & input, unordered_map <long long unsigned int, unsigned int> & mapping);

public:
    /**
     * A simple constructor.
     *
     * @param inputFile[in] A path towards a file that should be loaded using this loader.
     */
    explicit XenGraphLoader(string inputFile);

    /**
     * Function used to load a graph for the Dijkstra's algorithm. One minor improvement is that we first load the edges
     * into an 'SimpleGraph' instance, which automatically removes multiple (parallel) edges and then construct an
     * 'Graph' from that.
     *
     * @return Returns an instance of the Graph class containing the graph loaded from the file. The graph will not
     * contain any parallel edges.
     */
    Graph * loadGraph();

    /**
     * Function used to load a graph in a way that is practical for preprocessing using one of the methods.
     *
     * @return Returns an instance of the UpdateableGraph class containing the graph loaded from the file.
     * The graph will not contain any parallel edges.
     */
    UpdateableGraph * loadUpdateableGraph();

    /**
     * This function is used to reinsert all the original edges into the graph after the preprocessing has been
     * finished. Since during preprocessing edges are deleted during the contraction process, this puts the original
     * edges back into the graph.
     *
     * @param graph[in, out] An existing graph we will insert the edges into.
     */
    void putAllEdgesIntoUpdateableGraph(UpdateableGraph & graph);

    /**
     * This function can be used to load a mapping from the original IDs to IDs used by the query algorithms.
     * This is needed when for example using the query algorithms from other programming languages where some other
     * node IDs are used for some reason.
     *
     * @param mapping[out] An unordered map that will be filled with the mapping data.
     */
    void loadNodesMapping(unordered_map <long long unsigned int, unsigned int> & mapping);
};


#endif //CONTRACTION_HIERARCHIES_XENGRAPHLOADER_H
