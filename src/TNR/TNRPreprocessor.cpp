//
// Author: Xenty (Michal Cvach)
// Created on: 05.08.19
//

#include <climits>
#include <fstream>
#include <iostream>
#include <boost/numeric/conversion/cast.hpp>
#include "TNRPreprocessor.h"
#include "../GraphBuilding/Structures/FlagsGraph.h"
#include "../CH/CHDistanceQueryManager.h"
#include "Structures/AccessNodeData.h"
#include "../Dijkstra/DijkstraNode.h"
#include "../DistanceMatrix/Distance_matrix_travel_time_provider.h"
#include "../DistanceMatrix/DistanceMatrixComputorSlow.h"
#include "../Dijkstra/BasicDijkstra.h"

//______________________________________________________________________________________________________________________
void
TNRPreprocessor::preprocessUsingCH(UpdateableGraph &graph, std::string outputPath, unsigned int transitNodesAmount) {
    std::cout << "Getting transit nodes" << std::endl;
    std::vector<unsigned int> transitNodes(transitNodesAmount);
    graph.getNodesWithHighestRank(transitNodes, transitNodesAmount);

    std::cout << "Computing transit nodes distance table" << std::endl;
    FlagsGraph chGraph(graph);
    CHDistanceQueryManager qm(chGraph);
    std::vector<std::vector<unsigned int> > transitNodesDistanceTable(transitNodesAmount,
                                                                      std::vector<unsigned int>(transitNodesAmount));
    for (unsigned int i = 0; i < transitNodesAmount; i++) {
        if (i % 100 == 0) {
            std::cout << "\rComputed '" << i << "' transit nodes distance table rows.";
        }
        for (unsigned int j = 0; j < transitNodesAmount; j++) {
            if (i == j) {
                transitNodesDistanceTable[i][j] = 0;
            } else {
                transitNodesDistanceTable[i][j] = qm.findDistance(transitNodes[i], transitNodes[j]);
            }
        }
    }

    std::cout << "\rComputed the transit nodes distance table." << std::endl;

    std::cout << "Computing access nodes" << std::endl;
    std::vector<std::vector<AccessNodeData> > forwardAccessNodes(graph.nodes());
    std::vector<std::vector<AccessNodeData> > backwardAccessNodes(graph.nodes());
    std::vector<std::vector<unsigned int> > forwardSearchSpaces(graph.nodes());
    std::vector<std::vector<unsigned int> > backwardSearchSpaces(graph.nodes());
    std::unordered_map<unsigned int, unsigned int> transitNodesMapping;
    for (unsigned int i = 0; i < transitNodesAmount; i++) {
        transitNodesMapping.insert(std::make_pair(transitNodes[i], i));
    }

    for (unsigned int i = 0; i < graph.nodes(); i++) {
        if (i % 100 == 0) {
            std::cout << "\rComputed access nodes for '" << i << "' nodes.";
        }

        findForwardAccessNodes(i, forwardAccessNodes[i], forwardSearchSpaces[i], transitNodesMapping, chGraph);
        findBackwardAccessNodes(i, backwardAccessNodes[i], backwardSearchSpaces[i], transitNodesMapping, chGraph);
    }

    std::cout << "\rComputed acess nodes for all the nodes in the graph." << std::endl;

    std::vector<std::pair<unsigned int, QueryEdge> > allEdges;
    chGraph.getEdgesForFlushing(allEdges);

    outputGraph(outputPath, graph, allEdges, transitNodes, transitNodesDistanceTable, forwardAccessNodes,
                backwardAccessNodes, forwardSearchSpaces, backwardSearchSpaces, transitNodesAmount);
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::preprocessUsingCHslower(UpdateableGraph& graph, Graph& originalGraph, std::string outputPath,
                                              unsigned int transitNodesAmount) {
    std::cout << "Getting transit nodes" << std::endl;
    std::vector<unsigned int> transitNodes(transitNodesAmount);
    graph.getNodesWithHighestRank(transitNodes, transitNodesAmount);

    std::cout << "Computing transit nodes distance table" << std::endl;
    FlagsGraph chGraph(graph);
    CHDistanceQueryManager qm(chGraph);
    std::vector<std::vector<unsigned int> > transitNodesDistanceTable(transitNodesAmount,
                                                                      std::vector<unsigned int>(transitNodesAmount));
    for (unsigned int i = 0; i < transitNodesAmount; i++) {
        if (i % 100 == 0) {
            std::cout << "\rComputed '" << i << "' transit nodes distance table rows.";
        }

        std::vector<unsigned int> distancesFromNodeI(originalGraph.nodes());
        BasicDijkstra::computeOneToAllDistances(transitNodes[i], originalGraph, distancesFromNodeI);
        for (unsigned int j = 0; j < transitNodesAmount; j++) {
            transitNodesDistanceTable[i][j] = distancesFromNodeI[transitNodes[j]];
        }
    }

    std::cout << "\rComputed the transit nodes distance table." << std::endl;

    std::cout << "Computing access nodes" << std::endl;
    std::vector<std::vector<AccessNodeData> > forwardAccessNodes(graph.nodes());
    std::vector<std::vector<AccessNodeData> > backwardAccessNodes(graph.nodes());
    std::vector<std::vector<unsigned int> > forwardSearchSpaces(graph.nodes());
    std::vector<std::vector<unsigned int> > backwardSearchSpaces(graph.nodes());
    std::unordered_map<unsigned int, unsigned int> transitNodesMapping;
    for (unsigned int i = 0; i < transitNodesAmount; i++) {
        transitNodesMapping.insert(std::make_pair(transitNodes[i], i));
    }

    for (unsigned int i = 0; i < graph.nodes(); i++) {
        if (i % 100 == 0) {
            std::cout << "\rComputed access nodes for '" << i << "' nodes.";
        }

        std::vector<unsigned int> forwardDistsFromNode(graph.nodes());
        BasicDijkstra::computeOneToAllDistances(i, originalGraph, forwardDistsFromNode);
        findForwardAccessNodes(i, forwardAccessNodes[i], forwardSearchSpaces[i], transitNodesMapping,
                               forwardDistsFromNode, chGraph);
        std::vector<unsigned int> backwardDistsFromNode(graph.nodes());
        BasicDijkstra::computeOneToAllDistancesInReversedGraph(i, originalGraph, backwardDistsFromNode);
        findBackwardAccessNodes(i, backwardAccessNodes[i], backwardSearchSpaces[i], transitNodesMapping,
                                backwardDistsFromNode, chGraph);
    }

    std::cout << "\rComputed acess nodes for all the nodes in the graph." << std::endl;

    std::vector<std::pair<unsigned int, QueryEdge> > allEdges;
    chGraph.getEdgesForFlushing(allEdges);

    outputGraph(outputPath, graph, allEdges, transitNodes, transitNodesDistanceTable, forwardAccessNodes,
                backwardAccessNodes, forwardSearchSpaces, backwardSearchSpaces, transitNodesAmount);
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::preprocessWithDMvalidation(UpdateableGraph &graph, Graph &originalGraph, std::string outputPath,
                                                 unsigned int transitNodesAmount) {
    std::cout << "Getting transit nodes" << std::endl;
    std::vector<unsigned int> transitNodes(transitNodesAmount);
    graph.getNodesWithHighestRank(transitNodes, transitNodesAmount);

    FlagsGraph chGraph(graph);

    Distance_matrix_travel_time_provider *distanceMatrix;
    {
        DistanceMatrixComputorSlow dmComputor;
        dmComputor.computeDistanceMatrix(originalGraph);
        distanceMatrix = dmComputor.getDistanceMatrixInstance();
    }

    std::cout << "Computing transit nodes distance table" << std::endl;
    std::vector<std::vector<unsigned int> > transitNodesDistanceTable(transitNodesAmount,
                                                                      std::vector<unsigned int>(transitNodesAmount));
    for (unsigned int i = 0; i < transitNodesAmount; i++) {
        for (unsigned int j = 0; j < transitNodesAmount; j++) {
            transitNodesDistanceTable[i][j] = distanceMatrix->findDistance(transitNodes[i], transitNodes[j]);
        }
    }

    std::cout << "Computing access nodes" << std::endl;
    std::vector<std::vector<AccessNodeData> > forwardAccessNodes(graph.nodes());
    std::vector<std::vector<AccessNodeData> > backwardAccessNodes(graph.nodes());
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
                               *distanceMatrix);
    }

    std::cout << "\rComputed forward acess nodes for all the nodes in the graph." << std::endl;

    {
        delete distanceMatrix;
        DistanceMatrixComputorSlow dmComputor;
        dmComputor.computeDistanceMatrixInReversedGraph(originalGraph);
        distanceMatrix = dmComputor.getDistanceMatrixInstance();
    }
    for (unsigned int i = 0; i < graph.nodes(); i++) {
        if (i % 100 == 0) {
            std::cout << "\rComputed backward access nodes for '" << i << "' nodes.";
        }

        findBackwardAccessNodes(i, backwardAccessNodes[i], backwardSearchSpaces[i], transitNodesMapping, chGraph,
                                *distanceMatrix);
    }

    std::cout << "\rComputed backward acess nodes for all the nodes in the graph." << std::endl;

    delete distanceMatrix;

    std::vector<std::pair<unsigned int, QueryEdge> > allEdges;
    chGraph.getEdgesForFlushing(allEdges);

    outputGraph(outputPath, graph, allEdges, transitNodes, transitNodesDistanceTable, forwardAccessNodes,
                backwardAccessNodes, forwardSearchSpaces, backwardSearchSpaces, transitNodesAmount);
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::outputGraph(std::string outputPath, UpdateableGraph &graph,
                                  std::vector<std::pair<unsigned int, QueryEdge> > &allEdges,
                                  std::vector<unsigned int> &transitNodes,
                                  std::vector<std::vector<unsigned int> > &transitNodesDistanceTable,
                                  std::vector<std::vector<AccessNodeData> > &forwardAccessNodes,
                                  std::vector<std::vector<AccessNodeData> > &backwardAccessNodes,
                                  std::vector<std::vector<unsigned int> > &forwardSearchSpaces,
                                  std::vector<std::vector<unsigned int> > &backwardSearchSpaces,
                                  unsigned int transitNodesAmount) {
    std::cout << "Outputting TNR" << std::endl;
    std::ofstream output;
    output.open(outputPath + ".tnrg", std::ios::binary);
    if (!output.is_open()) {
        printf("Couldn't open file '%s'!", (outputPath + ".tnrg").c_str());
    }

    char c1, c2, c3, c4;
    c1 = 'T';
    c2 = 'N';
    c3 = 'R';
    c4 = 'G';
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

    for (unsigned int i = 0; i < graph.nodes(); i++) {
        unsigned int rank = graph.getRank(i);
        output.write((char *) &rank, sizeof(rank));
    }

    for (unsigned int i = 0; i < transitNodesAmount; i++) {
        output.write((char *) &transitNodes[i], sizeof(transitNodes[i]));
    }

    for (unsigned int i = 0; i < transitNodesAmount; i++) {
        for (unsigned int j = 0; j < transitNodesAmount; j++) {
            output.write((char *) &transitNodesDistanceTable[i][j], sizeof(transitNodesDistanceTable[i][j]));
        }
    }

    for (unsigned int i = 0; i < graph.nodes(); i++) {
        unsigned int fwSize = boost::numeric_cast<unsigned int>(forwardAccessNodes[i].size());
        output.write((char *) &fwSize, sizeof(fwSize));
        for (unsigned int j = 0; j < fwSize; j++) {
            output.write((char *) &forwardAccessNodes[i][j].accessNodeID,
                         sizeof(forwardAccessNodes[i][j].accessNodeID));
            output.write((char *) &forwardAccessNodes[i][j].distanceToNode,
                         sizeof(forwardAccessNodes[i][j].distanceToNode));
        }
        unsigned int bwSize = boost::numeric_cast<unsigned int>(backwardAccessNodes[i].size());
        output.write((char *) &bwSize, sizeof(bwSize));
        for (unsigned int j = 0; j < bwSize; j++) {
            output.write((char *) &backwardAccessNodes[i][j].accessNodeID,
                         sizeof(backwardAccessNodes[i][j].accessNodeID));
            output.write((char *) &backwardAccessNodes[i][j].distanceToNode,
                         sizeof(backwardAccessNodes[i][j].distanceToNode));
        }
    }

    size_t fwSearchSpaceSum = 0;
    size_t bwSearchSpaceSum = 0;
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

    output.close();
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findForwardAccessNodes(unsigned int source, std::vector<AccessNodeData> &accessNodes,
                                             std::vector<unsigned int> &forwardSearchSpace,
                                             std::unordered_map<unsigned int, unsigned int> &transitNodes,
                                             FlagsGraph<NodeData>& graph) {
    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight); };
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> forwardQ(cmp);
    std::vector<unsigned int> distances(graph.nodes(), UINT_MAX);
    std::vector<bool> settled(graph.nodes(), false);

    forwardQ.push(DijkstraNode(source, 0));

    distances[source] = 0;

    while (!forwardQ.empty()) {
        unsigned int curNode = forwardQ.top().ID;
        unsigned int curLen = forwardQ.top().weight;
        forwardQ.pop();

        if (settled[curNode]) {
            continue;
        }

        settled[curNode] = true;
        if (transitNodes.count(curNode) == 1) {
            accessNodes.push_back(AccessNodeData(curNode, curLen));
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
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findForwardAccessNodes(unsigned int source, std::vector<AccessNodeData> &accessNodes,
                                             std::vector<unsigned int> &forwardSearchSpace,
                                             std::unordered_map<unsigned int, unsigned int> &transitNodes,
                                             std::vector<unsigned int> &distsFromNode, FlagsGraph<NodeData>& graph) {
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

    for (unsigned int i = 0; i < accessNodesSuperset.size(); i++) {
        unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
        unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
        unsigned int realDistance = distsFromNode[accessNode];
        if (realDistance == accessNodeDistance) {
            accessNodes.push_back(
                    AccessNodeData(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode));
        }
    }
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findForwardAccessNodes(unsigned int source, std::vector<AccessNodeData> &accessNodes,
                                             std::vector<unsigned int> &forwardSearchSpace,
                                             std::unordered_map<unsigned int, unsigned int> &transitNodes,
                                             FlagsGraph<NodeData>& graph, Distance_matrix_travel_time_provider &dm) {
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

    for (size_t i = 0; i < accessNodesSuperset.size(); i++) {
        unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
        unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
        unsigned int realDistance = dm.findDistance(source, accessNode);
        if (realDistance == accessNodeDistance) {
            accessNodes.push_back(
                    AccessNodeData(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode));
        }
    }

}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findBackwardAccessNodes(unsigned int source, std::vector<AccessNodeData> &accessNodes,
                                              std::vector<unsigned int> &backwardSearchSpace,
                                              std::unordered_map<unsigned int, unsigned int> &transitNodes,
                                              FlagsGraph<NodeData>& graph) {
    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight); };
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> backwardQ(cmp);
    std::vector<unsigned int> distances(graph.nodes(), UINT_MAX);
    std::vector<bool> settled(graph.nodes(), false);

    backwardQ.push(DijkstraNode(source, 0));

    distances[source] = 0;

    while (!backwardQ.empty()) {
        unsigned int curNode = backwardQ.top().ID;
        unsigned int curLen = backwardQ.top().weight;
        backwardQ.pop();

        if (settled[curNode]) {
            continue;
        }

        settled[curNode] = true;
        if (transitNodes.count(curNode) == 1) {
            accessNodes.push_back(AccessNodeData(curNode, curLen));
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
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findBackwardAccessNodes(unsigned int source, std::vector<AccessNodeData> &accessNodes,
                                              std::vector<unsigned int> &backwardSearchSpace,
                                              std::unordered_map<unsigned int, unsigned int> &transitNodes,
                                              std::vector<unsigned int> &distsFromNode, FlagsGraph<NodeData>& graph) {
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


    for (size_t i = 0; i < accessNodesSuperset.size(); i++) {
        unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
        unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
        unsigned int realDistance = distsFromNode[accessNode];
        if (realDistance == accessNodeDistance) {
            accessNodes.push_back(
                    AccessNodeData(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode));
        }
    }
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findBackwardAccessNodes(unsigned int source, std::vector<AccessNodeData> &accessNodes,
                                              std::vector<unsigned int> &backwardSearchSpace,
                                              std::unordered_map<unsigned int, unsigned int> &transitNodes,
                                              FlagsGraph<NodeData>& graph, Distance_matrix_travel_time_provider &dm) {
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


    for (size_t i = 0; i < accessNodesSuperset.size(); i++) {
        unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
        unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
        unsigned int realDistance = dm.findDistance(source, accessNode);
        if (realDistance == accessNodeDistance) {
            accessNodes.push_back(
                    AccessNodeData(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode));
        }
    }

}
