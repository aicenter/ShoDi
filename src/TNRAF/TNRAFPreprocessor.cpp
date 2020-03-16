//
// Author: Xenty (Michal Cvach)
// Created on: 30.11.19
//

#include <iostream>
#include <climits>
#include <fstream>
#include "TNRAFPreprocessor.h"
#include "../CH/Integer/IntegerCHDistanceQueryManager.h"
#include "Structures/AccessNodeDataArcFlags.h"
#include "../Dijkstra/IntegerDijkstra/IntegerDijkstraNode.h"
#include "../Dijkstra/IntegerDijkstra/BasicIntegerDijkstra.h"
#include "../DistanceMatrix/IntegerDistanceMatrixComputor.h"
#include "../DistanceMatrix/IntegerDistanceMatrix.h"

// FIXME debug stuff
unsigned int TNRAFPreprocessor::totalArcFlags;
unsigned int TNRAFPreprocessor::trueArcFlags;
unsigned int TNRAFPreprocessor::totalAccessNodes;
unsigned int TNRAFPreprocessor::uselessAccessNodes;
long long unsigned int TNRAFPreprocessor::triedCombinations;
unsigned int TNRAFPreprocessor::incorrectANdistances;
unsigned int TNRAFPreprocessor::ANdistances;
unsigned int TNRAFPreprocessor::clusteringSkips;
IntegerDistanceMatrix * TNRAFPreprocessor::distanceMatrix = NULL;

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::preprocessUsingCH(IntegerUpdateableGraph & graph, IntegerGraph & originalGraph, string outputPath, unsigned int transitNodesAmount, unsigned int regionsCnt, bool useDistanceMatrix) {
    cout << "Getting transit nodes" << endl;
    vector < unsigned int > transitNodes(transitNodesAmount);
    graph.getNodesWithHighestRank(transitNodes, transitNodesAmount);

    cout << "Computing transit nodes distance table" << endl;
    IntegerFlagsGraph chGraph(graph);
    IntegerCHDistanceQueryManager qm(chGraph);
    vector < vector < unsigned int > > transitNodesDistanceTable(transitNodesAmount, vector < unsigned int > (transitNodesAmount));
    computeTransitNodeDistanceTable(transitNodes, transitNodesDistanceTable, transitNodesAmount, originalGraph);
    /*for(unsigned int i = 0; i < transitNodesAmount; i++) {
        if(i % 100 == 0) {
            cout << "Computed transit nodes distances for '" << i << "' transit nodes." << endl;
        }
        for(unsigned int j = 0; j < transitNodesAmount; j++) {
            if (i == j) {
                transitNodesDistanceTable[i][j] = 0;
            } else {
                transitNodesDistanceTable[i][j] = qm.findDistance(transitNodes[i], transitNodes[j]);
            }
        }
    }*/

    cout << "Generating regions for the nodes" << endl;
    //vector<unsigned int> clusters(originalGraph.nodes());
    RegionsStructure regions(graph.nodes(), regionsCnt);
    generateClustering(originalGraph, regions, regionsCnt);

    cout << "Computing access nodes" << endl;
    vector < vector < AccessNodeDataArcFlags > > forwardAccessNodes(graph.nodes());
    vector < vector < AccessNodeDataArcFlags > > backwardAccessNodes(graph.nodes());
    vector < vector < unsigned int > > forwardSearchSpaces(graph.nodes());
    vector < vector < unsigned int > > backwardSearchSpaces(graph.nodes());
    unordered_map < unsigned int, unsigned int > transitNodesMapping;
    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        transitNodesMapping.insert(make_pair(transitNodes[i], i));
    }

    // FIXME debug stuff
    totalArcFlags = 0;
    trueArcFlags = 0;
    totalAccessNodes = 0;
    uselessAccessNodes = 0;
    triedCombinations = 0;

    if(useDistanceMatrix) {
        cout << "Computing the auxiliary distance matrix for forward direction." << endl;
        IntegerDistanceMatrixComputor dmComputor;
        dmComputor.computeDistanceMatrix(originalGraph);
        distanceMatrix = dmComputor.getDistanceMatrixInstance();
        cout << "Distance matrix computed." << endl;
    }

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        if(i % 200 == 0) {
            cout << "Computed forward access nodes for '" << i << "' transit nodes." << endl;
        }

        findForwardAccessNodes(i, forwardAccessNodes[i], forwardSearchSpaces[i], transitNodesMapping, transitNodesDistanceTable, chGraph, originalGraph, regions, useDistanceMatrix);
    }

    //printf("Useless access nodes: %u, total access nodes: %u, that means %lf %% were useless.\n", uselessAccessNodes, totalAccessNodes, ((double) uselessAccessNodes / totalAccessNodes) * 100);
    printf("Tried %llu combinations when determining access nodes.\n", triedCombinations);
    printf("Additionally, %u out of %u access nodes have incorrect distances. That is %lf %%.\n", incorrectANdistances, ANdistances, ((double) incorrectANdistances / ANdistances) * 100);

    if(useDistanceMatrix) {
        cout << "Computing the auxiliary distance matrix for backward direction." << endl;
        delete distanceMatrix;
        IntegerDistanceMatrixComputor dmComputor;
        dmComputor.computeDistanceMatrixInReversedGraph(originalGraph);
        distanceMatrix = dmComputor.getDistanceMatrixInstance();
        cout << "Distance matrix computed." << endl;
    }

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        if(i % 200 == 0) {
            cout << "Computed backward access nodes for '" << i << "' transit nodes." << endl;
        }

        findBackwardAccessNodes(i, backwardAccessNodes[i], backwardSearchSpaces[i], transitNodesMapping, transitNodesDistanceTable, chGraph, originalGraph, regions, useDistanceMatrix);
    }
    cout << "True arc flags: " << trueArcFlags << ", total arc flags: " << totalArcFlags << ", that is: " << ((double) trueArcFlags / totalArcFlags) * 100 << "%." << endl;

    if(useDistanceMatrix) {
        delete distanceMatrix;
        distanceMatrix = NULL;
    }

    /*cout << "Computing locality filter" << endl;
    vector < vector < bool > > isLocal(graph.nodes(), vector < bool> (graph.nodes(), false));
    prepareLocalityFilter(isLocal, forwardSearchSpaces, backwardSearchSpaces);*/

    vector < pair < unsigned int, IntegerQueryEdge > > allEdges;
    chGraph.getEdgesForFlushing(allEdges);

    outputGraph(outputPath, graph, allEdges, transitNodes, transitNodesDistanceTable, forwardAccessNodes, backwardAccessNodes, forwardSearchSpaces, backwardSearchSpaces, transitNodesAmount, regions, regionsCnt);
}

