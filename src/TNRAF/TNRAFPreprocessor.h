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
// Created on: 30.11.19
//

#ifndef CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H
#define CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H


#include <queue>
#include "../TNR/TNRPreprocessor.h"
#include "Structures/AccessNodeDataArcFlags.h"
#include "Structures/RegionsStructure.h"
#include "../DistanceMatrix/Distance_matrix_travel_time_provider.h"
#include "../TNRAF/Structures/NodeDataRegions.h"
#include "TNRAFPreprocessingMode.h"

/**
 * This class is responsible for creating a Transit Node Routing with Arc Flags data-structure based on a given
 * UpdateableGraph. The user can specify the size of the transit node set, and also two preprocessing modes are present,
 * both giving the exact same results, but one mode is quite fast but requires a lot of memory, while the other mode is
 * significantly slower, but requires less memory.
 */
class TNRAFPreprocessor : public TNRPreprocessor {
public:
    /**
     * The main workhorse function. The result of this function will be the data-structure required for the Transit Node
     * Routing with Arc Flags query algorithm saved in a binary file. Since we are working with Transit Node Routing
     * based on Contraction Hierarchies, this function expects that the UpdateableGraph instance was already processed
     * by the CHPreprocessor and therefore already contains Contraction Hierarchies information such as ranks and
     * shortcut edges. The user can set the amount of transit nodes that will be used, this is done by setting
     * the transitNodesAmount variable. The regionsCnt variable should control the amount of regions that will be
     * used for the Arc Flags functionality, but only 32 are working currently due to the way the output is handled
     * right now. The bool variable useDistanceMatrix allows the user to switch between two preprocessing modes.
     * If the flag is set to true, preprocessing uses the a full Distance Matrix to speed up the process.
     * This leads to significant memory requirements, but the preprocessing itself can then be very fast.
     * This should be used if you have more resources available during the preprocessing phase than during
     * the actual queries. If the flag is set to false, the preprocessing will be much slower, but will require
     * less memory and it should then be possible to complete it even on machines where there is not enough memory
     * for the faster mode.
     *
     * @param graph[in] The input graph in the form of an UpdateableGraph.
     * @param originalGraph[in] A copy of the input graph in the simple Graph class form. This is used for example when
     * computing Arc Flags to verify that there is no shortest path between two nodes going through some other node.
     * @param outputPath[in] The desired ouput path where the precomputed data structure should be output.
     * @param transitNodesAmount[in] The desired size of the transit node set.
     * @param regionsCnt[in] The desired amount of regions for the Arc Flags.
     * @param useDistanceMatrix[in] A flag indicating whether the slower or the faster but more memory consuming
     * preprocessing mode should be used.
     */
    static void preprocessUsingCH(
            UpdateableGraph & graph,
            Graph & originalGraph,
            const std::string & outputFilenameprefix,
            unsigned int transitNodesDesired,
            unsigned int regionsCnt,
            unsigned int dmIntSize,
            TNRAFPreprocessingMode dm_mode);

    /**
     * Auxiliary function in order to make sure the powers of 2 are precomputed.
     *
     * @param[out] powersOf2
     */
    static void getPowersOf2(std::vector<uint32_t> & powersOf2);
protected:

    /**
     * Outputs the created Transit Node Routing with Arc Flags data-structure with all the information required for
     * the query algorithm into a binary file.
     *
     * @param outputPath[in] The desired ouput path where the precomputed data structure will be output.
     * @param graph[in] The graph representation.
     * @param allEdges[in] A std::vector containing all edges that need to be put into the data structure (includes
     * shortcuts).
     * @param transitNodes[in] A std::vector containing the IDs of the nodes that were chosen as transit nodes.
     * @param transitNodesDistanceTable[in] 2D matrix containing pairwise distances between all pairs of transit nodes.
     * @param forwardAccessNodes[in] Contains forward access nodes for each node with all their information.
     * @param backwardAccessNodes[in] Contains backward access nodes for each node with all their information.
     * @param forwardSearchSpaces[in] Contains forward search spaces for each node.
     * @param backwardSearchSpaces[in] Contains backward search spaces for each node.
     * @param transitNodesAmount[in] Denotes the number of transit nodes.
     * @param regions[in] A structure with all information about the regions for Arc Flags.
     * @param regionsCnt[in] Denotes the number of regions.
     */
    static void outputGraph(
            std::string outputPath,
            UpdateableGraph & graph,
            std::vector < std::pair< unsigned int, QueryEdge > > & allEdges,
            std::vector < unsigned int > & transitNodes,
            std::vector < std::vector < unsigned int > > & transitNodesDistanceTable,
            std::vector < std::vector < AccessNodeDataArcFlags > > & forwardAccessNodes,
            std::vector < std::vector < AccessNodeDataArcFlags > > & backwardAccessNodes,
            std::vector < std::vector < unsigned int > > & forwardSearchSpaces,
            std::vector < std::vector < unsigned int > > & backwardSearchSpaces,
            unsigned int transitNodesAmount,
            RegionsStructure & regions,
            unsigned int regionsCnt);

