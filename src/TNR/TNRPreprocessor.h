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
// Created on: 05.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRPREPROCESSOR_H
#define CONTRACTION_HIERARCHIES_TNRPREPROCESSOR_H

#include <unordered_set>
#include "../GraphBuilding/Structures/UpdateableGraph.h"
#include "Structures/AccessNodeData.h"
#include "../GraphBuilding/Structures/FlagsGraph.h"
#include "../GraphBuilding/Loaders/XenGraphLoader.h"
#include "../DistanceMatrix/Distance_matrix_travel_time_provider.h"




/**
 * This class can create a Transit Node Routing (TNR) data-structure based on a given UpdateableGraph. The user can
 * specify the desired size of the transit node set, and three preprocessing modes are supported. The quickest mode
 * (implemented by the 'preprocessUsingCH' function) generates data structures with worse performance than the other
 * two, but should be the quickest of the three. The other two (functions 'preprocessUsingCHslower' and
 * 'preprocessWithDMvalidation') will both provide the exact same result, but the first one mentioned is slower,
 * but also requires less memory, while the second one is quicker, but it can not be used for large graphs because
 * of its memory requirements.
 */
class TNRPreprocessor {
public:
    /**
     * Builds Transit Node Routing structures based on Contraction Hierarchies.
     * User can set the amount of Transit Nodes. Generally, more transit nodes mean more memory consumption as more
     * distances must be present in the distance table, but could potentially speed up the queries as more queries will
     * hit those new transit nodes. This function represent a preprocessing mode that is should be fairly quick,
     * but the obtained data structure will have worse performance than in the case of the other two functions.
     *
     * @param graph[in] The input graph in the UpdateableGraph format.
     * @param outputPath[in] The desired output path where the obtained data structure will be output.
     * @param transitNodesAmount[in] The desired amount of transit nodes.
     */
    static void preprocessUsingCH(
            UpdateableGraph & graph,
            std::string outputPath,
            unsigned int transitNodesAmount = 1000);

    /**
     * Builds Transit Node Routing structures based on Contraction Hierarchies.
     * User can set the amount of Transit Nodes. Generally, more transit nodes mean more memory consumption as more
     * distances must be present in the distance table, but could potentially speed up the queries as more queries will
     * hit those new transit nodes. This variant is slower but validates that the obtained access nodes have correct
     * distances which allows us to discard some access nodes, meaning the obtained Transit Node Routing data structure
     * will provide better performance.
     *
     * @param graph[in] The input graph in the UpdateableGraph format.
     * @param originalGraph[in] A copy of the input graph in the simple Graph format that is useful at some parts of the
     * preprocessing process.
     * @param outputPath[in] The desired output path where the obtained data structure will be output.
     * @param transitNodesAmount[in] The desired amount of transit nodes.
     */
    static void preprocessUsingCHslower(
            UpdateableGraph & graph,
            Graph & originalGraph,
            std::string outputPath,
            unsigned int transitNodesAmount = 1000);

    /**
     * Build Transit Node Routing structures based on Contraction Hierarchies.
     * This variant will return the exact same result as the 'preprocessUsingCHslower' function, but this variant should
     * be faster while also needing more memory. This function can therefore be used for small graphs to speed up the
     * precomputation process, but for large graphs it might not be possible to use it due to its memory requirements
     * and the slower variant will have to be used.
     *
     * @param graph[in] The input graph in the UpdateableGraph format.
     * @param originalGraph[in] A copy of the input graph in the simple Graph format that is useful at some parts of the
     * preprocessing process.
     * @param outputPath[in] The desired output path where the obtained data structure will be output.
     * @param transitNodesAmount[in] The desired amount of transit nodes.
     */
    static void preprocessWithDMvalidation(
            UpdateableGraph & graph,
            Graph & originalGraph,
            std::string outputPath,
            unsigned int transitNodesAmount = 1000,
            unsigned int intSize = 0);

protected:
    /**
     * Outputs the created Transit Node Routing data-structure with all the information required for the query algorithm
     * into a binary file.
     *
     * @param outputPath[in] The output path where the computed data structure will be output.
     * @param graph[in] The graph.
     * @param allEdges[in] Vector that contains all the edges that have to be output into the binary file.
     * @param transitNodes[in] Contains the IDs of all the transit nodes.
     * @param transitNodesDistanceTable[in] A 2D matrix containing pairwise distances between all pairs of transit
     * nodes.
     * @param forwardAccessNodes[in] Contains forward access nodes for each node in the graph (along with their data).
     * @param backwardAccessNodes[in] Contains backward access nodes for each node in the graph (along with their data).
     * @param forwardSearchSpaces[in] Contains forward search spaces for each node in the graph.
     * @param backwardSearchSpaces[in] Contains backward search spaces for each node in the graph.
     * @param transitNodesAmount[in] Denotes the size of the transit node set.
     */
    static void outputGraph(
            std::string outputPath,
            UpdateableGraph & graph,
            std::vector < std::pair< unsigned int, QueryEdge > > & allEdges,
            std::vector < unsigned int > & transitNodes,
            std::vector < std::vector < unsigned int > > & transitNodesDistanceTable,
            std::vector < std::vector < AccessNodeData > > & forwardAccessNodes,
            std::vector < std::vector < AccessNodeData > > & backwardAccessNodes,
            std::vector < std::vector < unsigned int > > & forwardSearchSpaces,
            std::vector < std::vector < unsigned int > > & backwardSearchSpaces,
            unsigned int transitNodesAmount);