//______________________________________________________________________________________________________________________
void
TNRAFPreprocessor::computeTransitNodeDistanceTable(vector<unsigned int> & transitNodes, vector<vector<unsigned int>> & distanceTable, unsigned int transitNodesCnt, IntegerGraph & originalGraph) {
    for(unsigned int i = 0; i < transitNodesCnt; i++) {
        if(i % 100 == 0) {
            cout << "Computed transit nodes distances for '" << i << "' transit nodes." << endl;
        }

        vector<unsigned int> distancesFromNodeI(originalGraph.nodes());
        BasicIntegerDijkstra::computeOneToAllDistances(transitNodes[i], originalGraph, distancesFromNodeI);
        for(unsigned int j = 0; j < transitNodesCnt; j++) {
            distanceTable[i][j] = distancesFromNodeI[transitNodes[j]];
        }
    }
}

// Outputs the created Transit Node Routing data-structure with all the information required for the query algorithm
// into a binary file.
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::outputGraph(string outputPath, IntegerUpdateableGraph & graph, vector < pair < unsigned int, IntegerQueryEdge > > & allEdges, vector < unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, vector < vector < AccessNodeDataArcFlags > > & forwardAccessNodes, vector < vector < AccessNodeDataArcFlags > > & backwardAccessNodes, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces, unsigned int transitNodesAmount, RegionsStructure & regions, unsigned int regionsCnt) {
    cout << "Outputting TNR" << endl;
    ofstream output;
    output.open ( outputPath + ".tgaf", ios::binary );
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (outputPath + ".tgaf").c_str());
    }

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
    unsigned int edges = allEdges.size();
    output.write((char *) &nodes, sizeof(nodes));
    output.write((char *) &edges, sizeof(edges));
    output.write((char *) &transitNodesAmount, sizeof(transitNodesAmount));
    output.write((char *) &regionsCnt, sizeof(regionsCnt));

    // Fixme - will need to actually output the regions for all nodes somewhere here

    for(unsigned int i = 0; i < allEdges.size(); i++) {
        output.write((char *) &allEdges[i].first, sizeof(allEdges[i].first));
        output.write((char *) &allEdges[i].second.targetNode, sizeof(allEdges[i].second.targetNode));
        unsigned int weight = allEdges[i].second.weight;
        output.write((char *) &weight, sizeof(weight));
        output.write((char *) &allEdges[i].second.forward, sizeof(allEdges[i].second.forward));
        output.write((char *) &allEdges[i].second.backward, sizeof(allEdges[i].second.backward));
        bool t = true;
        bool f = false;
        //printf("Check forward shortcut.\n");
        //printf("Check forward shortcut. (%u -> %u, fwflag: %u)\n", allEdges[i].first, allEdges[i].second.targetNode, allEdges[i].second.forward);
        if (allEdges[i].second.forward && graph.isShortcut(allEdges[i].first, allEdges[i].second.targetNode)) {
            output.write((char *) &t, sizeof(t));
        } else {
            output.write((char *) &f, sizeof(f));
        }

        //printf("Check backward shortcut. (%u -> %u, bwflag: %u)\n", allEdges[i].second.targetNode, allEdges[i].first, allEdges[i].second.backward);
        if (allEdges[i].second.backward && graph.isShortcut(allEdges[i].second.targetNode, allEdges[i].first)) {
            output.write((char *) &t, sizeof(t));
        } else {
            output.write((char *) &f, sizeof(f));
        }
    }

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        unsigned int rank = graph.getRank(i);
        output.write((char *) &rank, sizeof(rank));
    }

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        unsigned int region = regions.getRegion(i);
        output.write((char *) &region, sizeof(region));
    }

    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        output.write((char *) &transitNodes[i], sizeof(transitNodes[i]));
    }

    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        for(unsigned int j = 0; j < transitNodesAmount; j++) {
            output.write((char *) &transitNodesDistanceTable[i][j], sizeof(transitNodesDistanceTable[i][j]));
        }
    }

    // FIXME: For access nodes, one will need to output the arc flags somehow.
    vector<uint32_t> powersOf2(regionsCnt);
    initPowersOf2(powersOf2);

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        unsigned int fwSize = forwardAccessNodes[i].size();
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
        unsigned int bwSize = backwardAccessNodes[i].size();
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

            // FIXME debug stuff
            totalAccessNodes++;
            if(regionsOutput == 0) {
                uselessAccessNodes++;
            }
            output.write((char *) &regionsOutput, sizeof(regionsOutput));
        }
    }

    printf("Useless access nodes: %u, total access nodes: %u, that means %lf %% were useless.\n", uselessAccessNodes, totalAccessNodes, ((double) uselessAccessNodes / totalAccessNodes) * 100);
    printf("Tried %llu combinations when determining access nodes.\n", triedCombinations);

    printf("Will now output search spaces.\n");

    unsigned int fwSearchSpaceSum = 0;
    unsigned int bwSearchSpaceSum = 0;
    for(unsigned int i = 0; i < graph.nodes(); i++) {
        unsigned int fwSearchSpaceSize = forwardSearchSpaces[i].size();
        output.write((char *) &fwSearchSpaceSize, sizeof(fwSearchSpaceSize));
        for(unsigned int j = 0; j < fwSearchSpaceSize; j++) {
            output.write((char *) &forwardSearchSpaces[i][j], sizeof(forwardSearchSpaces[i][j]));
        }
        fwSearchSpaceSum += fwSearchSpaceSize;

        unsigned int bwSearchSpaceSize = backwardSearchSpaces[i].size();
        output.write((char *) &bwSearchSpaceSize, sizeof(bwSearchSpaceSize));
        for(unsigned int j = 0; j < bwSearchSpaceSize; j++) {
            output.write((char *) &backwardSearchSpaces[i][j], sizeof(backwardSearchSpaces[i][j]));
        }
        bwSearchSpaceSum += bwSearchSpaceSize;

    }

    printf("Average forward search space size: %lf\n", (double)fwSearchSpaceSum/nodes);
    printf("Average backward search space size: %lf\n", (double)bwSearchSpaceSum/nodes);


    /*for(unsigned int i = 0; i < graph.nodes(); i++) {
        for(unsigned int j = 0; j < graph.nodes(); j++) {
            bool flag = isLocal[i][j];
            output.write((char *) &flag, sizeof(flag));
        }
    }*/

    output.close();
}

