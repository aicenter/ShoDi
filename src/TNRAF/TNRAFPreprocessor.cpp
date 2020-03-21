//
// Author: Xenty (Michal Cvach)
// Created on: 30.11.19
//

#include <iostream>
#include <climits>
#include <fstream>
#include "TNRAFPreprocessor.h"
#include "../CH/CHDistanceQueryManager.h"
#include "Structures/AccessNodeDataArcFlags.h"
#include "../Dijkstra/DijkstraNode.h"
#include "../Dijkstra/BasicDijkstra.h"
#include "../DistanceMatrix/DistanceMatrixComputor.h"
#include "../DistanceMatrix/DistanceMatrix.h"

// FIXME debug stuff
unsigned int TNRAFPreprocessor::totalArcFlags;
unsigned int TNRAFPreprocessor::trueArcFlags;
unsigned int TNRAFPreprocessor::totalAccessNodes;
unsigned int TNRAFPreprocessor::uselessAccessNodes;
unsigned int TNRAFPreprocessor::triedCombinations;
unsigned int TNRAFPreprocessor::incorrectANdistances;
unsigned int TNRAFPreprocessor::ANdistances;
unsigned int TNRAFPreprocessor::clusteringSkips;
DistanceMatrix * TNRAFPreprocessor::distanceMatrix = NULL;

// The main workhorse function. The result of this function will be the data-structure required for the Transit Node
// Routing with Arc Flags query algorithm saved in a binary file. Since we are working with Transit Node Routing based
// on Contraction Hierarchies, this function expects that the UpdateableGraph instance was already processed by the
// CHPreprocessor and therefore already contains Contraction Hierarchies information such as ranks and shortcut edges.
// The user can set the amount of transit nodes that will be used, this is done by setting the transitNodesAmount
// variable. The regionsCnt variable should control the amount of regions that will be used for the Arc Flags
// functionality, but only 32 are working currently due to the way the output is handled right now. The bool variable
// useDistanceMatrix allows the user to switch between two preprocessing modes. If the flag is set to true,
// preprocessing uses the a full Distance Matrix to speed up the process. This leads to significant memory requirements,
// but the preprocessing itself can then be very fast. This should be used if you have more resources available during
// the preprocessing phase than during the actual queries. If the flag is set to false, the preprocessing will be much
// slower, but will require less memory and it should then be possible to complete it even on machines where there is
// not enough memory for the faster mode.
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
        DistanceMatrixComputor dmComputor;
        dmComputor.computeDistanceMatrix(originalGraph);
        distanceMatrix = dmComputor.getDistanceMatrixInstance();
        cout << "Distance matrix computed." << endl;

        fillTransitNodeDistanceTable(transitNodes, transitNodesDistanceTable, transitNodesAmount);
    } else {
        computeTransitNodeDistanceTable(transitNodes, transitNodesDistanceTable, transitNodesAmount, originalGraph);
    }

    cout << "Generating regions for the nodes" << endl;
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

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        if(i % 200 == 0) {
            cout << "Computed forward access nodes for '" << i << "' transit nodes." << endl;
        }

        findForwardAccessNodes(i, forwardAccessNodes[i], forwardSearchSpaces[i], transitNodesMapping, transitNodesDistanceTable, chGraph, originalGraph, regions, useDistanceMatrix);
    }

    //printf("Useless access nodes: %u, total access nodes: %u, that means %lf %% were useless.\n", uselessAccessNodes, totalAccessNodes, ((double) uselessAccessNodes / totalAccessNodes) * 100);
    printf("Tried %u combinations when determining access nodes.\n", triedCombinations);
    printf("Additionally, %u out of %u access nodes have incorrect distances. That is %lf %%.\n", incorrectANdistances, ANdistances, ((double) incorrectANdistances / ANdistances) * 100);

    if(useDistanceMatrix) {
        cout << "Computing the auxiliary distance matrix for backward direction." << endl;
        delete distanceMatrix;
        DistanceMatrixComputor dmComputor;
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

    vector < pair < unsigned int, QueryEdge > > allEdges;
    chGraph.getEdgesForFlushing(allEdges);

    outputGraph(outputPath, graph, allEdges, transitNodes, transitNodesDistanceTable, forwardAccessNodes, backwardAccessNodes, forwardSearchSpaces, backwardSearchSpaces, transitNodesAmount, regions, regionsCnt);
}

// Computes the full distance matrix for the transit node set. This is done by t runs of one-to-all Dijkstra (where t
// denotes the amount of transit nodes).
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::computeTransitNodeDistanceTable(vector<unsigned int> & transitNodes, vector<vector<unsigned int>> & distanceTable, unsigned int transitNodesCnt, Graph & originalGraph) {
    for(unsigned int i = 0; i < transitNodesCnt; i++) {
        if(i % 100 == 0) {
            cout << "Computed transit nodes distances for '" << i << "' transit nodes." << endl;
        }

        vector<unsigned int> distancesFromNodeI(originalGraph.nodes());
        BasicDijkstra::computeOneToAllDistances(transitNodes[i], originalGraph, distancesFromNodeI);
        for(unsigned int j = 0; j < transitNodesCnt; j++) {
            distanceTable[i][j] = distancesFromNodeI[transitNodes[j]];
        }
    }
}

// Fills the full distance matrix for the transit node set using value from the full distance matrix for the graph,
// when the distance matrix is used to speed up the preprocessing phase.
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::fillTransitNodeDistanceTable(vector<unsigned int> & transitNodes, vector<vector<unsigned int>> & distanceTable, unsigned int transitNodesCnt) {
    for(unsigned int i = 0; i < transitNodesCnt; i++) {
        for(unsigned int j = 0; j < transitNodesCnt; j++) {
            distanceTable[i][j] = distanceMatrix->findDistance(transitNodes[i], transitNodes[j]);
        }
    }
}

