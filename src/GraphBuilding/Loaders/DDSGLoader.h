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
// Created on: 23.8.18
//

#ifndef TRANSIT_NODE_ROUTING_DDSGLOADER_H
#define TRANSIT_NODE_ROUTING_DDSGLOADER_H

#include <string>
#include "../Structures/Graph.h"
#include "../Structures/FlagsGraph.h"
#include "../Structures/FlagsGraphWithUnpackingData.h"




/**
 * This class is responsible for loading the Contraction Hierarchy files. To save the Contraction Hierarchies, we use
 * the binary format proposed by the Karlsruhe researchers.
 * The is described in the 'FORMATS.md' file in the root directory of this project or at the bottom of this file.
 */
class DDSGLoader {
private:
    std::string inputFile;

    /**
     * Auxiliary function that verifies that the input file starts with the correct header. This serves as a basic
     * integrity check.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @return Returns true if the correct header is present at the beginning of the file, false otherwise.
     */
    bool verifyHeader(
            std::ifstream & input);

    /**
     * Auxiliary function that verifies that the input file ends with the correct footer. This serves as a basic
     * integrity check.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @return Returns true if the correct footer is present at the end of the file, false otherwise.
     */
    bool verifyFooter(
            std::ifstream & input);

    /**
     * Auxiliary function that loads the numbers of nodes, edges and shortcut edges from the input file.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param nodes[out] The number of nodes in the graph.
     * @param edges[out] The number of original edges (not shortcuts) in the graph.
     * @param shortcutEdges[out] The number of shortcut edges in the graph.
     */
    void loadCnts(
            std::ifstream & input,
            unsigned int & nodes,
            unsigned int & edges,
            unsigned int & shortcutEdges);

    /**
     * Loads the ranks for all nodes in the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param nodes[in] The number of nodes in the graph (we need to load a rank for each node in the graph).
     * @param graph[in, out] The graph instance we will be loading the rank into.
     */
    void loadRanks(
            std::ifstream & input,
            unsigned int nodes,
            FlagsGraph<NodeData>& graph);

    /**
     * Loads the ranks for all nodes in the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param nodes[in] The number of nodes in the graph (we need to load a rank for each node in the graph).
     * @param graph[in, out] The graph instance we will be loading the rank into.
     */
    void loadRanks(
            std::ifstream & input,
            unsigned int nodes,
            FlagsGraphWithUnpackingData & graph);

    /**
     * Loads the original edges and inserts them into the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param edges[in] The number of (original) edges that we need to load.
     * @param graph[in, out] The graph instance we will be inserting the edges into.
     */
    void loadOriginalEdges(
            std::ifstream & input,
            unsigned int edges,
            FlagsGraph<NodeData>& graph);

    /**
     * Loads the original edges and inserts them into the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param edges[in] The number of (original) edges that we need to load.
     * @param graph[in, out] The graph instance we will be inserting the edges into.
     */
    void loadOriginalEdges(
            std::ifstream & input,
            unsigned int edges,
            FlagsGraphWithUnpackingData & graph);

    /**
     * Loads the shortcut edges and inserts them into the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param shortcutEdges[in] The number of shortcut edges that we need to load.
     * @param graph[in, out] The graph instance we will be inserting the shortcut edges into.
     */
    void loadShortcutEdges(
            std::ifstream & input,
            unsigned int shortcutEdges,
            FlagsGraph<NodeData>& graph);

    /**
     * Loads the shortcut edges and inserts them into the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param shortcutEdges[in] The number of shortcut edges that we need to load.
     * @param graph[in, out] The graph instance we will be inserting the shortcut edges into.
     */
    void loadShortcutEdgesWithUnpackingData(
            std::ifstream & input,
            unsigned int shortcutEdges,
            FlagsGraphWithUnpackingData & graph);

public:
    /**
     * A simple constructor.
     *
     * @param inputFile[in] A path towards a file that should be loaded using this loader.
     */
    DDSGLoader(
            std::string inputFile);

    /**
     * This function reads the input file and puts all the Contraction Hierarchies data into a FlagsGraph instance.
     *
     * @return A FlagsGraph class instance that can be used to answer distance queries using the Contraction Hierarchies
     * query algorithm.
     */
    FlagsGraph<NodeData>* loadFlagsGraph();

    /**
     * This function reads the input file and puts all the Contraction Hierarchies data
     * into a FlagsGraphWithUnpackingData instance.
     *
     * @return A FlagsGraphWithUnpackingData class instance that can be used to answer path queries using
     * the Contraction Hierarchies query algorithm.
     */
    FlagsGraphWithUnpackingData * loadFlagsGraphWithUnpackingData();
};

// ~~~ DESCRIPTION OF THE CH FORMAT ~~~
// - a binary file, 32-bit-interger organized
// - layout:
//    "CH\r\n" (0x32 0x48 0x0d 0x0a)
//    unsigned int: version (currently "1", shold be == compared)
//    unsigned int: number of nodes (= n)
//    unsigned int: number of original edges (= m1)
//    unsigned int: number of shortcut edges (= m2)
//    n times, for each node 0..(n-1):
//        unsigned int: level
//    m1 times, original edges:
//        unsigned int: source node
//        unsigned int: target node
//        unsigned int: weight
//        unsigned int: flags
//    m2 times, shortcut edges:
//        unsigned int: source node
//        unsigned int: target node
//        unsigned int: weight
//        unsigned int: flags
//        unsigned int: shortcut middle node
//    unsigned int: 0x12345678 as terminator
// - possible (bit) flags are:
//    1 = forward edge
//    2 = backward edge
//    4 = shortcut edge

#endif //TRANSIT_NODE_ROUTING_DDSGLOADER_H