// Auxiliary function that will find forward access nodes for a given node. The process consists of first finding all
// the access nodes candidates using a simplified version of the Contraction Hierarchies query algorithm,
// some of the candidates can then be removed by a simple stalling process.
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::findForwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & forwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph, IntegerGraph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix) {
    auto cmp = [](IntegerDijkstraNode left, IntegerDijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<IntegerDijkstraNode, vector<IntegerDijkstraNode>, decltype(cmp)> forwardQ(cmp);
    vector<unsigned int> distances(graph.nodes(), UINT_MAX);
    vector<bool> settled(graph.nodes(), false);

    forwardQ.push(IntegerDijkstraNode(source, 0));

    distances[source] = 0;

    vector < AccessNodeData > accessNodesSuperset;

    while (! forwardQ.empty() ) {
        unsigned int curNode = forwardQ.top().ID;
        long long unsigned int curLen = forwardQ.top().weight;
        forwardQ.pop();

        if(settled[curNode]) {
            continue;
        }

        settled[curNode] = true;
        if(transitNodes.count(curNode) == 1) {
            accessNodesSuperset.push_back(AccessNodeData(curNode, curLen));
        } else {
            forwardSearchSpaces.push_back(curNode);

            const vector<IntegerQueryEdge> & neighbours = graph.nextNodes(curNode);
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
                    long long unsigned int newlen = curLen + (*iter).weight;
                    if (newlen < distances[(*iter).targetNode]) {
                        distances[(*iter).targetNode] = newlen;
                        forwardQ.push(IntegerDijkstraNode((*iter).targetNode, newlen));
                    }
                }
            }
        }

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
            accessNodes.push_back(AccessNodeDataArcFlags(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode, regions.getRegionsCnt()));
        }
    }

    /*for(unsigned int i = 0; i < accessNodesSuperset.size(); i++) {
        bool validAccessNode = true;
        for(unsigned int j = 0; j < accessNodesSuperset.size(); j++) {
            if (j != i) {
                unsigned int t1 = transitNodes[accessNodesSuperset[j].accessNodeID];
                unsigned int t2 = transitNodes[accessNodesSuperset[i].accessNodeID];
                if (accessNodesSuperset[j].distanceToNode + transitNodesDistanceTable[t1][t2] <= accessNodesSuperset[i].distanceToNode) {
                    //validAccessNode = false;
                    break;
                }
            }
        }

        if (validAccessNode) {
            accessNodes.push_back(AccessNodeDataArcFlags(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode, regions.getRegionsCnt()));
        }

    }*/

    computeForwardArcFlags(source, accessNodes, originalGraph, regions, useDistanceMatrix);

}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::computeForwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, IntegerGraph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix) {
    if(useDistanceMatrix) {
        for (unsigned int i = 0; i < accessNodes.size(); i++) {
            unsigned int accessNode = accessNodes[i].accessNodeID;
            unsigned int distanceToAccessNode = accessNodes[i].distanceToNode;
            for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
                vector<unsigned int> & nodesInRegion = regions.nodesInRegion(j);
                totalArcFlags++;
                //printf("Determining arc flag %u for %u (access node of %u). Amount of nodes in region: %lu\n", j, accessNode, node, nodesInRegion.size());
                if(distanceToAccessNode != distanceMatrix->findDistance(node, accessNode)) {
                    incorrectANdistances++;
                }
                ANdistances++;
                for (unsigned int k = 0; k < nodesInRegion.size(); k++) {
                    /*if(nodesInRegion[k] == accessNode) {
                        printf("Distance %u -> %u: %u while distance %u -> %u: %u + %u -> %u: %u = %u\n",
                               node, nodesInRegion[k], distanceMatrix->findDistance(node, nodesInRegion[k]),
                               node, accessNode, distanceToAccessNode,
                               accessNode, nodesInRegion[k], distanceMatrix->findDistance(accessNode, nodesInRegion[k]),
                               distanceMatrix->findDistance(accessNode, nodesInRegion[k]) + distanceToAccessNode);
                    }*/
                    triedCombinations++;
                    if (distanceMatrix->findDistance(node, nodesInRegion[k]) ==
                        distanceMatrix->findDistance(accessNode, nodesInRegion[k]) + distanceToAccessNode) {
                        accessNodes[i].regionFlags[j] = true;
                        trueArcFlags++;
                        //printf("The flag was set to true.\n");
                        break;
                    }
                }
            }
        }
    } else {
        vector<unsigned int> distancesFromNode(originalGraph.nodes());
        BasicIntegerDijkstra::computeOneToAllDistances(node, originalGraph, distancesFromNode);
        for (unsigned int i = 0; i < accessNodes.size(); i++) {
            vector<unsigned int> distancesFromAccessNode(originalGraph.nodes());
            unsigned int accessNode = accessNodes[i].accessNodeID;
            BasicIntegerDijkstra::computeOneToAllDistances(accessNode, originalGraph, distancesFromAccessNode);
            unsigned int distanceToAccessNode = distancesFromAccessNode[node];
            for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
                vector<unsigned int> &nodesInRegion = regions.nodesInRegion(j);
                totalArcFlags++;
                for (unsigned int k = 0; k < nodesInRegion.size(); k++) {
                    if (distancesFromNode[nodesInRegion[k]] ==
                        distancesFromAccessNode[nodesInRegion[k]] + distanceToAccessNode) {
                        accessNodes[i].regionFlags[j] = true;
                        trueArcFlags++;
                        break;
                    }
                }
            }
        }
    }
}