    /**
     * Auxiliary function that will find backward access nodes for a given node. This is achieved using a simplified
     * version of the Contraction Hierarchies query algorithm. This function is used in the 'preprocessUsingCH'
     * function. This function can also return some access nodes that are never used since their distances are
     * incorrect. This can happen if better path from the access node to the source node would be found in the backward
     * direction.
     *
     * @param source[in] The ID of the node for which the access nodes are being computed.
     * @param accessNodes[out] The std::vector into which the found access nodes will be stored (along with their data).
     * @param forwardSearchSpace[out] The search space that was explored during the process of finding access nodes.
     * This is used for the locality filter.
     * @param transitNodes[in] Mapping from node IDs to their positions in the transit node distance matrix. Nodes that
     * are not transit nodes are not present in this table.
     * @param graph[in]
     */
    static void findForwardAccessNodes(
            unsigned int source,
            std::vector < AccessNodeData> & accessNodes,
            std::vector < unsigned int > & forwardSearchSpace,
            std::unordered_map< unsigned int, unsigned int > & transitNodes,
            FlagsGraph<NodeData>& graph);

    /**
     * Auxiliary function that will find backward access nodes for a given node. This is achieved using a simplified
     * version of the Contraction Hierarchies query algorithm. This function is used in the 'preprocessUsingCH'
     * function. This function can also return some access nodes that are never used since their distances are
     * incorrect. This can happen if better path from the access node to the source node would be found in the forward
     * direction.
     *
     * @param source[in] The ID of the node for which the access nodes are being computed.
     * @param accessNodes[out] The std::vector into which the found access nodes will be stored (along with their data).
     * @param forwardSearchSpace[out] The search space that was explored during the process of finding access nodes.
     * This is used for the locality filter.
     * @param transitNodes[in] Mapping from node IDs to their positions in the transit node distance matrix. Nodes that
     * are not transit nodes are not present in this table.
     * @param graph[in]
     */
    static void findBackwardAccessNodes(
            unsigned int source,
            std::vector < AccessNodeData> & accessNodes,
            std::vector < unsigned int > & backwardSearchSpace,
            std::unordered_map< unsigned int, unsigned int > & transitNodes,
            FlagsGraph<NodeData>& graph);

    /**
     * Auxiliary function that will find forward access nodes for a given node. The process consists of first finding
     * all the access nodes candidates using a simplified version of the Contraction Hierarchies query algorithm,
     * some of the candidates can then be removed if their distances are incorrect. This can be verified using the
     * precomputed distances from source node in the 'distsFromNode' argument. This function is used in the
     * 'preprocessUsingCHslower' function.
     *
     * @param source[in] The ID of the node for which the access nodes are being computed.
     * @param accessNodes[out] The std::vector into which the found access nodes will be stored (along with their data).
     * @param forwardSearchSpace[out] The search space that was explored during the process of finding access nodes.
     * This is used for the locality filter.
     * @param transitNodes[in] Mapping from node IDs to their positions in the transit node distance matrix. Nodes that
     * are not transit nodes are not present in this table.
     * @param distsFromNode[in] Contains the distances from 'source' to all other nodes in the graph. Those distances
     * can be used to discard some access nodes due to their incorrect distance.
     * @param graph[in]
     */
    static void findForwardAccessNodes(
            unsigned int source,
            std::vector < AccessNodeData> & accessNodes,
            std::vector < unsigned int > & forwardSearchSpace,
            std::unordered_map< unsigned int, unsigned int > & transitNodes,
            std::vector < unsigned int > & distsFromNode,
            FlagsGraph<NodeData>& graph);

