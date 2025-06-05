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
#include "../Dijkstra/BasicDijkstra.h"
#include <boost/numeric/conversion/cast.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>

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
    void preprocessUsingCH(
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

    // Getters for benchmark times
    std::chrono::milliseconds getForwardDmComputationTimeMs() const { return forward_dm_computation_time_ms_; }
    std::chrono::milliseconds getBackwardDmComputationTimeMs() const { return backward_dm_computation_time_ms_; }
    std::chrono::milliseconds getForwardAccessNodesComputationTimeMs() const { return forward_access_nodes_computation_time_ms_; }
    std::chrono::milliseconds getBackwardAccessNodesComputationTimeMs() const { return backward_access_nodes_computation_time_ms_; }

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
            Regions_with_borders & regions,
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
    void fillTransitNodeDistanceTable(
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
     * @param mode[in]
     */
    void find_forward_access_nodes_for_single_node(
        unsigned int source,
        std::vector<AccessNodeDataArcFlags>& accessNodes,
        std::vector<unsigned int>& forwardSearchSpace,
        std::unordered_map<unsigned int, unsigned int>& transitNodes,
        FlagsGraph<NodeDataRegions>& graph,
        Graph& originalGraph,
        Regions_with_borders& regions,
        TNRAFPreprocessingMode mode
    );

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
    void find_backward_access_nodes_for_single_node(
            unsigned int source,
            std::vector <AccessNodeDataArcFlags> & accessNodes,
            std::vector < unsigned int > & backwardSearchSpace,
            std::unordered_map< unsigned int, unsigned int > & transitNodes,
            FlagsGraph<NodeDataRegions>& graph, Graph & originalGraph,
            Regions_with_borders & regions,
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
     * @param access_nodes[in, out] The list of access nodes for which the flags need to be computed
     * @param originalGraph[in]
     * @param regions[in] The structure containing all the information about the regions for the Arc Flags.
     * @param optionalDistancesFromNode[in] This is only used for the slower ('slow') preprocessing mode. The std::vector
     * contains distances from 'node' to all the other nodes in the graph. This is needed to set the Arc Flags
     * correctly. When using the distance matrix (the 'dm' mode), those distances can be obtained from the distance
     * matrix so this is not needed.
     * @param forward_direction
     */
    template<TNRAFPreprocessingMode mode>
    void compute_arc_flags(
        std::vector<std::vector<AccessNodeDataArcFlags>>& access_nodes,
        Graph& originalGraph,
        Regions_with_borders& regions,
        bool forward_direction
    ) {
        for (unsigned int region_index = 0; region_index < regions.getRegionsCnt(); region_index++) {
            const auto& border_nodes_in_region = regions.getBorderNodes(region_index);

            // compute the DM of distances to border nodes from all other nodes for the current region
            std::optional<std::vector<std::vector<dist_t>>> distances_to_border_nodes;
            if constexpr(mode != TNRAFPreprocessingMode::DM) {
                distances_to_border_nodes.emplace();
                for (unsigned int bn_index = 0; bn_index < border_nodes_in_region.size(); bn_index++) {
                    distances_to_border_nodes->emplace_back(originalGraph.nodes());
                    auto border_node = border_nodes_in_region[bn_index];
                    if(forward_direction) {
                        BasicDijkstra::computeOneToAllDistancesInReversedGraph(
                            border_node,
                            originalGraph,
                            distances_to_border_nodes.value()[bn_index]
                        );
                    }
                    else {
                        BasicDijkstra::computeOneToAllDistances(
                            border_node,
                            originalGraph,
                            distances_to_border_nodes.value()[bn_index]
                        );
                    }
                }
            }

            // std::optional<std::vector<std::vector<dist_t>>> distances_from_border_nodes;
            // if constexpr(mode == TNRAFPreprocessingMode::SLOW) {
            //     distances_to_border_nodes.emplace();
            //     for (unsigned int bn_index = 0; bn_index < border_nodes_in_region.size(); bn_index++) {
            //         distances_to_border_nodes->emplace_back(originalGraph.nodes());
            //         auto border_node = border_nodes_in_region[bn_index];
            //         if(forward_direction) {
            //             BasicDijkstra::computeOneToAllDistancesInReversedGraph(
            //                 border_node,
            //                 originalGraph,
            //                 distances_to_border_nodes.value()[bn_index]
            //             );
            //         }
            //         else {
            //             BasicDijkstra::computeOneToAllDistances(
            //                 border_node,
            //                 originalGraph,
            //                 distances_to_border_nodes.value()[bn_index]
            //             );
            //         }
            //     }
            // }

            for(unsigned node_index = 0; node_index < originalGraph.nodes(); node_index++) {
                auto& access_nodes_per_node = access_nodes[node_index];

                for(size_t access_node_index = 0; access_node_index < access_nodes_per_node.size(); access_node_index++) {
                    unsigned int access_node = access_nodes_per_node[access_node_index].accessNodeID;
                    unsigned int distanceToAccessNode = access_nodes_per_node[access_node_index].distanceToNode;

                    // std::optional<std::vector<unsigned int>> distancesFromAccessNode;
                    // if constexpr(mode == TNRAFPreprocessingMode::SLOW) {
                    //     distancesFromAccessNode.emplace(originalGraph.nodes());
                    //     if(forward_direction) {
                    //         BasicDijkstra::computeOneToAllDistances(access_node, originalGraph, distancesFromAccessNode.value());
                    //     }
                    //     else {
                    //         BasicDijkstra::computeOneToAllDistancesInReversedGraph(
                    //             access_node,
                    //             originalGraph,
                    //             distancesFromAccessNode.value()
                    //         );
                    //     }
                    // }


                    // if the region is the region of the access node, set the flag to true and continue
                    if (regions.getRegion(access_node) == region_index) {
                        access_nodes_per_node[access_node_index].regionFlags[region_index] = true;
                        continue;
                    }

                    for(unsigned int bn_index = 0; bn_index < border_nodes_in_region.size(); bn_index++) {
                        auto border_node = border_nodes_in_region[bn_index];
                        dist_t distance_from_node_to_border_node = 0;
                        dist_t distance_from_access_node_to_border_node = 0;

                        if constexpr(mode == TNRAFPreprocessingMode::DM) {
                            distance_from_node_to_border_node = this->distanceMatrix->findDistance(
                                node_index,
                                border_node
                            );
                            distance_from_access_node_to_border_node = this->distanceMatrix->findDistance(
                                access_node,
                                border_node
                            );
                        }
                        else {
                            distance_from_node_to_border_node = distances_to_border_nodes.value()[bn_index][node_index];
                            // distance_from_access_node_to_border_node = this->all_transit_dm->findDistance(
                            //     border_node,
                            //     access_nodes_per_node[access_node_index].tnr_index
                            // );
                            distance_from_access_node_to_border_node = distances_to_border_nodes.value()[bn_index][
                                access_nodes_per_node[access_node_index].accessNodeID];
                        }

                        if(distance_from_node_to_border_node == distance_from_access_node_to_border_node +
                            distanceToAccessNode)
                        {
                            access_nodes_per_node[access_node_index].regionFlags[region_index] = true;
                            break;
                        }
                    }
                }
            }
        }


        // for (size_t i = 0; i < access_nodes.size(); i++) {
        //     unsigned int accessNode = access_nodes[i].accessNodeID;
        //     unsigned int distanceToAccessNode = access_nodes[i].distanceToNode;
        //
        //     std::optional<std::vector<unsigned int>> distancesFromAccessNode;
        //     if constexpr(mode == TNRAFPreprocessingMode::SLOW) {
        //         distancesFromAccessNode.emplace(originalGraph.nodes());
        //         if(forward_direction) {
        //             BasicDijkstra::computeOneToAllDistances(accessNode, originalGraph, distancesFromAccessNode.value());
        //         }
        //         else {
        //             BasicDijkstra::computeOneToAllDistancesInReversedGraph(
        //                 accessNode,
        //                 originalGraph,
        //                 distancesFromAccessNode.value()
        //             );
        //         }
        //     }
        //
        //     for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
        //         // if the region is the region of the access node, set the flag to true and continue
        //         if (regions.getRegion(accessNode) == j) {
        //             access_nodes[i].regionFlags[j] = true;
        //             continue;
        //         }
        //
        //         auto border_nodes_in_region = regions.getBorderNodes(j);
        //         for (unsigned int k = 0; k < border_nodes_in_region.size(); k++) {
        //             auto border_node = border_nodes_in_region[k];
        //             dist_t distance_from_node_to_border_node = 0;
        //             dist_t distance_from_access_node_to_border_node = 0;
        //
        //             if constexpr(mode == TNRAFPreprocessingMode::DM) {
        //                 distance_from_node_to_border_node = this->distanceMatrix->findDistance(node, border_node);
        //                 distance_from_access_node_to_border_node = this->distanceMatrix->findDistance(
        //                     accessNode,
        //                     border_nodes_in_region[k]
        //                 );
        //             }
        //             else if constexpr (mode == TNRAFPreprocessingMode::FAST) {
        //                 distance_from_node_to_border_node = optionalDistancesFromNode[border_node];
        //                 distance_from_access_node_to_border_node = this->all_transit_dm->findDistance(border_node, access_nodes[i].tnr_index);
        //             }
        //             else {
        //                 distance_from_node_to_border_node = optionalDistancesFromNode[border_node];
        //                 distance_from_access_node_to_border_node = distancesFromAccessNode.value()[border_node];
        //             }
        //             if (distance_from_node_to_border_node == distance_from_access_node_to_border_node + distanceToAccessNode) {
        //                 access_nodes[i].regionFlags[j] = true;
        //                 break;
        //             }
        //         }
        //     }
        // }
    }

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
     * @param clustersCnt[in] The number of clusters that need to be computed.
     */
    static Regions_with_borders generateClustering(
            Graph & originalGraph,
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

private:
	void generateDistanceMatrix(Graph& originalGraph, unsigned int dmIntSize, bool forward);
	DistanceMatrixInterface* distanceMatrix = nullptr;

	std::unique_ptr<DistanceMatrixInterface> all_transit_dm = nullptr;

    // Benchmarking timers
    std::chrono::milliseconds forward_dm_computation_time_ms_{0};
    std::chrono::milliseconds backward_dm_computation_time_ms_{0};
    std::chrono::milliseconds forward_access_nodes_computation_time_ms_{0};
    std::chrono::milliseconds backward_access_nodes_computation_time_ms_{0};

    template<typename DataType>
    void createAndFillAllToTransitDM(
	    Graph &originalGraph,
	    const std::vector<unsigned int>& transitNodes,
	    unsigned int transitNodesAmount,
	    // This is effectively the number of columns
	    const std::string& dataTypeForLog,
	    bool forward
    ) {
        unsigned int num_rows = originalGraph.nodes();
        unsigned int num_cols = transitNodesAmount;

        // 1. Create and populate the 2D matrix as before
        std::vector<std::vector<DataType>>* temp_2d_matrix = new std::vector<std::vector<DataType>>(num_rows, std::vector<DataType>(num_cols));
        std::vector<unsigned int> distances_per_one_transit_node(num_rows);

        for (unsigned int c = 0; c < num_cols; ++c) { // Iterate by column (transit node index)
            unsigned int transit_node_id = transitNodes[c];
            if(forward) {
            	BasicDijkstra::computeOneToAllDistancesInReversedGraph(transit_node_id, originalGraph, distances_per_one_transit_node);
            }
        	else {
        		BasicDijkstra::computeOneToAllDistances(transit_node_id, originalGraph, distances_per_one_transit_node);
        	}
            for (unsigned int r = 0; r < num_rows; ++r) {
                (*temp_2d_matrix)[r][c] = boost::numeric_cast<DataType>(distances_per_one_transit_node[r]);
            }
            if (c % 10 == 0 && num_cols > 10) {
                 std::cout << "\rProcessed " << c << "/" << num_cols << " transit nodes for all-to-transit DM (" << dataTypeForLog << ").";
            }
        }
        if (num_cols > 0) {
             std::cout << "\rProcessed " << num_cols << "/" << num_cols << " transit nodes for all-to-transit DM (" << dataTypeForLog << ").";
        }

        // 2. Convert the 2D matrix to a 1D std::unique_ptr<DataType[]>
        std::unique_ptr<DataType[]> one_d_array = std::make_unique<DataType[]>(static_cast<size_t>(num_rows) * num_cols);
        for (unsigned int r = 0; r < num_rows; ++r) {
            for (unsigned int c = 0; c < num_cols; ++c) {
                one_d_array[static_cast<size_t>(r) * num_cols + c] = (*temp_2d_matrix)[r][c];
            }
        }

        // 3. Clean up the temporary 2D matrix
        delete temp_2d_matrix;

        // 4. Create the provider using the 1D array
        all_transit_dm = std::make_unique<Distance_matrix_travel_time_provider<DataType>>(std::move(one_d_array), num_rows, num_cols);
    }

    void process_forward_access_nodes(
        unsigned int numNodes,
        std::vector<std::vector<AccessNodeDataArcFlags>>& accessNodesVec,
        std::vector<std::vector<unsigned int>>& searchSpacesVec,
        std::unordered_map<unsigned int, unsigned int>& transitNodesMap,
        FlagsGraph<NodeDataRegions>& chGraphInstance,
        Graph& origGraphInstance,
        Regions_with_borders& regionsInstance,
        TNRAFPreprocessingMode currentMode
    );

    void process_backward_access_nodes(
        unsigned int numNodes,
        std::vector<std::vector<AccessNodeDataArcFlags>>& accessNodesVec,
        std::vector<std::vector<unsigned int>>& searchSpacesVec,
        std::unordered_map<unsigned int, unsigned int>& transitNodesMap,
        FlagsGraph<NodeDataRegions>& chGraphInstance,
        Graph& origGraphInstance,
        Regions_with_borders& regionsInstance,
        TNRAFPreprocessingMode currentMode
    );

};

#endif //CONTRACTION_HIERARCHIES_TNRAFPREPROCESSOR_H