// Auxiliary function that will find backward access nodes for a given node. The process consists of first finding all
// the access nodes candidates using a simplified version of the Contraction Hierarchies query algorithm,
// some of the candidates can then be removed by a simple stalling process.
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::findBackwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & backwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph, IntegerGraph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix) {
    auto cmp = [](IntegerDijkstraNode left, IntegerDijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<IntegerDijkstraNode, vector<IntegerDijkstraNode>, decltype(cmp)> backwardQ(cmp);
    vector<unsigned int> distances(graph.nodes(), UINT_MAX);
    vector<bool> settled(graph.nodes(), false);

    backwardQ.push(IntegerDijkstraNode(source, 0));

    distances[source] = 0;

    vector < AccessNodeData > accessNodesSuperset;

    while (! backwardQ.empty() ) {
        unsigned int curNode = backwardQ.top().ID;
        long long unsigned int curLen = backwardQ.top().weight;
        backwardQ.pop();

        if(settled[curNode]) {
            continue;
        }

        settled[curNode] = true;
        if(transitNodes.count(curNode) == 1) {
            accessNodesSuperset.push_back(AccessNodeData(curNode, curLen));
        } else {
            backwardSearchSpaces.push_back(curNode);

            const vector<IntegerQueryEdge> & neighbours = graph.nextNodes(curNode);
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
                    long long unsigned int newlen = curLen + (*iter).weight;
                    if (newlen < distances[(*iter).targetNode]) {
                        distances[(*iter).targetNode] = newlen;
                        backwardQ.push(IntegerDijkstraNode((*iter).targetNode, newlen));
                    }
                }
            }
        }

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
            accessNodes.push_back(AccessNodeDataArcFlags(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode, regions.getRegionsCnt()));
        }
    }

    /*for(unsigned int i = 0; i < accessNodesSuperset.size(); i++) {
        bool validAccessNode = true;
        for(unsigned int j = 0; j < accessNodesSuperset.size(); j++) {
            if (j != i) {
                unsigned int t1 = transitNodes[accessNodesSuperset[j].accessNodeID];
                unsigned int t2 = transitNodes[accessNodesSuperset[i].accessNodeID];
                if (accessNodesSuperset[j].distanceToNode + transitNodesDistanceTable[t1][t2] <= accessNodesSuperset[i].distanceToNode) {
                    //validAccessNode = false;
                    break;
                }
            }
        }

        if (validAccessNode) {
            accessNodes.push_back(AccessNodeDataArcFlags(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode, regions.getRegionsCnt()));
        }

    }*/

    computeBackwardArcFlags(source, accessNodes, originalGraph, regions, useDistanceMatrix);

}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::computeBackwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, IntegerGraph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix) {
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
        vector<unsigned int> distancesFromNode(originalGraph.nodes());
        BasicIntegerDijkstra::computeOneToAllDistancesInReversedGraph(node, originalGraph, distancesFromNode);
        for (unsigned int i = 0; i < accessNodes.size(); i++) {
            vector<unsigned int> distancesFromAccessNode(originalGraph.nodes());
            unsigned int accessNode = accessNodes[i].accessNodeID;
            BasicIntegerDijkstra::computeOneToAllDistancesInReversedGraph(accessNode, originalGraph,
                                                                          distancesFromAccessNode);
            unsigned int distanceToAccessNode = distancesFromAccessNode[node];
            for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
                vector<unsigned int> &nodesInRegion = regions.nodesInRegion(j);
                for (unsigned int k = 0; k < nodesInRegion.size(); k++) {
                    if (distancesFromNode[nodesInRegion[k]] ==
                        distancesFromAccessNode[nodesInRegion[k]] + distanceToAccessNode) {
                        accessNodes[i].regionFlags[j] = true;
                        break;
                    }
                }
            }
        }
    }
}