    /**
     * Auxiliary function that will find backward access nodes for a given node. The process consists of first finding
     * all the access nodes candidates using a simplified version of the Contraction Hierarchies query algorithm,
     * some of the candidates can then be removed if their distances are incorrect. This can be verified using the
     * precomputed distances from source node in the 'distsFromNode' argument. This function is used in the
     * 'preprocessUsingCHslower' function.
     *
     * @param source[in] The ID of the node for which the access nodes are being computed.
     * @param accessNodes[out] The std::vector into which the found access nodes will be stored (along with their data).
     * @param backwardSearchSpace[out] The search space that was explored during the process of finding access nodes.
     * This is used for the locality filter.
     * @param transitNodes[in] Mapping from node IDs to their positions in the transit node distance matrix. Nodes that
     * are not transit nodes are not present in this table.
     * @param distsFromNode[in] Contains the distances from 'source' to all other nodes in the graph. Those distances
     * can be used to discard some access nodes due to their incorrect distance.
     * @param graph[in]
     */
    static void findBackwardAccessNodes(
            unsigned int source,
            std::vector < AccessNodeData> & accessNodes,
            std::vector < unsigned int > & backwardSearchSpace,
            std::unordered_map< unsigned int, unsigned int > & transitNodes,
            std::vector < unsigned int > & distsFromNode,
            FlagsGraph<NodeData>& graph);

    /**
     * Auxiliary function that will find forward access nodes for a given node. The process consists of first finding
     * all the access nodes candidates using a simplified version of the Contraction Hierarchies query algorithm,
     * some of the candidates can then be removed if their distances are incorrect. This can be verified using the
     * value in the distance matrix. This function is used in the 'preprocessWithDMvalidation' function.
     *
     * @param source[in] The ID of the node for which the access nodes are being computed.
     * @param accessNodes[out] The std::vector into which the found access nodes will be stored (along with their data).
     * @param forwardSearchSpace[out] The search space that was explored during the process of finding access nodes.
     * This is used for the locality filter.
     * @param transitNodes[in] Mapping from node IDs to their positions in the transit node distance matrix. Nodes that
     * are not transit nodes are not present in this table.
     * @param graph[in]
     * @param dm[in] A full distance matrix containing pairwise distances between all pairs of nodes in the graph.
     */
    static void findForwardAccessNodes(
            unsigned int source,
            std::vector < AccessNodeData> & accessNodes,
            std::vector < unsigned int > & forwardSearchSpace,
            std::unordered_map< unsigned int, unsigned int > & transitNodes,
            FlagsGraph<NodeData>& graph,
            DistanceMatrixInterface& dm);

    /**
     * Auxiliary function that will find backward access nodes for a given node. The process consists of first finding
     * all the access nodes candidates using a simplified version of the Contraction Hierarchies query algorithm,
     * some of the candidates can then be removed if their distances are incorrect. This can be verified using the
     * value in the distance matrix. This function is used in the 'preprocessWithDMvalidation' function.
     *
     * @param source[in] The ID of the node for which the access nodes are being computed.
     * @param accessNodes[out] The std::vector into which the found access nodes will be stored (along with their data).
     * @param backwardSearchSpace[out] The search space that was explored during the process of finding access nodes.
     * This is used for the locality filter.
     * @param transitNodes[in] Mapping from node IDs to their positions in the transit node distance matrix. Nodes that
     * are not transit nodes are not present in this table.
     * @param graph[in]
     * @param dm[in] A full distance matrix containing pairwise distances between all pairs of nodes in the graph.
     */
    static void findBackwardAccessNodes(
        unsigned int source,
        std::vector<AccessNodeData> &accessNodes,
        std::vector<unsigned int> &backwardSearchSpace,
        std::unordered_map<unsigned int, unsigned int> &transitNodes,
        FlagsGraph<NodeData> &graph,
        DistanceMatrixInterface& dm);

};


#endif //CONTRACTION_HIERARCHIES_TNRPREPROCESSOR_H