// Outputs the created Transit Node Routing with Arc Flags data-structure with all the information required for
// the query algorithm into a binary file.
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::outputGraph(string outputPath, UpdateableGraph & graph, vector < pair < unsigned int, QueryEdge > > & allEdges, vector < unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, vector < vector < AccessNodeDataArcFlags > > & forwardAccessNodes, vector < vector < AccessNodeDataArcFlags > > & backwardAccessNodes, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces, unsigned int transitNodesAmount, RegionsStructure & regions, unsigned int regionsCnt) {
    cout << "Outputting TNR" << endl;
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
    unsigned int edges = allEdges.size();
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
    printf("Tried %u combinations when determining access nodes.\n", triedCombinations);

    // Output search spaces (those are needed for the locality filter).
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

    output.close();
}

// Auxiliary function that will find forward access nodes for a given node. The process consists of first finding all
// the access nodes candidates using a simplified version of the Contraction Hierarchies query algorithm,
// some of the candidates can then be removed if their distances are incorrect.
// Arc Flags then have to be computed for the actual set of access nodes.
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::findForwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & forwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, FlagsGraph & graph, Graph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix) {
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
            forwardSearchSpaces.push_back(curNode);

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

// Computes Arc Flags for a set of forward access nodes of a given node. This is done simply by comparing the distances.
// If the distance from node to the access node plus the distance from the access node to some node from the region
// is equal to the distance from node to the same node from the region, then the flag for that region is set to true
// because we have found a shortest path to some node in the region going through our access node. If we do not find any
// node from some region for which the access node would be on a shortest path, then the Arc Flag for this region can be
// set to false.
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::computeForwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, Graph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix, vector<unsigned int> & optionalDistancesFromNode) {
    if(useDistanceMatrix) {
        for (unsigned int i = 0; i < accessNodes.size(); i++) {
            unsigned int accessNode = accessNodes[i].accessNodeID;
            unsigned int distanceToAccessNode = accessNodes[i].distanceToNode;
            for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
                vector<unsigned int> & nodesInRegion = regions.nodesInRegion(j);
                totalArcFlags++;
                if(distanceToAccessNode != distanceMatrix->findDistance(node, accessNode)) {
                    incorrectANdistances++;
                }
                ANdistances++;
                for (unsigned int k = 0; k < nodesInRegion.size(); k++) {
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
        for (unsigned int i = 0; i < accessNodes.size(); i++) {
            vector<unsigned int> distancesFromAccessNode(originalGraph.nodes());
            unsigned int accessNode = accessNodes[i].accessNodeID;
            BasicDijkstra::computeOneToAllDistances(accessNode, originalGraph, distancesFromAccessNode);
            unsigned int distanceToAccessNode = distancesFromAccessNode[node];
            for (unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
                vector<unsigned int> &nodesInRegion = regions.nodesInRegion(j);
                totalArcFlags++;
                for (unsigned int k = 0; k < nodesInRegion.size(); k++) {
                    if (optionalDistancesFromNode[nodesInRegion[k]] ==
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
// some of the candidates can then be removed if their distances are incorrect.
// Arc Flags then have to be computed for the actual set of access nodes.
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::findBackwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & backwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, FlagsGraph & graph, Graph & originalGraph, RegionsStructure & regions, bool useDistanceMatrix) {
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
            backwardSearchSpaces.push_back(curNode);

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

// Computes the backward Arc Flags for a set of access nodes of a given node. This is similar to the forward direction,
// so check computeForwardArcFlags for more information.
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
            unsigned int distanceToAccessNode = distancesFromAccessNode[node];
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

// Generates a clustering that is used for the regions (and those are used for Arc Flags). The query algorithm will work
// with any clustering where each node is assigned to exactly one cluster. Clusterings where we have clusters of equal
// sizes and nodes that are close to each other are also in the same cluster give better performance though.
// Here we use a very simple clustering that is based on the k-means idea. We first initialize our k clusters
// by assigning one node to each of them. We then try to expand each cluster by adding some node that is connected to
// some node already in the cluster that has not been assigned to any cluster so far. If no such node can be found,
// we just randomly add some other node that does not have a cluster yet to the cluster in order to keep the size
// of the clusters somewhat balanced. This approach seems to give solid results.
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::generateClustering(Graph & originalGraph, RegionsStructure & regions, unsigned int clustersCnt) {
    unsigned int nodesCnt = originalGraph.nodes();
    unsigned int approxNodesPerCluster = nodesCnt / clustersCnt;
    unsigned int unassignedNodes = nodesCnt - clustersCnt;
    vector < unsigned int > assignedClusters(nodesCnt, UINT_MAX);
    vector < queue < unsigned int > > q(clustersCnt, queue<unsigned int>());

    clusteringSkips = 0;

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

// Auxiliary function in order to make sure the powers of 2 are precomputed.
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::getPowersOf2(vector<uint32_t> & powersOf2) {
    initPowersOf2(powersOf2);
}

// Auxiliary function that precomputes powers of 2 up to 2^32. This is used for the Arc Flags output. In order to output
// Arc Flags efficiently, we output 32 Arc Flags as one unsigned int. To set individual Arc Flags, we can add or
// subtract powers of 2. We prepare those powers of 2 using this function to speed up the process.
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::initPowersOf2(vector<uint32_t> & powersOf2) {
    uint32_t val = 1;
    for(unsigned int i = 0; i < 32; i++) {
        powersOf2[i] = val;
        val = val * 2;
    }
}

