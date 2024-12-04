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
// Created on: 25.7.18
//

#ifndef TRANSIT_NODE_ROUTING_LOADER_H
#define TRANSIT_NODE_ROUTING_LOADER_H

#include <fstream>
#include <string>
#include <map>
#include "../Structures/Graph.h"
#include "../Structures/UpdateableGraph.h"
#include "../Structures/BaseGraph.h"
#include "GraphLoader.h"



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
    std::string inputFile;
    std::ifstream input;
    bool amountsParsed;
    unsigned int nodesAmount;
    size_t edgesAmount = 0;

    void parseAmounts();

    /**
     * Auxiliary function used to parse the problem line of the input file (first line that is not a comment).
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param nodes[out] The number of nodes in the graph.
     * @param edges[out] The number of edges in the graph.
     */
    void parseGraphProblemLine(std::ifstream & input, unsigned int & nodes, size_t & edges);

    /**
     * Auxiliary function used to parse the edges when loading the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] The graph instance that the edges will be inserted into.
     * @param edges[in] The number of edges that need to be loaded.
     * @param[in] scaling_factor This parameter allows us to lose some precision
     * of the weight values. Each loaded weight will be divided by this value before rounding.
     */
	void parseEdges(BaseGraph& graph, int scaling_factor);

    /**
     * Auxiliary function that extracts the number of nodes and the number of edges from the problem line.
     *
     * @param buffer[in] A buffer containing the problem line.
     * @param nodes[out] The number of nodes in the graph.
     * @param edges[out] The number of edges in the graph.
     */
    void processGraphProblemLine(std::string & buffer, unsigned int & nodes, size_t & edges);

    /**
     * Extracts an edge data from an input line representing an edge.
     *
     * @param buffer[in] A buffer containing the edge line.
     * @param from[out] The source node of the edge.
     * @param to[out] The target node of the edge.
     * @param weight[out] The weight of the edge.
     */
    void getEdge(std::string & buffer, unsigned int & from, unsigned int & to, dist_t & weight);

public:
    /**
     * A simple constructor.
     *
     * @param inputFile[in] A path towards a file that should be loaded using this loader.
     */
    explicit DIMACSLoader(std::string inputFile);

    unsigned int nodes() override;

    size_t edges();

    void loadGraph(BaseGraph &graph, int scaling_factor) override;

    ~DIMACSLoader() override = default;
};

#endif //TRANSIT_NODE_ROUTING_LOADER_H