// Completely random clustering just for testing purposes. FIXME
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::generateClustering(IntegerGraph & originalGraph, RegionsStructure & regions, unsigned int clustersCnt) {
    /*for(unsigned int i = 0; i < originalGraph.nodes(); i++) {
        regions.addNode(i, i % clustersCnt);
    }*/

    // Testing dumb clustering
    /*for(unsigned int i = 0; i < originalGraph.nodes(); i++) {
        for(unsigned int j = 0; j < clustersCnt; j++) {
            regions.addNode(i, j);
        }
    }*/

    unsigned int nodesCnt = originalGraph.nodes();
    unsigned int approxNodesPerCluster = nodesCnt / clustersCnt;
    unsigned int unassignedNodes = nodesCnt - clustersCnt;
    vector < unsigned int > assignedClusters(nodesCnt, UINT_MAX);
    vector < queue < unsigned int > > q(clustersCnt, queue<unsigned int>());

    clusteringSkips = 0;

    for(unsigned int i = 0; i < clustersCnt; ++i) {
        const unsigned int nodeID = approxNodesPerCluster * i;
        assignedClusters[nodeID] = i;
        const vector<pair<unsigned int, unsigned long long int>> & neighboursFW = originalGraph.outgoingEdges(nodeID);
        for(unsigned int j = 0; j < neighboursFW.size(); ++j) {
            q[i].push(neighboursFW[j].first);
        }
        const vector<pair<unsigned int, unsigned long long int>> & neighboursBW = originalGraph.incomingEdges(nodeID);
        for(unsigned int j = 0; j < neighboursBW.size(); ++j) {
            q[i].push(neighboursBW[j].first);
        }
    }

    bool jumpout = false;
    while(! jumpout) {
        for(unsigned int i = 0; i < clustersCnt; ++i) {
            const unsigned int newNodeForCluster = getNewNodeForCluster(assignedClusters, q[i]);

            assignedClusters[newNodeForCluster] = i;
            const vector<pair<unsigned int, unsigned long long int>> & neighboursFW = originalGraph.outgoingEdges(newNodeForCluster);
            for(unsigned int j = 0; j < neighboursFW.size(); ++j) {
                q[i].push(neighboursFW[j].first);
            }
            const vector<pair<unsigned int, unsigned long long int>> & neighboursBW = originalGraph.incomingEdges(newNodeForCluster);
            for(unsigned int j = 0; j < neighboursBW.size(); ++j) {
                q[i].push(neighboursBW[j].first);
            }

            unassignedNodes--;
            if(unassignedNodes == 0) {
                jumpout = true;
                break;
            }

            if(unassignedNodes % 2000 == 0) {
                printf("There are %u nodes left to be assigned to clusters.\n", unassignedNodes);
            }
        }
    }

    for(unsigned int i = 0; i < nodesCnt; ++i) {
        regions.addNode(i, assignedClusters[i]);
    }

    printf("%u skips occured during the clustering process. That is %lf %% of nodes.\n", clusteringSkips, ((double) clusteringSkips / nodesCnt) * 100);

}

// Finds a node to add into the currently expanded cluster. First tries to get an unassigned node from the queue, if no
// such node exists, assigns some other unassigned node.
//______________________________________________________________________________________________________________________
const unsigned int TNRAFPreprocessor::getNewNodeForCluster( vector < unsigned int > & assignedClusters, queue < unsigned int > & q) {
    while(! q.empty()) {
        unsigned int nodeID = q.front();
        q.pop();
        if(assignedClusters[nodeID] == UINT_MAX) {
            return nodeID;
        }
    }

    for(unsigned int i = 0; i < assignedClusters.size(); ++i) {
        if(assignedClusters[i] == UINT_MAX) {
            clusteringSkips++;
            return i;
        }
    }
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

