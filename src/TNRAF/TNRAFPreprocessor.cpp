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
#include "../CH/CHDistanceQueryManager.h"
#include "Structures/AccessNodeDataArcFlags.h"
#include "../Dijkstra/DijkstraNode.h"
#include "../Dijkstra/BasicDijkstra.h"

Distance_matrix_travel_time_provider<dist_t>* TNRAFPreprocessor::distanceMatrix = NULL;

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::preprocessUsingCH(
	UpdateableGraph &graph,
	Graph &originalGraph,
	std::string outputPath,
	unsigned int transitNodesAmount,
	unsigned int regionsCnt,
	bool useDistanceMatrix
) {
	std::cout << "Getting transit nodes" << std::endl;
	std::vector<unsigned int> transitNodes(transitNodesAmount);
	graph.getNodesWithHighestRank(transitNodes, transitNodesAmount);

	std::cout << "Computing transit nodes distance table" << std::endl;
	FlagsGraph<NodeDataRegions> chGraph(graph);
	CHDistanceQueryManager qm(chGraph);
	std::vector<std::vector<unsigned int> > transitNodesDistanceTable(transitNodesAmount,
																	  std::vector<unsigned int>(transitNodesAmount));
	if (useDistanceMatrix) {
		std::cout
			<< "Computing the auxiliary distance matrix for transit node set distance matrix and access nodes forward direction."
			<< std::endl;
		DistanceMatrixComputorSlow dmComputor;
		dmComputor.computeDistanceMatrix(originalGraph);
		distanceMatrix = dmComputor.getDistanceMatrixInstance();
		std::cout << "Distance matrix computed." << std::endl;

		fillTransitNodeDistanceTable(transitNodes, transitNodesDistanceTable, transitNodesAmount);
	} else {
		computeTransitNodeDistanceTable(transitNodes, transitNodesDistanceTable, transitNodesAmount, originalGraph);
	}

	RegionsStructure regions(graph.nodes(), regionsCnt);
	generateClustering(originalGraph, regions, regionsCnt);

	std::vector<std::vector<AccessNodeDataArcFlags> > forwardAccessNodes(graph.nodes());
	std::vector<std::vector<AccessNodeDataArcFlags> > backwardAccessNodes(graph.nodes());
	std::vector<std::vector<unsigned int> > forwardSearchSpaces(graph.nodes());
	std::vector<std::vector<unsigned int> > backwardSearchSpaces(graph.nodes());
	std::unordered_map<unsigned int, unsigned int> transitNodesMapping;
	for (unsigned int i = 0; i < transitNodesAmount; i++) {
		transitNodesMapping.insert(std::make_pair(transitNodes[i], i));
	}

	for (unsigned int i = 0; i < graph.nodes(); i++) {
		if (i % 100 == 0) {
			std::cout << "\rComputed forward access nodes for '" << i << "' nodes.";
		}

		findForwardAccessNodes(i, forwardAccessNodes[i], forwardSearchSpaces[i], transitNodesMapping, chGraph,
							   originalGraph, regions, useDistanceMatrix);
	}

	std::cout << "\rComputed forward access nodes for all nodes in the graph." << std::endl;

	if (useDistanceMatrix) {
		std::cout << "Computing the auxiliary distance matrix for backward direction." << std::endl;
		delete distanceMatrix;
		DistanceMatrixComputorSlow dmComputor;
		dmComputor.computeDistanceMatrixInReversedGraph(originalGraph);
		distanceMatrix = dmComputor.getDistanceMatrixInstance();
		std::cout << "Distance matrix computed." << std::endl;
	}

	for (unsigned int i = 0; i < graph.nodes(); i++) {
		if (i % 100 == 0) {
			std::cout << "\rComputed backward access nodes for '" << i << "' nodes.";
		}

		findBackwardAccessNodes(i, backwardAccessNodes[i], backwardSearchSpaces[i], transitNodesMapping, chGraph,
								originalGraph, regions, useDistanceMatrix);
	}

	std::cout << "\rComputed backward access nodes for all nodes in the graph." << std::endl;

	if (useDistanceMatrix) {
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
	std::vector<std::vector<unsigned int>> &distanceTable,
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
	unsigned int transitNodesAmount, RegionsStructure &regions,
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
void TNRAFPreprocessor::findForwardAccessNodes(
	unsigned int source, std::vector<AccessNodeDataArcFlags> &accessNodes,
	std::vector<unsigned int> &forwardSearchSpace,
	std::unordered_map<unsigned int, unsigned int> &transitNodes,
	FlagsGraph<NodeDataRegions>& graph, Graph &originalGraph, RegionsStructure &regions,
	bool useDistanceMatrix
) {
	auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight); };
	std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> forwardQ(cmp);
	std::vector<unsigned int> distances(graph.nodes(), UINT_MAX);
	std::vector<bool> settled(graph.nodes(), false);

	forwardQ.push(DijkstraNode(source, 0));

	distances[source] = 0;

	std::vector<AccessNodeData> accessNodesSuperset;

	while (!forwardQ.empty()) {
		unsigned int curNode = forwardQ.top().ID;
		unsigned int curLen = forwardQ.top().weight;
		forwardQ.pop();

		if (settled[curNode]) {
			continue;
		}

		settled[curNode] = true;
		if (transitNodes.count(curNode) == 1) {
			accessNodesSuperset.push_back(AccessNodeData(curNode, curLen));
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
	if (!useDistanceMatrix) {
		distancesFromNode.resize(originalGraph.nodes());
		BasicDijkstra::computeOneToAllDistances(source, originalGraph, distancesFromNode);
	}
	for (size_t i = 0; i < accessNodesSuperset.size(); i++) {
		if (useDistanceMatrix) {
			unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
			unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
			unsigned int realDistance = distanceMatrix->findDistance(source, accessNode);
			if (realDistance == accessNodeDistance) {
				accessNodes.push_back(AccessNodeDataArcFlags(accessNodesSuperset[i].accessNodeID,
															 accessNodesSuperset[i].distanceToNode,
															 regions.getRegionsCnt()));
			}
		} else {
			unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
			unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
			unsigned int realDistance = distancesFromNode[accessNode];
			if (realDistance == accessNodeDistance) {
				accessNodes.push_back(AccessNodeDataArcFlags(accessNodesSuperset[i].accessNodeID,
															 accessNodesSuperset[i].distanceToNode,
															 regions.getRegionsCnt()));
			}
		}
	}

	computeForwardArcFlags(source, accessNodes, originalGraph, regions, useDistanceMatrix, distancesFromNode);

}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::computeForwardArcFlags(
	unsigned int node, std::vector<AccessNodeDataArcFlags> &accessNodes,
	Graph &originalGraph, RegionsStructure &regions, bool useDistanceMatrix,
	std::vector<unsigned int> &optionalDistancesFromNode
) {
	if (useDistanceMatrix) {
		for (size_t i = 0; i < accessNodes.size(); i++) {
			unsigned int accessNode = accessNodes[i].accessNodeID;
			unsigned int distanceToAccessNode = accessNodes[i].distanceToNode;
			for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
				std::vector<unsigned int> &nodesInRegion = regions.nodesInRegion(j);
				for (unsigned int k = 0; k < nodesInRegion.size(); k++) {
					if (distanceMatrix->findDistance(node, nodesInRegion[k]) ==
						distanceMatrix->findDistance(accessNode, nodesInRegion[k]) + distanceToAccessNode) {
						accessNodes[i].regionFlags[j] = true;
						break;
					}
				}
			}
		}
	} else {
		for (size_t i = 0; i < accessNodes.size(); i++) {
			std::vector<unsigned int> distancesFromAccessNode(originalGraph.nodes());
			unsigned int accessNode = accessNodes[i].accessNodeID;
			BasicDijkstra::computeOneToAllDistances(accessNode, originalGraph, distancesFromAccessNode);
			unsigned int distanceToAccessNode = optionalDistancesFromNode[accessNode];
			for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
				std::vector<unsigned int> &nodesInRegion = regions.nodesInRegion(j);
				for (size_t k = 0; k < nodesInRegion.size(); k++) {
					if (optionalDistancesFromNode[nodesInRegion[k]] ==
						distancesFromAccessNode[nodesInRegion[k]] + distanceToAccessNode) {
						accessNodes[i].regionFlags[j] = true;
						break;
					}
				}
			}
		}
	}
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::findBackwardAccessNodes(
	unsigned int source, std::vector<AccessNodeDataArcFlags> &accessNodes,
	std::vector<unsigned int> &backwardSearchSpace,
	std::unordered_map<unsigned int, unsigned int> &transitNodes,
	FlagsGraph<NodeDataRegions>& graph, Graph &originalGraph, RegionsStructure &regions,
	bool useDistanceMatrix
) {
	auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight); };
	std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> backwardQ(cmp);
	std::vector<unsigned int> distances(graph.nodes(), UINT_MAX);
	std::vector<bool> settled(graph.nodes(), false);

	backwardQ.push(DijkstraNode(source, 0));

	distances[source] = 0;

	std::vector<AccessNodeData> accessNodesSuperset;

	while (!backwardQ.empty()) {
		unsigned int curNode = backwardQ.top().ID;
		unsigned int curLen = backwardQ.top().weight;
		backwardQ.pop();

		if (settled[curNode]) {
			continue;
		}

		settled[curNode] = true;
		if (transitNodes.count(curNode) == 1) {
			accessNodesSuperset.push_back(AccessNodeData(curNode, curLen));
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
	if (!useDistanceMatrix) {
		distancesFromNode.resize(originalGraph.nodes());
		BasicDijkstra::computeOneToAllDistancesInReversedGraph(source, originalGraph, distancesFromNode);
	}
	for (size_t i = 0; i < accessNodesSuperset.size(); i++) {
		if (useDistanceMatrix) {
			unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
			unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
			unsigned int realDistance = distanceMatrix->findDistance(source, accessNode);
			if (realDistance == accessNodeDistance) {
				accessNodes.push_back(AccessNodeDataArcFlags(accessNodesSuperset[i].accessNodeID,
															 accessNodesSuperset[i].distanceToNode,
															 regions.getRegionsCnt()));
			}
		} else {
			unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
			unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
			unsigned int realDistance = distancesFromNode[accessNode];
			if (realDistance == accessNodeDistance) {
				accessNodes.push_back(AccessNodeDataArcFlags(accessNodesSuperset[i].accessNodeID,
															 accessNodesSuperset[i].distanceToNode,
															 regions.getRegionsCnt()));
			}
		}
	}

	computeBackwardArcFlags(source, accessNodes, originalGraph, regions, useDistanceMatrix, distancesFromNode);

}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::computeBackwardArcFlags(
	unsigned int node, std::vector<AccessNodeDataArcFlags> &accessNodes,
	Graph &originalGraph, RegionsStructure &regions, bool useDistanceMatrix,
	std::vector<unsigned int> &optionalDistancesFromNode
) {
	if (useDistanceMatrix) {
		for (size_t i = 0; i < accessNodes.size(); i++) {
			unsigned int accessNode = accessNodes[i].accessNodeID;
			unsigned int distanceToAccessNode = accessNodes[i].distanceToNode;
			for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
				std::vector<unsigned int> &nodesInRegion = regions.nodesInRegion(j);
				for (size_t k = 0; k < nodesInRegion.size(); k++) {
					if (distanceMatrix->findDistance(node, nodesInRegion[k]) ==
						distanceMatrix->findDistance(accessNode, nodesInRegion[k]) + distanceToAccessNode) {
						accessNodes[i].regionFlags[j] = true;
						break;
					}
				}
			}
		}
	} else {
		for (size_t i = 0; i < accessNodes.size(); i++) {
			std::vector<unsigned int> distancesFromAccessNode(originalGraph.nodes());
			unsigned int accessNode = accessNodes[i].accessNodeID;
			BasicDijkstra::computeOneToAllDistancesInReversedGraph(accessNode, originalGraph,
																   distancesFromAccessNode);
			unsigned int distanceToAccessNode = optionalDistancesFromNode[accessNode];
			for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
				std::vector<unsigned int> &nodesInRegion = regions.nodesInRegion(j);
				for (size_t k = 0; k < nodesInRegion.size(); k++) {
					if (optionalDistancesFromNode[nodesInRegion[k]] ==
						distancesFromAccessNode[nodesInRegion[k]] + distanceToAccessNode) {
						accessNodes[i].regionFlags[j] = true;
						break;
					}
				}
			}
		}
	}
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::generateClustering(Graph &originalGraph, RegionsStructure &regions, unsigned int clustersCnt) {
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

	for (unsigned int i = 0; i < nodesCnt; ++i) {
		regions.addNode(i, assignedClusters[i]);
	}
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