    /**
     * Computes the full distance matrix for the transit node set. This is done by t runs of one-to-all Dijkstra
     * (where t denotes the amount of transit nodes).
     *
     * @param transitNodes[in] A std::vector containing the IDs of the nodes that were chosen as transit nodes.
     * @param distanceTable[out] 2D matrix that will contain pairwise distances between all pairs of transit nodes
     * after this function finishes.
     * @param transitNodesCnt[in] The number denoting the transit node set size.
     * @param originalGraph[in] The original graph that is used to compute the distances.
     */
    static void computeTransitNodeDistanceTable(
            std::vector<unsigned int> & transitNodes,
            std::vector<std::vector<unsigned int>> & distanceTable,
            unsigned int transitNodesCnt,
            Graph & originalGraph);

    /**
     * Fills the full distance matrix for the transit node set using value from the full distance matrix for the graph,
     * when the distance matrix is used to speed up the preprocessing phase (when the 'dm' mode is used).
     *
     * @param transitNodes[in] A std::vector containing the IDs of the nodes that were chosen as transit nodes.
     * @param distanceTable[out] 2D matrix that will contain pairwise distances between all pairs of transit nodes
     * after this function finishes.
     * @param transitNodesCnt[in] The number denoting the transit node set size.
     */
    static void fillTransitNodeDistanceTable(
            std::vector<unsigned int> & transitNodes,
            std::vector<std::vector<unsigned int>> & distanceTable,
            unsigned int transitNodesCnt);

    /**
     * Auxiliary function that will find forward access nodes for a given node. The process consists of first finding
     * all the access nodes candidates using a simplified version of the Contraction Hierarchies query algorithm,
     * some of the candidates can then be removed if their distances are incorrect.
     * Arc Flags then have to be computed for the actual set of access nodes.
     *
     * @param source[in] The ID of the node for which the access nodes are being computed.
     * @param accessNodes[out] The std::vector into which the found access nodes will be stored (along with their data).
     * @param forwardSearchSpace[out] The search space that was explored during the process of finding access nodes.
     * This is used for the locality filter.
     * @param transitNodes[in] Mapping from node IDs to their positions in the transit node distance matrix. Nodes that
     * are not transit nodes are not present in this table.
     * @param graph[in]
     * @param originalGraph[in]
     * @param regions[in]
     * @param useDistanceMatrix[in]
     */
    static void findForwardAccessNodes(
            unsigned int source,
            std::vector <AccessNodeDataArcFlags> & accessNodes,
            std::vector < unsigned int > & forwardSearchSpace,
            std::unordered_map< unsigned int, unsigned int > & transitNodes,
            FlagsGraph<NodeDataRegions>& graph,
            Graph & originalGraph,
            RegionsStructure & regions,
            TNRAFPreprocessingMode useDistanceMatrix);

    /**
     * Auxiliary function that will find backward access nodes for a given node. The process consists of first finding
     * all the access nodes candidates using a simplified version of the Contraction Hierarchies query algorithm,
     * some of the candidates can then be removed if their distances are incorrect.
     * Arc Flags then have to be computed for the actual set of access nodes.
     *
     * @param source[in] The ID of the node for which the access nodes are being computed.
     * @param accessNodes[out] The std::vector into which the found access nodes will be stored (along with their data).
     * @param backwardSearchSpace[out] The search space that was explored during the process of finding access nodes.
     * This is used for the locality filter.
     * @param transitNodes[in] Mapping from node IDs to their positions in the transit node distance matrix. Nodes that
     * are not transit nodes are not present in this table.
     * @param graph[in]
     * @param originalGraph[in]
     * @param regions[in]
     * @param useDistanceMatrix[in]
     */
    static void findBackwardAccessNodes(
            unsigned int source,
            std::vector <AccessNodeDataArcFlags> & accessNodes,
            std::vector < unsigned int > & backwardSearchSpace,
            std::unordered_map< unsigned int, unsigned int > & transitNodes,
            FlagsGraph<NodeDataRegions>& graph, Graph & originalGraph,
            RegionsStructure & regions,
            TNRAFPreprocessingMode useDistanceMatrix);

