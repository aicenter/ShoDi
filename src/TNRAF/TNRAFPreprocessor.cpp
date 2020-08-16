//
// Author: Xenty (Michal Cvach)
// Created on: 30.11.19
//

#include <iostream>
#include <climits>
#include <fstream>
#include "DistanceMatrix/DistanceMatrixComputorSlow.h"
#include "TNRAFPreprocessor.h"
#include "../CH/CHDistanceQueryManager.h"
#include "Structures/AccessNodeDataArcFlags.h"
#include "../Dijkstra/DijkstraNode.h"
#include "../Dijkstra/BasicDijkstra.h"

DistanceMatrix * TNRAFPreprocessor::distanceMatrix = NULL;

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::preprocessUsingCH(UpdateableGraph & graph, Graph & originalGraph, string outputPath, unsigned int transitNodesAmount, unsigned int regionsCnt, bool useDistanceMatrix) {
    cout << "Getting transit nodes" << endl;
    vector<unsigned int> transitNodes(transitNodesAmount);
    graph.getNodesWithHighestRank(transitNodes, transitNodesAmount);

    cout << "Computing transit nodes distance table" << endl;
    FlagsGraph chGraph(graph);
    CHDistanceQueryManager qm(chGraph);
    vector<vector<unsigned int> > transitNodesDistanceTable(transitNodesAmount,
                                                            vector<unsigned int>(transitNodesAmount));
    if (useDistanceMatrix) {
        cout << "Computing the auxiliary distance matrix for transit node set distance matrix and access nodes forward direction." << endl;
        DistanceMatrixComputorSlow dmComputor;
        dmComputor.computeDistanceMatrix(originalGraph);
        distanceMatrix = dmComputor.getDistanceMatrixInstance();
        cout << "Distance matrix computed." << endl;

        fillTransitNodeDistanceTable(transitNodes, transitNodesDistanceTable, transitNodesAmount);
    } else {
        computeTransitNodeDistanceTable(transitNodes, transitNodesDistanceTable, transitNodesAmount, originalGraph);
    }

    RegionsStructure regions(graph.nodes(), regionsCnt);
    generateClustering(originalGraph, regions, regionsCnt);

    vector < vector < AccessNodeDataArcFlags > > forwardAccessNodes(graph.nodes());
    vector < vector < AccessNodeDataArcFlags > > backwardAccessNodes(graph.nodes());
    vector < vector < unsigned int > > forwardSearchSpaces(graph.nodes());
    vector < vector < unsigned int > > backwardSearchSpaces(graph.nodes());
    unordered_map < unsigned int, unsigned int > transitNodesMapping;
    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        transitNodesMapping.insert(make_pair(transitNodes[i], i));
    }

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        if(i % 100 == 0) {
            cout << "\rComputed forward access nodes for '" << i << "' nodes.";
        }

        findForwardAccessNodes(i, forwardAccessNodes[i], forwardSearchSpaces[i], transitNodesMapping, chGraph, originalGraph, regions, useDistanceMatrix);
    }

    cout << "\rComputed forward access nodes for all nodes in the graph." << endl;

    if(useDistanceMatrix) {
        cout << "Computing the auxiliary distance matrix for backward direction." << endl;
        delete distanceMatrix;
        DistanceMatrixComputorSlow dmComputor;
        dmComputor.computeDistanceMatrixInReversedGraph(originalGraph);
        distanceMatrix = dmComputor.getDistanceMatrixInstance();
        cout << "Distance matrix computed." << endl;
    }

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        if(i % 100 == 0) {
            cout << "\rComputed backward access nodes for '" << i << "' nodes.";
        }

        findBackwardAccessNodes(i, backwardAccessNodes[i], backwardSearchSpaces[i], transitNodesMapping, chGraph, originalGraph, regions, useDistanceMatrix);
    }

    cout << "\rComputed backward access nodes for all nodes in the graph." << endl;

    if(useDistanceMatrix) {
        delete distanceMatrix;
        distanceMatrix = NULL;
    }

    vector < pair < unsigned int, QueryEdge > > allEdges;
    chGraph.getEdgesForFlushing(allEdges);

    outputGraph(outputPath, graph, allEdges, transitNodes, transitNodesDistanceTable, forwardAccessNodes, backwardAccessNodes, forwardSearchSpaces, backwardSearchSpaces, transitNodesAmount, regions, regionsCnt);
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::computeTransitNodeDistanceTable(vector<unsigned int> & transitNodes, vector<vector<unsigned int>> & distanceTable, unsigned int transitNodesCnt, Graph & originalGraph) {
    for(unsigned int i = 0; i < transitNodesCnt; i++) {
        if(i % 100 == 0) {
            cout << "\rComputed '" << i << "' transit nodes distance table rows.";
        }

        vector<unsigned int> distancesFromNodeI(originalGraph.nodes());
        BasicDijkstra::computeOneToAllDistances(transitNodes[i], originalGraph, distancesFromNodeI);
        for(unsigned int j = 0; j < transitNodesCnt; j++) {
            distanceTable[i][j] = distancesFromNodeI[transitNodes[j]];
        }
    }

    cout << "\rComputed the transit nodes distance table." << endl;
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::fillTransitNodeDistanceTable(vector<unsigned int> & transitNodes, vector<vector<unsigned int>> & distanceTable, unsigned int transitNodesCnt) {
    for(unsigned int i = 0; i < transitNodesCnt; i++) {
        for(unsigned int j = 0; j < transitNodesCnt; j++) {
            distanceTable[i][j] = distanceMatrix->findDistance(transitNodes[i], transitNodes[j]);
        }
    }
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::outputGraph(string outputPath, UpdateableGraph & graph, vector < pair < unsigned int, QueryEdge > > & allEdges, vector < unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, vector < vector < AccessNodeDataArcFlags > > & forwardAccessNodes, vector < vector < AccessNodeDataArcFlags > > & backwardAccessNodes, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces, unsigned int transitNodesAmount, RegionsStructure & regions, unsigned int regionsCnt) {
    cout << "Outputting TNRAF" << endl;
    ofstream output;
    output.open ( outputPath + ".tgaf", ios::binary );
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (outputPath + ".tgaf").c_str());
    }

    // Output the header consisting of a predefined string, and then the numbers of nodes, edges, the size of the
    // transit node set and the number of regions.
    char c1, c2, c3, c4;
    c1 = 'T';
    c2 = 'G';
    c3 = 'A';
    c4 = 'F';
    output.write(&c1, sizeof (c1));
    output.write(&c2, sizeof (c2));
    output.write(&c3, sizeof (c3));
    output.write(&c4, sizeof (c4));
    unsigned int nodes = graph.nodes();
    unsigned int edges = (unsigned int) allEdges.size();
    output.write((char *) &nodes, sizeof(nodes));
    output.write((char *) &edges, sizeof(edges));
    output.write((char *) &transitNodesAmount, sizeof(transitNodesAmount));
    output.write((char *) &regionsCnt, sizeof(regionsCnt));

    // Output all edges, this means the original edges as well as the shorctut edges.
    for(unsigned int i = 0; i < allEdges.size(); i++) {
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
    for(unsigned int i = 0; i < graph.nodes(); i++) {
        unsigned int rank = graph.getRank(i);
        output.write((char *) &rank, sizeof(rank));
    }

    // Output regions for all nodes in the graph (used for Arc Flags).
    for(unsigned int i = 0; i < graph.nodes(); i++) {
        unsigned int region = regions.getRegion(i);
        output.write((char *) &region, sizeof(region));
    }

    // Output the transit nodes ids and then the full transit node set distance matrix (distances between all pairs of
    // transit nodes).
    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        output.write((char *) &transitNodes[i], sizeof(transitNodes[i]));
    }
    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        for(unsigned int j = 0; j < transitNodesAmount; j++) {
            output.write((char *) &transitNodesDistanceTable[i][j], sizeof(transitNodesDistanceTable[i][j]));
        }
    }

    // Output the access nodes (their arc flags are also output here).
    vector<uint32_t> powersOf2(regionsCnt);
    initPowersOf2(powersOf2);
    for(unsigned int i = 0; i < graph.nodes(); i++) {
        unsigned int fwSize = (unsigned int) forwardAccessNodes[i].size();
        output.write((char *) &fwSize, sizeof(fwSize));
        for(unsigned int j = 0; j < forwardAccessNodes[i].size(); j++) {
            output.write((char *) &forwardAccessNodes[i][j].accessNodeID, sizeof(forwardAccessNodes[i][j].accessNodeID));
            output.write((char *) &forwardAccessNodes[i][j].distanceToNode, sizeof(forwardAccessNodes[i][j].distanceToNode));
            uint32_t regionsOutput = 0;
            for(unsigned int k = 0; k < regionsCnt; k++) {
                if(forwardAccessNodes[i][j].regionFlags[k]) {
                    regionsOutput = regionsOutput | powersOf2[k];
                }
            }
            output.write((char *) &regionsOutput, sizeof(regionsOutput));
        }
        unsigned int bwSize = (unsigned int) backwardAccessNodes[i].size();
        output.write((char *) &bwSize, sizeof(bwSize));
        for(unsigned int j = 0; j < backwardAccessNodes[i].size(); j++) {
            output.write((char *) &backwardAccessNodes[i][j].accessNodeID, sizeof(backwardAccessNodes[i][j].accessNodeID));
            output.write((char *) &backwardAccessNodes[i][j].distanceToNode, sizeof(backwardAccessNodes[i][j].distanceToNode));
            uint32_t regionsOutput = 0;
            for(unsigned int k = 0; k < regionsCnt; k++) {
                if(backwardAccessNodes[i][j].regionFlags[k]) {
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
    for(unsigned int i = 0; i < graph.nodes(); i++) {
        unsigned int fwSearchSpaceSize = (unsigned int) forwardSearchSpaces[i].size();
        output.write((char *) &fwSearchSpaceSize, sizeof(fwSearchSpaceSize));
        for(unsigned int j = 0; j < fwSearchSpaceSize; j++) {
            output.write((char *) &forwardSearchSpaces[i][j], sizeof(forwardSearchSpaces[i][j]));
        }
        fwSearchSpaceSum += fwSearchSpaceSize;

        unsigned int bwSearchSpaceSize = (unsigned int) backwardSearchSpaces[i].size();
        output.write((char *) &bwSearchSpaceSize, sizeof(bwSearchSpaceSize));
        for(unsigned int j = 0; j < bwSearchSpaceSize; j++) {
            output.write((char *) &backwardSearchSpaces[i][j], sizeof(backwardSearchSpaces[i][j]));
        }
        bwSearchSpaceSum += bwSearchSpaceSize;

    }

    printf("Average forward search space size: %lf\n", (double)fwSearchSpaceSum/nodes);
    printf("Average backward search space size: %lf\n", (double)bwSearchSpaceSum/nodes);

    output.close();
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::findForwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & forwardSearchSpace, unordered_map< unsigned int, unsigned int > & transitNodes, FlagsGraph & graph, Graph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix) {
    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> forwardQ(cmp);
    vector<unsigned int> distances(graph.nodes(), UINT_MAX);
    vector<bool> settled(graph.nodes(), false);

    forwardQ.push(DijkstraNode(source, 0));

    distances[source] = 0;

    vector < AccessNodeData > accessNodesSuperset;

    while (! forwardQ.empty() ) {
        unsigned int curNode = forwardQ.top().ID;
        unsigned int curLen = forwardQ.top().weight;
        forwardQ.pop();

        if(settled[curNode]) {
            continue;
        }

        settled[curNode] = true;
        if(transitNodes.count(curNode) == 1) {
            accessNodesSuperset.push_back(AccessNodeData(curNode, curLen));
        } else {
            forwardSearchSpace.push_back(curNode);

            const vector<QueryEdge> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                // Skip edge if it is only in the other direction.
                if (! (*iter).forward) {
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

    vector<unsigned int> distancesFromNode;
    if(! useDistanceMatrix) {
        distancesFromNode.resize(originalGraph.nodes());
        BasicDijkstra::computeOneToAllDistances(source, originalGraph, distancesFromNode);
    }
    for(unsigned int i = 0; i < accessNodesSuperset.size(); i++) {
        if(useDistanceMatrix) {
            unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
            unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
            unsigned int realDistance = distanceMatrix->findDistance(source, accessNode);
            if (realDistance == accessNodeDistance) {
                accessNodes.push_back(AccessNodeDataArcFlags(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode, regions.getRegionsCnt()));
            }
        } else {
            unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
            unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
            unsigned int realDistance = distancesFromNode[accessNode];
            if (realDistance == accessNodeDistance) {
                accessNodes.push_back(AccessNodeDataArcFlags(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode, regions.getRegionsCnt()));
            }
        }
    }

    computeForwardArcFlags(source, accessNodes, originalGraph, regions, useDistanceMatrix, distancesFromNode);

}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::computeForwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, Graph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix, vector<unsigned int> & optionalDistancesFromNode) {
    if(useDistanceMatrix) {
        for (unsigned int i = 0; i < accessNodes.size(); i++) {
            unsigned int accessNode = accessNodes[i].accessNodeID;
            unsigned int distanceToAccessNode = accessNodes[i].distanceToNode;
            for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
                vector<unsigned int> & nodesInRegion = regions.nodesInRegion(j);
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
        for (unsigned int i = 0; i < accessNodes.size(); i++) {
            vector<unsigned int> distancesFromAccessNode(originalGraph.nodes());
            unsigned int accessNode = accessNodes[i].accessNodeID;
            BasicDijkstra::computeOneToAllDistances(accessNode, originalGraph, distancesFromAccessNode);
            unsigned int distanceToAccessNode = optionalDistancesFromNode[accessNode];
            for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
                vector<unsigned int> &nodesInRegion = regions.nodesInRegion(j);
                for (unsigned int k = 0; k < nodesInRegion.size(); k++) {
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
void TNRAFPreprocessor::findBackwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & backwardSearchSpace, unordered_map< unsigned int, unsigned int > & transitNodes, FlagsGraph & graph, Graph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix) {
    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> backwardQ(cmp);
    vector<unsigned int> distances(graph.nodes(), UINT_MAX);
    vector<bool> settled(graph.nodes(), false);

    backwardQ.push(DijkstraNode(source, 0));

    distances[source] = 0;

    vector < AccessNodeData > accessNodesSuperset;

    while (! backwardQ.empty() ) {
        unsigned int curNode = backwardQ.top().ID;
        unsigned int curLen = backwardQ.top().weight;
        backwardQ.pop();

        if(settled[curNode]) {
            continue;
        }

        settled[curNode] = true;
        if(transitNodes.count(curNode) == 1) {
            accessNodesSuperset.push_back(AccessNodeData(curNode, curLen));
        } else {
            backwardSearchSpace.push_back(curNode);

            const vector<QueryEdge> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                // Skip edge if it is only in the other direction.
                if (! (*iter).backward) {
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

    vector<unsigned int> distancesFromNode;
    if(! useDistanceMatrix) {
        distancesFromNode.resize(originalGraph.nodes());
        BasicDijkstra::computeOneToAllDistancesInReversedGraph(source, originalGraph, distancesFromNode);
    }
    for(unsigned int i = 0; i < accessNodesSuperset.size(); i++) {
        if(useDistanceMatrix) {
            unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
            unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
            unsigned int realDistance = distanceMatrix->findDistance(source, accessNode);
            if (realDistance == accessNodeDistance) {
                accessNodes.push_back(AccessNodeDataArcFlags(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode, regions.getRegionsCnt()));
            }
        } else {
            unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
            unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
            unsigned int realDistance = distancesFromNode[accessNode];
            if (realDistance == accessNodeDistance) {
                accessNodes.push_back(AccessNodeDataArcFlags(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode, regions.getRegionsCnt()));
            }
        }
    }

    computeBackwardArcFlags(source, accessNodes, originalGraph, regions, useDistanceMatrix, distancesFromNode);

}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::computeBackwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, Graph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix, vector<unsigned int> & optionalDistancesFromNode) {
    if (useDistanceMatrix) {
        for (unsigned int i = 0; i < accessNodes.size(); i++) {
            unsigned int accessNode = accessNodes[i].accessNodeID;
            unsigned int distanceToAccessNode = accessNodes[i].distanceToNode;
            for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
                vector<unsigned int> &nodesInRegion = regions.nodesInRegion(j);
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
        for (unsigned int i = 0; i < accessNodes.size(); i++) {
            vector<unsigned int> distancesFromAccessNode(originalGraph.nodes());
            unsigned int accessNode = accessNodes[i].accessNodeID;
            BasicDijkstra::computeOneToAllDistancesInReversedGraph(accessNode, originalGraph,
                                                                   distancesFromAccessNode);
            unsigned int distanceToAccessNode = optionalDistancesFromNode[accessNode];
            for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
                vector<unsigned int> &nodesInRegion = regions.nodesInRegion(j);
                for (unsigned int k = 0; k < nodesInRegion.size(); k++) {
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
void TNRAFPreprocessor::generateClustering(Graph & originalGraph, RegionsStructure & regions, unsigned int clustersCnt) {
    unsigned int nodesCnt = originalGraph.nodes();
    unsigned int approxNodesPerCluster = nodesCnt / clustersCnt;
    unsigned int unassignedNodes = nodesCnt - clustersCnt;
    vector < unsigned int > assignedClusters(nodesCnt, UINT_MAX);
    vector < queue < unsigned int > > q(clustersCnt, queue<unsigned int>());

    for(unsigned int i = 0; i < clustersCnt; ++i) {
        const unsigned int nodeID = approxNodesPerCluster * i;
        assignedClusters[nodeID] = i;
        const vector<pair<unsigned int, unsigned int>> & neighboursFW = originalGraph.outgoingEdges(nodeID);
        for(unsigned int j = 0; j < neighboursFW.size(); ++j) {
            q[i].push(neighboursFW[j].first);
        }
        const vector<pair<unsigned int, unsigned int>> & neighboursBW = originalGraph.incomingEdges(nodeID);
        for(unsigned int j = 0; j < neighboursBW.size(); ++j) {
            q[i].push(neighboursBW[j].first);
        }
    }

    bool jumpout = false;
    while(! jumpout) {
        for(unsigned int i = 0; i < clustersCnt; ++i) {
            const unsigned int newNodeForCluster = getNewNodeForCluster(assignedClusters, q[i]);

            assignedClusters[newNodeForCluster] = i;
            const vector<pair<unsigned int, unsigned int>> & neighboursFW = originalGraph.outgoingEdges(newNodeForCluster);
            for(unsigned int j = 0; j < neighboursFW.size(); ++j) {
                q[i].push(neighboursFW[j].first);
            }
            const vector<pair<unsigned int, unsigned int>> & neighboursBW = originalGraph.incomingEdges(newNodeForCluster);
            for(unsigned int j = 0; j < neighboursBW.size(); ++j) {
                q[i].push(neighboursBW[j].first);
            }

            unassignedNodes--;
            if(unassignedNodes == 0) {
                jumpout = true;
                break;
            }

            if(unassignedNodes % 2000 == 0) {
                printf("\rThere are %u nodes left to be assigned to clusters.", unassignedNodes);
            }
        }
    }

    printf("\rClustering for the Arc Flags computed.\n");

    for(unsigned int i = 0; i < nodesCnt; ++i) {
        regions.addNode(i, assignedClusters[i]);
    }
}

//______________________________________________________________________________________________________________________
unsigned int TNRAFPreprocessor::getNewNodeForCluster( vector < unsigned int > & assignedClusters, queue < unsigned int > & q) {
    while(! q.empty()) {
        unsigned int nodeID = q.front();
        q.pop();
        if(assignedClusters[nodeID] == UINT_MAX) {
            return nodeID;
        }
    }

    for(unsigned int i = 0; i < assignedClusters.size(); ++i) {
        if(assignedClusters[i] == UINT_MAX) {
            return i;
        }
    }

    return UINT_MAX;
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::getPowersOf2(vector<uint32_t> & powersOf2) {
    initPowersOf2(powersOf2);
}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::initPowersOf2(vector<uint32_t> & powersOf2) {
    uint32_t val = 1;
    for(unsigned int i = 0; i < 32; i++) {
        powersOf2[i] = val;
        val = val * 2;
    }
}

