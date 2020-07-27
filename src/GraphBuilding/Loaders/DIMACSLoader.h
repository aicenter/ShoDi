//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#ifndef TRANSIT_NODE_ROUTING_LOADER_H
#define TRANSIT_NODE_ROUTING_LOADER_H

#include <string>
#include <map>
#include "../Structures/Graph.h"
#include "../Structures/UpdateableGraph.h"
#include "GraphLoader.h"

using namespace std;

/**
 * This class is responsible for loading input files in the DIMACS format. This means either loading the graph into
 * an instance of Graph, which can be used with the Dijkstra algorithm, or an instance of UpdateableGraph,
 * from which the Contraction Hierarchy can be created.
 *
 * By DIMACS format I mean the 9th DIMACS Implementation Challenge format. Information about the format along with some
 * sample graphs that can be directly used with this program can be found here:
 * http://www.dis.uniroma1.it/challenge9/download.shtml
 */
class DIMACSLoader : public GraphLoader {
private:
    string inputFile;

    /**
     * Auxiliary function used to parse the problem line of the input file (first line that is not a comment).
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param nodes[out] The number of nodes in the graph.
     * @param edges[out] The number of edges in the graph.
     */
    void parseGraphProblemLine(ifstream & input, unsigned int & nodes, unsigned int & edges);

    /**
     Auxiliary function used to parse the edges when loading the graph.
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
     * Auxiliary function that extracts the number of nodes and the number of edges from the problem line.
     *
     * @param buffer[in] A buffer containing the problem line.
     * @param nodes[out] The number of nodes in the graph.
     * @param edges[out] The number of edges in the graph.
     */
    void processGraphProblemLine(string & buffer, unsigned int & nodes, unsigned int & edges);

    /**
     * Extracts an edge data from an input line representing an edge.
     *
     * @param buffer[in] A buffer containing the edge line.
     * @param from[out] The source node of the edge.
     * @param to[out] The target node of the edge.
     * @param weight[out] The weight of the edge.
     */
    void getEdge(string & buffer, unsigned int & from, unsigned int & to, unsigned int & weight);

public:
    /**
     * A simple constructor.
     *
     * @param inputFile[in] A path towards a file that should be loaded using this loader.
     */
    explicit DIMACSLoader(string inputFile);

    vector<int> loadAdjacencyMatrix() override;

    Graph *loadGraph() override;

    UpdateableGraph *loadUpdateableGraph() override;

    void putAllEdgesIntoUpdateableGraph(UpdateableGraph &graph) override;

    ~DIMACSLoader() override = default;
};

#endif //TRANSIT_NODE_ROUTING_LOADER_H