    /**
     * Computes forward Arc Flags for a set of forward access nodes of a given node. This is done simply by comparing
     * the distances. If the distance from node to the access node plus the distance from the access node to some node
     * from the region is equal to the distance from node to the same node from the region, then the flag for
     * that region is set to true because we have found a shortest path to some node in the region going through
     * our access node. If we do not find any node from some region for which the access node would be
     * on a shortest path, then the Arc Flag for this region can be set to false.
     *
     * @param node[in] The ID of the node for which the access nodes are being computed currently.
     * @param accessNodes[in, out] The list of access nodes for which the flags need to be computed
     * @param originalGraph[in]
     * @param regions[in] The structure containing all the information about the regions for the Arc Flags.
     * @param useDistanceMatrix[in]
     * @param optionalDistancesFromNode[in] This is only used for the slower ('slow') preprocessing mode. The std::vector
     * contains distances from 'node' to all the other nodes in the graph. This is needed to set the Arc Flags
     * correctly. When using the distance matrix (the 'dm' mode), those distances can be obtained from the distance
     * matrix so this is not needed.
     */
    static void computeForwardArcFlags(
            unsigned int node,
            std::vector<AccessNodeDataArcFlags> & accessNodes,
            Graph & originalGraph,
            RegionsStructure & regions,
            TNRAFPreprocessingMode useDistanceMatrix,
            std::vector<unsigned int> & optionalDistancesFromNode);

    /**
     * Computes the backward Arc Flags for a set of access nodes of a given node.
     * This is similar to the forward direction, so check computeForwardArcFlags for more information.
     *
     * @param node[in] The ID of the node for which the access nodes are being computed currently.
     * @param accessNodes[in, out] The list of access nodes for which the flags need to be computed
     * @param originalGraph[in]
     * @param regions[in] The structure containing all the information about the regions for the Arc Flags.
     * @param useDistanceMatrix[in]
     * @param optionalDistancesFromNode[in] This is only used for the slower ('slow') preprocessing mode. The std::vector
     * contains distances from 'node' to all the other nodes in the graph. This is needed to set the Arc Flags
     * correctly. When using the distance matrix (the 'dm' mode), those distances can be obtained from the distance
     * matrix so this is not needed.
     */
    static void computeBackwardArcFlags(
            unsigned int node,
            std::vector<AccessNodeDataArcFlags> & accessNodes,
            Graph & originalGraph,
            RegionsStructure & regions,
            TNRAFPreprocessingMode useDistanceMatrix,
            std::vector<unsigned int> & optionalDistancesFromNode);

    /**
     * Generates a clustering that is used for the regions (and those are used for Arc Flags). The query algorithm will
     * work with any clustering where each node is assigned to exactly one cluster. Clusterings where we have clusters
     * of equal sizes and nodes that are close to each other are also in the same cluster give better
     * performance though. Here we use a very simple clustering that is based on the k-means idea.
     * We first initialize our k clusters by assigning one node to each of them. We then try to expand each cluster
     * by adding some node that is connected to some node already in the cluster that has not been assigned to any
     * cluster so far. If no such node can be found, we just randomly add some other node that does not have a cluster
     * yet to the cluster in order to keep the size of the clusters somewhat balanced.
     * This approach seems to give solid results.
     *
     * @param originalGraph[in] The original graph.
     * @param regions[out] A structure that will contain all the information about the regions (which nodes corresponds
     * to which regions).
     * @param clustersCnt[in] The number of clusters that need to be computed.
     */
    static void generateClustering(
            Graph & originalGraph,
            RegionsStructure & regions,
            unsigned int clustersCnt);

    /**
     * Finds a node to add into the currently expanded cluster. First tries to get an unassigned node from the queue,
     * if no such node exists, assigns some other unassigned node.
     *
     * @param assignedClusters Contains the IDs of the clusters to which the nodes in the graph are assigned. If some
     * node is not assigned to any cluster yet, this std::vector contains UINT_MAX.
     * @param q The queue containing the IDs of the candidates that could be added into the currently processed cluster.
     * @return The ID of the node that should be added to the currently processed cluster.
     */
    static unsigned int getNewNodeForCluster(
            std::vector < unsigned int > & assignedClusters,
            std::queue < unsigned int > & q);

    /**
     * Auxiliary function that precomputes powers of 2 up to 2^32. This is used for the Arc Flags output. In order to
     * output Arc Flags efficiently, we output 32 Arc Flags as one unsigned int. To set individual Arc Flags,
     * we can add or subtract powers of 2. We prepare those powers of 2 using this function to speed up the process.
     *
     * @param powersOf2[out]
     */
    static void initPowersOf2(std::vector<uint32_t> & powersOf2);

    static DistanceMatrixInterface* distanceMatrix;
};


#endif //CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H
