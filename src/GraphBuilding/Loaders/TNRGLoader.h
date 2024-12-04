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
// Created on: 06.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRGLOADER_H
#define CONTRACTION_HIERARCHIES_TNRGLOADER_H

#include <string>
#include "../Structures/TransitNodeRoutingGraph.h"
#include "../Structures/TransitNodeRoutingGraphForPathQueries.h"


/**
 * Class used for loading the Transit Node Routing data-structure from a file.
 * I use a simple binary format for those files.
 */
template <class T = TransitNodeRoutingGraph<NodeData>>
class TNRGLoader {
protected:
    /**
     * Auxiliary function that parses the header. The file should start with the std::string "TNRG" which is used as some
     * sort of a magic number  to check the integrity of the file. Then three unsigned ints should follow
     * denoting the number of nodes, the number of edges and the size of the transit node set.
     *h
     * @param input[in] The input stream corresponding to the input file.
     * @param nodes[out] The number of nodes in the graph.
     * @param edges[out] The number of edges in the graph.
     * @param tnodesAmount[out] The size of the transit node set for the Transit Node Routing data-structure.
     */
    void parseFirstLine(
            std::ifstream& input,
            unsigned int& nodes,
            unsigned int& edges,
            unsigned int& tnodesAmount);

    /**
     * Parses edges and inserts them into the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] An instance of the graph the edges will be inserted into.
     * @param edges[in] The number of edges that need to be processed (loaded).
     */
    void parseEdgesForDistanceQueries(
            std::ifstream& input,
            T& graph,
            unsigned int edges);

    /**
     * Parses edges and inserts them into the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] An instance of the graph the edges will be inserted into.
     * @param edges[in] The number of edges that need to be processed (loaded).
     */
    void parseEdgesForPathQueries(
            std::ifstream & input,
            TransitNodeRoutingGraphForPathQueries & graph,
            unsigned int edges);

    /**
     * Parses ranks for all the nodes in the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] An instance of the graph the ranks will be loaded into.
     * @param nodes[in] The number of nodes in the graph (we need to load rank for each of the nodes).
     */
    void parseRanks(
            std::ifstream& input,
            T& graph,
            unsigned int nodes);

    /**
     * Parses the transit node mapping, meaning it will load the information about which nodes from the graph are
     * transit nodes and which nodes from have which IDs in the transit node set.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] An instance of the graph the mapping will be loaded into.
     * @param tnodesAmount[in] The size of the transit node set.
     */
    void parseTransitNodesMapping(
            std::ifstream& input,
            T& graph,
            unsigned int tnodesAmount);

    /**
     * Parses the transit node distance table that contains distances between all pairs of access nodes.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] An instance of the graph the distance table will be loaded into.
     * @param tnodesAmount[in] The size of the transit node set.
     */
    void parseTransitNodesDistanceTable(
            std::ifstream& input,
            T& graph,
            unsigned int tnodesAmount);

    /**
     * Parses access nodes. This will load access nodes in both directions for all the nodes in the graph along with
     * all their data necessary for the query algorithm.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] An instance of the graph the access nodes will be loaded into.
     * @param nodes[in] The number of nodes in the graph (we need access nodes for each node in the graph).
     */
    void parseAccessNodes(
            std::ifstream& input,
            T& graph,
            unsigned int nodes);

    /**
     * Parses search spaces. This will load search spaces in both directions for all the nodes in the graph. The search
     * spaces are used in the locality filter to determine whether a query is a local query or a global one.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] An instance of the graph the search spaces will be loaded into.
     * @param nodes[in] The number of nodes in the graph (we need search spaces for each node in the graph).
     */
    void parseSearchSpaces(
            std::ifstream& input,
            T& graph,
            unsigned int nodes);

    /**
     * Parses ranks for all the nodes in the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] An instance of the graph the ranks will be loaded into.
     * @param nodes[in] The number of nodes in the graph (we need to load rank for each of the nodes).
     */
    void parseRanks(
            std::ifstream& input,
            TransitNodeRoutingGraphForPathQueries & graph,
            unsigned int nodes);

    /**
     * Parses the transit node mapping, meaning it will load the information about which nodes from the graph are
     * transit nodes and which nodes from have which IDs in the transit node set.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] An instance of the graph the mapping will be loaded into.
     * @param tnodesAmount[in] The size of the transit node set.
     */
    void parseTransitNodesMapping(
            std::ifstream & input,
            TransitNodeRoutingGraphForPathQueries & graph,
            unsigned int tnodesAmount);

    /**
     * Parses the transit node distance table that contains distances between all pairs of access nodes.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] An instance of the graph the distance table will be loaded into.
     * @param tnodesAmount[in] The size of the transit node set.
     */
    void parseTransitNodesDistanceTable(
            std::ifstream & input,
            TransitNodeRoutingGraphForPathQueries & graph,
            unsigned int tnodesAmount);

    /**
     * Parses access nodes. This will load access nodes in both directions for all the nodes in the graph along with
     * all their data necessary for the query algorithm.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] An instance of the graph the access nodes will be loaded into.
     * @param nodes[in] The number of nodes in the graph (we need access nodes for each node in the graph).
     */
    void parseAccessNodes(
            std::ifstream & input,
            TransitNodeRoutingGraphForPathQueries & graph,
            unsigned int nodes);

    /**
     * Parses search spaces. This will load search spaces in both directions for all the nodes in the graph. The search
     * spaces are used in the locality filter to determine whether a query is a local query or a global one.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] An instance of the graph the search spaces will be loaded into.
     * @param nodes[in] The number of nodes in the graph (we need search spaces for each node in the graph).
     */
    void parseSearchSpaces(
            std::ifstream & input,
            TransitNodeRoutingGraphForPathQueries & graph,
            unsigned int nodes);

    std::string inputFile;

public:
    /**
     * A simple constructor.
     *
     * @param inputFile[in] A path towards a file that should be loaded using this loader.
     */
    explicit TNRGLoader(
            std::string inputFile);

    /**
     * Can be used to load Transit Node Routing data structures from a file given to the loader during its
     * initialization. This function provides an initialized TransitNodeRoutingGraph instance as output,
     * which can be used to answer distance queries (queries, where we only care about the shortest path
     * between two points and not about the actual shortest path).
     *
     * @return An instance of the TransitNodeRoutingGraph class than can be used to answer queries using the
     * Transit Node Routing query algorithm.
     */
    TransitNodeRoutingGraph<NodeData>* loadTNRforDistanceQueries();

    /**
     * Can be used to load Transit Node Routing data structures from a file given to the loader during its
     * initialization.
     *
     * @return An instance of the TransitNodeRoutingGraphForPathQueries class than can be used to answer path queries
     * using the Transit Node Routing query algorithm.
     */
    TransitNodeRoutingGraphForPathQueries * loadTNRforPathQueries();
};

#include "TNRGLoader.tpp"

#endif //CONTRACTION_HIERARCHIES_TNRGLOADER_H
