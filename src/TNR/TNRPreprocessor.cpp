//
// Author: Xenty (Michal Cvach)
// Created on: 05.08.19
//

#include <climits>
#include "TNRPreprocessor.h"
#include "../CH/Integer/IntegerCHPreprocessor.h"
#include "../GraphBuilding/Structures/IntegerStructures/IntegerFlagsGraph.h"
#include "../CH/Integer/IntegerCHDistanceQueryManager.h"
#include "Structures/AccessNodeData.h"
#include "../Dijkstra/IntegerDijkstra/IntegerDijkstraNode.h"

// Builds Transit Node Routing structures based on Contraction Hierarchies. User can set the amount of Transit Nodes.
// Generally, more transit nodes mean more memory consumption as more distances must be present in the distance table,
// but could potentially speed up the queries as more queries will hit those new transit nodes.
//______________________________________________________________________________________________________________________
void TNRPreprocessor::preprocessUsingCH(IntegerUpdateableGraph & graph, unsigned int transitNodesAmount) {
    IntegerCHPreprocessor::preprocessForDDSG(graph);

    vector < unsigned int > transitNodes(transitNodesAmount);
    graph.getNodesWithHighestRank(transitNodes, transitNodesAmount);

    IntegerFlagsGraph chGraph(graph);
    IntegerCHDistanceQueryManager qm(chGraph);
    vector < vector < unsigned int > > transitNodesDistanceTable(transitNodesAmount, vector < unsigned int > (transitNodesAmount));
    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        for(unsigned int j = 0; j < transitNodesAmount; j++) {
            if (i == j) {
                transitNodesDistanceTable[i][j] = 0;
            } else {
                transitNodesDistanceTable[i][j] = qm.findDistance(transitNodes[i], transitNodes[j]);
            }
        }
    }

    vector < vector < AccessNodeData > > forwardAccessNodes(graph.nodes());
    vector < vector < AccessNodeData > > backwardAccessNodes(graph.nodes());
    unordered_map < unsigned int, unsigned int > transitNodesMapping;
    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        transitNodesMapping.insert(transitNodes[i], i);
    }

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        findForwardAccessNodes(i, forwardAccessNodes[i], transitNodesMapping, transitNodesDistanceTable, chGraph);
        findBackwardAccessNodes(i, backwardAccessNodes[i], transitNodesMapping, transitNodesDistanceTable, chGraph);
    }

}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findForwardAccessNodes(unsigned int source, vector < AccessNodeData > & accessNodes, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph) {
    auto cmp = [](IntegerDijkstraNode left, IntegerDijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<IntegerDijkstraNode, vector<IntegerDijkstraNode>, decltype(cmp)> forwardQ(cmp);
    vector<unsigned int> distances(graph.nodes(), UINT_MAX);
    vector<bool> reached(graph.nodes(), false);

    forwardQ.push(IntegerDijkstraNode(source, 0));

    distances[source] = 0;
    reached[source] = true;

    vector < AccessNodeData > accessNodesSuperset;

    while (! forwardQ.empty() ) {
        unsigned int curNode = forwardQ.top().ID;
        long long unsigned int curLen = forwardQ.top().weight;
        forwardQ.pop();

        // Classic edges relaxation
        const vector<IntegerQueryEdge> & neighbours = graph.nextNodes(curNode);
        for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
            // Skip edge if it is only in the other direction.
            if (! (*iter).forward) {
                continue;
            }

            // This is basically the dijkstra edge relaxation process. Additionaly, we unstall the node
            // if it was stalled previously, because it might be now reached on the optimal path.
            if (graph.data((*iter).targetNode).rank > graph.data(curNode).rank) {
                long long unsigned int newlen = curLen + (*iter).weight;
                if (! reached[(*iter).targetNode] && newlen < distances[(*iter).targetNode]) {
                    distances[(*iter).targetNode] = newlen;
                    reached[(*iter).targetNode] = true;

                    if (transitNodes.count((*iter).targetNode) == 1) {
                        accessNodesSuperset.push_back(AccessNodeData((*iter).targetNode, distances[(*iter).targetNode]));
                    } else {
                        forwardQ.push(IntegerDijkstraNode((*iter).targetNode, distances[(*iter).targetNode]));
                    }
                }
            }
        }

    }

    for(unsigned int i = 0; i < accessNodesSuperset.size(); i++) {
        bool validAccessNode = true;
        for(unsigned int j = 0; j < accessNodesSuperset.size(); j++) {
            if (j != i) {
                unsigned int t1 = transitNodes[accessNodesSuperset[j].acessNodeID];
                unsigned int t2 = transitNodes[accessNodesSuperset[i].acessNodeID];
                if (accessNodesSuperset[j].distanceToNode + transitNodesDistanceTable[t1][t2] <= accessNodesSuperset[i].distanceToNode) {
                    validAccessNode = false;
                    break;
                }
            }
        }

        if (validAccessNode) {
            accessNodes.push_back(AccessNodeData(accessNodesSuperset[i].acessNodeID, accessNodesSuperset[i].distanceToNode));
        }

    }

}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findBackwardAccessNodes(unsigned int source, vector < AccessNodeData > & accessNodes, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph) {
    auto cmp = [](IntegerDijkstraNode left, IntegerDijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<IntegerDijkstraNode, vector<IntegerDijkstraNode>, decltype(cmp)> backwardQ(cmp);
    vector<unsigned int> distances(graph.nodes(), UINT_MAX);
    vector<bool> reached(graph.nodes(), false);

    backwardQ.push(IntegerDijkstraNode(source, 0));

    distances[source] = 0;
    reached[source] = true;

    vector < AccessNodeData > accessNodesSuperset;

    while (! backwardQ.empty() ) {
        unsigned int curNode = backwardQ.top().ID;
        long long unsigned int curLen = backwardQ.top().weight;
        backwardQ.pop();

        // Classic edges relaxation
        const vector<IntegerQueryEdge> & neighbours = graph.nextNodes(curNode);
        for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
            // Skip edge if it is only in the other direction.
            if (! (*iter).backward) {
                continue;
            }

            // This is basically the dijkstra edge relaxation process. Additionaly, we unstall the node
            // if it was stalled previously, because it might be now reached on the optimal path.
            if (graph.data((*iter).targetNode).rank > graph.data(curNode).rank) {
                long long unsigned int newlen = curLen + (*iter).weight;
                if (! reached[(*iter).targetNode] && newlen < distances[(*iter).targetNode]) {
                    distances[(*iter).targetNode] = newlen;
                    reached[(*iter).targetNode] = true;

                    if (transitNodes.count((*iter).targetNode) == 1) {
                        accessNodesSuperset.push_back(AccessNodeData((*iter).targetNode, distances[(*iter).targetNode]));
                    } else {
                        backwardQ.push(IntegerDijkstraNode((*iter).targetNode, distances[(*iter).targetNode]));
                    }
                }
            }
        }

    }

    for(unsigned int i = 0; i < accessNodesSuperset.size(); i++) {
        bool validAccessNode = true;
        for(unsigned int j = 0; j < accessNodesSuperset.size(); j++) {
            if (j != i) {
                unsigned int t1 = transitNodes[accessNodesSuperset[j].acessNodeID];
                unsigned int t2 = transitNodes[accessNodesSuperset[i].acessNodeID];
                if (accessNodesSuperset[j].distanceToNode + transitNodesDistanceTable[t1][t2] <= accessNodesSuperset[i].distanceToNode) {
                    validAccessNode = false;
                    break;
                }
            }
        }

        if (validAccessNode) {
            accessNodes.push_back(AccessNodeData(accessNodesSuperset[i].acessNodeID, accessNodesSuperset[i].distanceToNode));
        }

    }

}