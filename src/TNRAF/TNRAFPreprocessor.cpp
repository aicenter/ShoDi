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

#include <iostream>
#include <climits>
#include <fstream>
#include <boost/numeric/conversion/cast.hpp>
#include "../DistanceMatrix/DistanceMatrixComputorSlow.h"
#include "TNRAFPreprocessor.h"
#include "TNRAFPreprocessingMode.h"
#include "../CH/CHDistanceQueryManager.h"
#include "Structures/AccessNodeDataArcFlags.h"
#include "../Dijkstra/DijkstraNode.h"
#include "../Dijkstra/BasicDijkstra.h"
#include "../benchmark.h"

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::preprocessUsingCH(
	UpdateableGraph &graph,
	Graph &originalGraph,
	const std::string & outputPath,
	unsigned int transitNodesAmount,
	unsigned int regionsCnt,
	unsigned int dmIntSize,
	TNRAFPreprocessingMode mode
) {
	std::cout << "Getting transit nodes" << std::endl;
	std::vector<unsigned int> transitNodes(transitNodesAmount);
	graph.getNodesWithHighestRank(transitNodes, transitNodesAmount);

	std::cout << "Computing transit nodes distance table" << std::endl;
	FlagsGraph<NodeDataRegions> chGraph(graph);
	CHDistanceQueryManager qm(chGraph);

	// compute dm between transit nodes - this dm is computed in all modes
	std::vector<std::vector<unsigned int> > transitNodesDistanceTable(transitNodesAmount,std::vector<unsigned int>(transitNodesAmount));
	if (mode == TNRAFPreprocessingMode::DM) {
		std::cout
			<< "Computing the auxiliary distance matrix for transit node set distance matrix and access nodes forward direction."
			<< std::endl;
		this->forward_dm_computation_time_ms_ = benchmark<std::chrono::milliseconds>(
            &TNRAFPreprocessor::generateDistanceMatrix, this, std::ref(originalGraph), dmIntSize, true);
		std::cout << "Distance matrix computed." << std::endl;

		fillTransitNodeDistanceTable(transitNodes, transitNodesDistanceTable, transitNodesAmount);
	} else {
		computeTransitNodeDistanceTable(transitNodes, transitNodesDistanceTable, transitNodesAmount, originalGraph);
	}

	// compute dm from transit nodes to all nodes - this dm is computed only for fast mode
	if (mode == TNRAFPreprocessingMode::FAST) {
		std::cout << "Computing all-nodes to transit-nodes distance matrix (FAST mode, forward)." << std::endl;
        this->forward_dm_computation_time_ms_ = benchmark<std::chrono::milliseconds>([&]() {
            if (dmIntSize == 16) {
                createAndFillAllToTransitDM<uint_least16_t>(originalGraph, transitNodes, transitNodesAmount, "16-bit", true);
            } else if (dmIntSize == 32) {
                createAndFillAllToTransitDM<uint_least32_t>(originalGraph, transitNodes, transitNodesAmount, "32-bit", true);
            } else {
                createAndFillAllToTransitDM<dist_t>(originalGraph, transitNodes, transitNodesAmount, "default-bit", true);
            }
        });
		std::cout << "\nAll-nodes to transit-nodes distance matrix computed for " << transitNodesAmount << " transit nodes (forward)." << std::endl;
	}

	Regions_with_borders regions = generateClustering(originalGraph, regionsCnt);

	std::vector<std::vector<AccessNodeDataArcFlags> > forwardAccessNodes(graph.nodes());
	std::vector<std::vector<AccessNodeDataArcFlags> > backwardAccessNodes(graph.nodes());
	std::vector<std::vector<unsigned int> > forwardSearchSpaces(graph.nodes());
	std::vector<std::vector<unsigned int> > backwardSearchSpaces(graph.nodes());
	std::unordered_map<unsigned int, unsigned int> transitNodesMapping;
	for (unsigned int i = 0; i < transitNodesAmount; i++) {
		transitNodesMapping.insert(std::make_pair(transitNodes[i], i));
	}

    this->forward_access_nodes_computation_time_ms_ = benchmark<std::chrono::milliseconds>(
        &TNRAFPreprocessor::process_forward_access_nodes, this,
        originalGraph.nodes(), std::ref(forwardAccessNodes), std::ref(forwardSearchSpaces),
        std::ref(transitNodesMapping), std::ref(chGraph), std::ref(originalGraph), std::ref(regions), mode);

	// forward arc flags computation
	if(mode == TNRAFPreprocessingMode::DM) {
		compute_arc_flags<TNRAFPreprocessingMode::DM>(forwardAccessNodes, originalGraph, regions, true);
	}
	else if(mode == TNRAFPreprocessingMode::FAST) {
		compute_arc_flags<TNRAFPreprocessingMode::FAST>(forwardAccessNodes, originalGraph, regions, true);
	}
	// else {
	// 	// SLOW
	// 	compute_arc_flags<TNRAFPreprocessingMode::SLOW>(
	// 		source,
	// 		accessNodes,
	// 		originalGraph,
	// 		regions,
	// 		distancesFromNode,
	// 		true
	// 	);
	// }


	// BACKWARD ACCESS NODE COMPUTATION

	if (mode == TNRAFPreprocessingMode::DM) {
		std::cout << "Computing the auxiliary distance matrix for backward direction." << std::endl;
		delete distanceMatrix;
        distanceMatrix = nullptr; // Ensure it's null before potential reassignment
		this->backward_dm_computation_time_ms_ = benchmark<std::chrono::milliseconds>(
            &TNRAFPreprocessor::generateDistanceMatrix, this, std::ref(originalGraph), dmIntSize, false);
		std::cout << "Distance matrix computed." << std::endl;
	}

	// compute dm from all nodes to transit nodes - this dm is computed only for fast mode
	if (mode == TNRAFPreprocessingMode::FAST) {
		std::cout << "Computing all-nodes to transit-nodes distance matrix (FAST mode, backward)." << std::endl;
        this->backward_dm_computation_time_ms_ = benchmark<std::chrono::milliseconds>([&]() {
            if (dmIntSize == 16) {
                createAndFillAllToTransitDM<uint_least16_t>(originalGraph, transitNodes, transitNodesAmount, "16-bit", false);
            } else if (dmIntSize == 32) {
                createAndFillAllToTransitDM<uint_least32_t>(originalGraph, transitNodes, transitNodesAmount, "32-bit", false);
            } else {
                createAndFillAllToTransitDM<dist_t>(originalGraph, transitNodes, transitNodesAmount, "default-bit", false);
            }
        });
		std::cout << "\nAll-nodes to transit-nodes distance matrix computed for " << transitNodesAmount << " transit nodes (backward)." << std::endl;
	}

    this->backward_access_nodes_computation_time_ms_ = benchmark<std::chrono::milliseconds>(
        &TNRAFPreprocessor::process_backward_access_nodes, this,
        originalGraph.nodes(), std::ref(backwardAccessNodes), std::ref(backwardSearchSpaces),
        std::ref(transitNodesMapping), std::ref(chGraph), std::ref(originalGraph), std::ref(regions), mode);

	// backward arc flags computation
	if(mode == TNRAFPreprocessingMode::DM) {
		compute_arc_flags<TNRAFPreprocessingMode::DM>(backwardAccessNodes, originalGraph, regions, false);
	}
	else if(mode == TNRAFPreprocessingMode::FAST) {
		compute_arc_flags<TNRAFPreprocessingMode::FAST>(backwardAccessNodes, originalGraph, regions, false);
	}

	// FINAL STEPS
	if (mode == TNRAFPreprocessingMode::DM) {
		delete distanceMatrix;
		distanceMatrix = NULL;
	}

	std::vector<std::pair<unsigned int, QueryEdge> > allEdges;
	chGraph.getEdgesForFlushing(allEdges);

	outputGraph(outputPath, graph, allEdges, transitNodes, transitNodesDistanceTable, forwardAccessNodes,
				backwardAccessNodes, forwardSearchSpaces, backwardSearchSpaces, transitNodesAmount, regions,
				regionsCnt);
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::process_forward_access_nodes(
    unsigned int numNodes,
    std::vector<std::vector<AccessNodeDataArcFlags>>& accessNodesVec,
    std::vector<std::vector<unsigned int>>& searchSpacesVec,
    std::unordered_map<unsigned int, unsigned int>& transitNodesMap,
    FlagsGraph<NodeDataRegions>& chGraphInstance,
    Graph& origGraphInstance,
    Regions_with_borders& regionsInstance,
    TNRAFPreprocessingMode currentMode
) {
    for (unsigned int i = 0; i < numNodes; i++) {
        if (i % 100 == 0) {
            std::cout << "\rComputed forward access nodes for '" << i << "' nodes.";
        }
        find_forward_access_nodes_for_single_node(
	        i,
	        accessNodesVec[i],
	        searchSpacesVec[i],
	        transitNodesMap,
	        chGraphInstance,
	        origGraphInstance,
	        regionsInstance,
	        currentMode
        );
    }
    std::cout << "\rComputed forward access nodes for all nodes in the graph." << std::endl;
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::process_backward_access_nodes(
    unsigned int numNodes,
    std::vector<std::vector<AccessNodeDataArcFlags>>& accessNodesVec,
    std::vector<std::vector<unsigned int>>& searchSpacesVec,
    std::unordered_map<unsigned int, unsigned int>& transitNodesMap,
    FlagsGraph<NodeDataRegions>& chGraphInstance,
    Graph& origGraphInstance,
    Regions_with_borders& regionsInstance,
    TNRAFPreprocessingMode currentMode
) {
    for (unsigned int i = 0; i < numNodes; i++) {
        if (i % 100 == 0) {
            std::cout << "\rComputed backward access nodes for '" << i << "' nodes.";
        }
        find_backward_access_nodes_for_single_node(i, accessNodesVec[i], searchSpacesVec[i], transitNodesMap, chGraphInstance,
                                origGraphInstance, regionsInstance, currentMode);
    }
    std::cout << "\rComputed backward access nodes for all nodes in the graph." << std::endl;
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::generateDistanceMatrix(Graph& originalGraph, unsigned int dmIntSize, bool forward) {
	if (forward) {
		if (dmIntSize == 16) {
			DistanceMatrixComputorSlow<uint_least16_t> dmComputor;
			dmComputor.computeDistanceMatrix(originalGraph);
			distanceMatrix = new Distance_matrix_travel_time_provider(dmComputor.getDistanceMatrixInstance(), originalGraph.nodes());
		} else if (dmIntSize == 32) {
			DistanceMatrixComputorSlow<uint_least32_t> dmComputor;
			dmComputor.computeDistanceMatrix(originalGraph);
			distanceMatrix = new Distance_matrix_travel_time_provider(dmComputor.getDistanceMatrixInstance(), originalGraph.nodes());
		} else {
			DistanceMatrixComputorSlow<dist_t> dmComputor;
			dmComputor.computeDistanceMatrix(originalGraph);
			distanceMatrix = new Distance_matrix_travel_time_provider(dmComputor.getDistanceMatrixInstance(), originalGraph.nodes());
		}
	} else {
		if (dmIntSize == 16) {
			DistanceMatrixComputorSlow<uint_least16_t> dmComputor;
			dmComputor.computeDistanceMatrixInReversedGraph(originalGraph);
			distanceMatrix = new Distance_matrix_travel_time_provider(dmComputor.getDistanceMatrixInstance(), originalGraph.nodes());
		} else if (dmIntSize == 32) {
			DistanceMatrixComputorSlow<uint_least32_t> dmComputor;
			dmComputor.computeDistanceMatrixInReversedGraph(originalGraph);
			distanceMatrix = new Distance_matrix_travel_time_provider(dmComputor.getDistanceMatrixInstance(), originalGraph.nodes());
		} else {
			DistanceMatrixComputorSlow<dist_t> dmComputor;
			dmComputor.computeDistanceMatrixInReversedGraph(originalGraph);
			distanceMatrix = new Distance_matrix_travel_time_provider(dmComputor.getDistanceMatrixInstance(), originalGraph.nodes());
		}
	}
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::computeTransitNodeDistanceTable(
	std::vector<unsigned int> &transitNodes,
	std::vector<std::vector<unsigned int>> &distanceTable,
	unsigned int transitNodesCnt, Graph &originalGraph
) {
	for (unsigned int i = 0; i < transitNodesCnt; i++) {
		if (i % 100 == 0) {
			std::cout << "\rComputed '" << i << "' transit nodes distance table rows.";
		}

		std::vector<unsigned int> distancesFromNodeI(originalGraph.nodes());
		BasicDijkstra::computeOneToAllDistances(transitNodes[i], originalGraph, distancesFromNodeI);
		for (unsigned int j = 0; j < transitNodesCnt; j++) {
			distanceTable[i][j] = distancesFromNodeI[transitNodes[j]];
		}
	}

	std::cout << "\rComputed the transit nodes distance table." << std::endl;
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::fillTransitNodeDistanceTable(
	std::vector<unsigned int> &transitNodes,
	std::vector<std::vector<dist_t>> &distanceTable,
	unsigned int transitNodesCnt
) {
	for (unsigned int i = 0; i < transitNodesCnt; i++) {
		for (unsigned int j = 0; j < transitNodesCnt; j++) {
			distanceTable[i][j] = distanceMatrix->findDistance(transitNodes[i], transitNodes[j]);
		}
	}
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::outputGraph(
	std::string outputPath, UpdateableGraph &graph,
	std::vector<std::pair<unsigned int, QueryEdge> > &allEdges,
	std::vector<unsigned int> &transitNodes,
	std::vector<std::vector<unsigned int> > &transitNodesDistanceTable,
	std::vector<std::vector<AccessNodeDataArcFlags> > &forwardAccessNodes,
	std::vector<std::vector<AccessNodeDataArcFlags> > &backwardAccessNodes,
	std::vector<std::vector<unsigned int> > &forwardSearchSpaces,
	std::vector<std::vector<unsigned int> > &backwardSearchSpaces,
	unsigned int transitNodesAmount, Regions_with_borders &regions,
	unsigned int regionsCnt
) {
	std::cout << "Outputting TNRAF" << std::endl;
	std::ofstream output;
	output.open(outputPath + ".tgaf", std::ios::binary);
	if (!output.is_open()) {
		printf("Couldn't open file '%s'!", (outputPath + ".tgaf").c_str());
	}

	// Output the header consisting of a predefined std::string, and then the numbers of nodes, edges, the size of the
	// transit node set and the number of regions.
	char c1, c2, c3, c4;
	c1 = 'T';
	c2 = 'G';
	c3 = 'A';
	c4 = 'F';
	output.write(&c1, sizeof(c1));
	output.write(&c2, sizeof(c2));
	output.write(&c3, sizeof(c3));
	output.write(&c4, sizeof(c4));
	unsigned int nodes = graph.nodes();
	// TODO endes as unsigned long or unsigned long long
	unsigned int edges = boost::numeric_cast<unsigned int>(allEdges.size());
	output.write((char *) &nodes, sizeof(nodes));
	output.write((char *) &edges, sizeof(edges));
	output.write((char *) &transitNodesAmount, sizeof(transitNodesAmount));
	output.write((char *) &regionsCnt, sizeof(regionsCnt));

	// Output all edges, this means the original edges as well as the shorctut edges.
	for (size_t i = 0; i < allEdges.size(); i++) {
		output.write((char *) &allEdges[i].first, sizeof(allEdges[i].first));
		output.write((char *) &allEdges[i].second.targetNode, sizeof(allEdges[i].second.targetNode));
		unsigned int weight = allEdges[i].second.weight;
		output.write((char *) &weight, sizeof(weight));
		output.write((char *) &allEdges[i].second.forward, sizeof(allEdges[i].second.forward));
		output.write((char *) &allEdges[i].second.backward, sizeof(allEdges[i].second.backward));
		bool t = true;
		bool f = false;
		if (allEdges[i].second.forward && graph.isShortcut(allEdges[i].first, allEdges[i].second.targetNode)) {
			output.write((char *) &t, sizeof(t));
		} else {
			output.write((char *) &f, sizeof(f));
		}

		if (allEdges[i].second.backward && graph.isShortcut(allEdges[i].second.targetNode, allEdges[i].first)) {
			output.write((char *) &t, sizeof(t));
		} else {
			output.write((char *) &f, sizeof(f));
		}
	}

	// Output ranks for all nodes in the graph.
	for (unsigned int i = 0; i < graph.nodes(); i++) {
		unsigned int rank = graph.getRank(i);
		output.write((char *) &rank, sizeof(rank));
	}

	// Output regions for all nodes in the graph (used for Arc Flags).
	for (unsigned int i = 0; i < graph.nodes(); i++) {
		unsigned int region = regions.getRegion(i);
		output.write((char *) &region, sizeof(region));
	}

	// Output the transit nodes ids and then the full transit node set distance matrix (distances between all pairs of
	// transit nodes).
	for (unsigned int i = 0; i < transitNodesAmount; i++) {
		output.write((char *) &transitNodes[i], sizeof(transitNodes[i]));
	}
	for (unsigned int i = 0; i < transitNodesAmount; i++) {
		for (unsigned int j = 0; j < transitNodesAmount; j++) {
			output.write((char *) &transitNodesDistanceTable[i][j], sizeof(transitNodesDistanceTable[i][j]));
		}
	}

	// Output the access nodes (their arc flags are also output here).
	std::vector<uint32_t> powersOf2(regionsCnt);
	initPowersOf2(powersOf2);
	for (unsigned int i = 0; i < graph.nodes(); i++) {
		unsigned int fwSize = boost::numeric_cast<unsigned int>(forwardAccessNodes[i].size());
		output.write((char *) &fwSize, sizeof(fwSize));
		for (unsigned int j = 0; j < fwSize; j++) {
			output.write((char *) &forwardAccessNodes[i][j].accessNodeID,
						 sizeof(forwardAccessNodes[i][j].accessNodeID));
			output.write((char *) &forwardAccessNodes[i][j].distanceToNode,
						 sizeof(forwardAccessNodes[i][j].distanceToNode));
			uint32_t regionsOutput = 0;
			for (unsigned int k = 0; k < regionsCnt; k++) {
				if (forwardAccessNodes[i][j].regionFlags[k]) {
					regionsOutput = regionsOutput | powersOf2[k];
				}
			}
			output.write((char *) &regionsOutput, sizeof(regionsOutput));
		}
		unsigned int bwSize = boost::numeric_cast<unsigned int>(backwardAccessNodes[i].size());
		output.write((char *) &bwSize, sizeof(bwSize));
		for (unsigned int j = 0; j < bwSize; j++) {
			output.write((char *) &backwardAccessNodes[i][j].accessNodeID,
						 sizeof(backwardAccessNodes[i][j].accessNodeID));
			output.write((char *) &backwardAccessNodes[i][j].distanceToNode,
						 sizeof(backwardAccessNodes[i][j].distanceToNode));
			uint32_t regionsOutput = 0;
			for (unsigned int k = 0; k < regionsCnt; k++) {
				if (backwardAccessNodes[i][j].regionFlags[k]) {
					regionsOutput = regionsOutput | powersOf2[k];
				}
			}

			output.write((char *) &regionsOutput, sizeof(regionsOutput));
		}
	}

	// Output search spaces (those are needed for the locality filter).
	printf("Will now output search spaces.\n");
	unsigned int fwSearchSpaceSum = 0;
	unsigned int bwSearchSpaceSum = 0;
	for (unsigned int i = 0; i < graph.nodes(); i++) {
		unsigned int fwSearchSpaceSize = boost::numeric_cast<unsigned int>(forwardSearchSpaces[i].size());
		output.write((char *) &fwSearchSpaceSize, sizeof(fwSearchSpaceSize));
		for (unsigned int j = 0; j < fwSearchSpaceSize; j++) {
			output.write((char *) &forwardSearchSpaces[i][j], sizeof(forwardSearchSpaces[i][j]));
		}
		fwSearchSpaceSum += fwSearchSpaceSize;

		unsigned int bwSearchSpaceSize = boost::numeric_cast<unsigned int>(backwardSearchSpaces[i].size());
		output.write((char *) &bwSearchSpaceSize, sizeof(bwSearchSpaceSize));
		for (unsigned int j = 0; j < bwSearchSpaceSize; j++) {
			output.write((char *) &backwardSearchSpaces[i][j], sizeof(backwardSearchSpaces[i][j]));
		}
		bwSearchSpaceSum += bwSearchSpaceSize;

	}

	printf("Average forward search space size: %lf\n", (double) fwSearchSpaceSum / nodes);
	printf("Average backward search space size: %lf\n", (double) bwSearchSpaceSum / nodes);

	output.close();
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::find_forward_access_nodes_for_single_node(
	unsigned int source,
	std::vector<AccessNodeDataArcFlags>& accessNodes,
	std::vector<unsigned int>& forwardSearchSpace,
	std::unordered_map<unsigned int, unsigned int>& transitNodes,
	FlagsGraph<NodeDataRegions>& graph,
	Graph& originalGraph,
	Regions_with_borders& regions,
	TNRAFPreprocessingMode mode
) {
	auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight); };
	std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> forwardQ(cmp);
	std::vector<unsigned int> distances(graph.nodes(), UINT_MAX);
	std::vector<bool> settled(graph.nodes(), false);

	forwardQ.push(DijkstraNode(source, 0));

	distances[source] = 0;

	std::vector<AccessNodeDataArcFlags> accessNodesSuperset;

	while (!forwardQ.empty()) {
		unsigned int curNode = forwardQ.top().ID;
		unsigned int curLen = forwardQ.top().weight;
		forwardQ.pop();

		if (settled[curNode]) {
			continue;
		}

		settled[curNode] = true;
		if (transitNodes.contains(curNode)) {
			accessNodesSuperset.push_back(
				AccessNodeDataArcFlags(curNode, curLen, regions.getRegionsCnt(), static_cast<unsigned short>(transitNodes.at(curNode))
				)
			);
		} else {
			forwardSearchSpace.push_back(curNode);

			const std::vector<QueryEdge> &neighbours = graph.nextNodes(curNode);
			for (auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
				// Skip edge if it is only in the other direction.
				if (!(*iter).forward) {
					continue;
				}

				if (settled[(*iter).targetNode]) {
					continue;
				}

				// This is basically the dijkstra edge relaxation process. Additionaly, we unstall the node
				// if it was stalled previously, because it might be now reached on the optimal path.
				if (graph.data((*iter).targetNode).rank > graph.data(curNode).rank) {
					unsigned int newlen = curLen + (*iter).weight;
					if (newlen < distances[(*iter).targetNode]) {
						distances[(*iter).targetNode] = newlen;
						forwardQ.push(DijkstraNode((*iter).targetNode, newlen));
					}
				}
			}
		}

	}

	std::vector<unsigned int> distancesFromNode;
	if (mode == TNRAFPreprocessingMode::SLOW) {
		distancesFromNode.resize(originalGraph.nodes());
		BasicDijkstra::computeOneToAllDistances(source, originalGraph, distancesFromNode);
	}
	for (size_t i = 0; i < accessNodesSuperset.size(); i++) {
		auto access_node = accessNodesSuperset[i];
		unsigned int access_node_id = access_node.accessNodeID;
		unsigned int access_node_distance = access_node.distanceToNode;

		if (mode == TNRAFPreprocessingMode::DM) {
			unsigned int realDistance = distanceMatrix->findDistance(source, access_node_id);
			if (realDistance == access_node_distance) {
				accessNodes.push_back(accessNodesSuperset[i]);
			}
		}
		else if (mode == TNRAFPreprocessingMode::FAST) {
			unsigned int realDistance = all_transit_dm->findDistance(source, access_node.tnr_index);
			if (realDistance == access_node_distance) {
				accessNodes.push_back(accessNodesSuperset[i]);
			}
		}
		else {
			unsigned int realDistance = distancesFromNode[access_node_id];
			if (realDistance == access_node_distance) {
				accessNodes.push_back(accessNodesSuperset[i]);
			}
		}
	}
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::find_backward_access_nodes_for_single_node(
	unsigned int source, std::vector<AccessNodeDataArcFlags> &accessNodes,
	std::vector<unsigned int> &backwardSearchSpace,
	std::unordered_map<unsigned int, unsigned int> &transitNodes,
	FlagsGraph<NodeDataRegions>& graph, Graph &originalGraph, Regions_with_borders &regions,
	TNRAFPreprocessingMode useDistanceMatrix
) {
	auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight); };
	std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> backwardQ(cmp);
	std::vector<unsigned int> distances(graph.nodes(), UINT_MAX);
	std::vector<bool> settled(graph.nodes(), false);

	backwardQ.push(DijkstraNode(source, 0));

	distances[source] = 0;

	std::vector<AccessNodeDataArcFlags> accessNodesSuperset;

	while (!backwardQ.empty()) {
		unsigned int curNode = backwardQ.top().ID;
		unsigned int curLen = backwardQ.top().weight;
		backwardQ.pop();

		if (settled[curNode]) {
			continue;
		}

		settled[curNode] = true;
		if (transitNodes.contains(curNode)) {
			accessNodesSuperset.push_back(
				AccessNodeDataArcFlags(curNode, curLen, regions.getRegionsCnt(), static_cast<unsigned short>(transitNodes.at(curNode))
				)
			);
		} else {
			backwardSearchSpace.push_back(curNode);

			const std::vector<QueryEdge> &neighbours = graph.nextNodes(curNode);
			for (auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
				// Skip edge if it is only in the other direction.
				if (!(*iter).backward) {
					continue;
				}

				if (settled[(*iter).targetNode]) {
					continue;
				}

				// This is basically the dijkstra edge relaxation process. Additionaly, we unstall the node
				// if it was stalled previously, because it might be now reached on the optimal path.
				if (graph.data((*iter).targetNode).rank > graph.data(curNode).rank) {
					unsigned int newlen = curLen + (*iter).weight;
					if (newlen < distances[(*iter).targetNode]) {
						distances[(*iter).targetNode] = newlen;
						backwardQ.push(DijkstraNode((*iter).targetNode, newlen));
					}
				}
			}
		}

	}

	std::vector<unsigned int> distancesFromNode;
	if (useDistanceMatrix == TNRAFPreprocessingMode::SLOW) {
		distancesFromNode.resize(originalGraph.nodes());
		BasicDijkstra::computeOneToAllDistancesInReversedGraph(source, originalGraph, distancesFromNode);
	}
	for (size_t i = 0; i < accessNodesSuperset.size(); i++) {
		auto access_node = accessNodesSuperset[i];
		unsigned int access_node_id = access_node.accessNodeID;
		unsigned int access_node_distance = access_node.distanceToNode;

		if (useDistanceMatrix == TNRAFPreprocessingMode::DM) {
			unsigned int realDistance = distanceMatrix->findDistance(source, access_node_id);
			if (realDistance == access_node_distance) {
				accessNodes.emplace_back(accessNodesSuperset[i]);
			}
		}
		else if (useDistanceMatrix == TNRAFPreprocessingMode::FAST) {
			unsigned int realDistance = all_transit_dm->findDistance(source, access_node.tnr_index);
			if (realDistance == access_node_distance) {
				accessNodes.emplace_back(accessNodesSuperset[i]);
			}
		}
		else {
			unsigned int realDistance = distancesFromNode[access_node_id];
			if (realDistance == access_node_distance) {
				accessNodes.emplace_back(accessNodesSuperset[i]);
			}
		}
	}
}

//______________________________________________________________________________________________________________________
Regions_with_borders TNRAFPreprocessor::generateClustering(Graph& originalGraph, unsigned int clustersCnt) {
	auto nodesCnt = originalGraph.nodes();
	auto approxNodesPerCluster = nodesCnt / clustersCnt;
	auto unassignedNodes = nodesCnt - clustersCnt;
	std::vector<unsigned int> assignedClusters(nodesCnt, UINT_MAX);
	std::vector<std::queue<unsigned int> > q(clustersCnt, std::queue<unsigned int>());

	for (unsigned int i = 0; i < clustersCnt; ++i) {
		const unsigned int nodeID = approxNodesPerCluster * i;
		assignedClusters[nodeID] = i;
		const std::vector<std::pair<unsigned int, unsigned int>> &neighboursFW = originalGraph.outgoingEdges(nodeID);
		for (size_t j = 0; j < neighboursFW.size(); ++j) {
			q[i].push(neighboursFW[j].first);
		}
		const std::vector<std::pair<unsigned int, unsigned int>> &neighboursBW = originalGraph.incomingEdges(nodeID);
		for (size_t j = 0; j < neighboursBW.size(); ++j) {
			q[i].push(neighboursBW[j].first);
		}
	}

	bool jumpout = unassignedNodes == 0;
	while (!jumpout) {
		for (unsigned int i = 0; i < clustersCnt; ++i) {
			const unsigned int newNodeForCluster = getNewNodeForCluster(assignedClusters, q[i]);

			assignedClusters[newNodeForCluster] = i;
			const std::vector<std::pair<unsigned int, unsigned int>> &neighboursFW = originalGraph.outgoingEdges(
				newNodeForCluster);
			for (size_t j = 0; j < neighboursFW.size(); ++j) {
				q[i].push(neighboursFW[j].first);
			}
			const std::vector<std::pair<unsigned int, unsigned int>> &neighboursBW = originalGraph.incomingEdges(
				newNodeForCluster);
			for (size_t j = 0; j < neighboursBW.size(); ++j) {
				q[i].push(neighboursBW[j].first);
			}

			unassignedNodes--;
			if (unassignedNodes == 0) {
				jumpout = true;
				break;
			}

			if (unassignedNodes % 2000 == 0) {
				printf("\rThere are %u nodes left to be assigned to clusters.", unassignedNodes);
			}
		}
	}

	printf("\rClustering for the Arc Flags computed.\n");


	return Regions_with_borders(assignedClusters, clustersCnt, originalGraph);
}

//______________________________________________________________________________________________________________________
unsigned int
TNRAFPreprocessor::getNewNodeForCluster(std::vector<unsigned int> &assignedClusters, std::queue<unsigned int> &q) {
	while (!q.empty()) {
		unsigned int nodeID = q.front();
		q.pop();
		if (assignedClusters[nodeID] == UINT_MAX) {
			return nodeID;
		}
	}

	for (unsigned int i = 0; i < boost::numeric_cast<unsigned int>(assignedClusters.size()); ++i) {
		if (assignedClusters[i] == UINT_MAX) {
			return i;
		}
	}

	return UINT_MAX;
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::getPowersOf2(std::vector<uint32_t> &powersOf2) {
	initPowersOf2(powersOf2);
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::initPowersOf2(std::vector<uint32_t> &powersOf2) {
	uint32_t val = 1;
	for (unsigned int i = 0; i < powersOf2.size(); i++) {
		powersOf2[i] = val;
		val = val * 2;
	}
}
