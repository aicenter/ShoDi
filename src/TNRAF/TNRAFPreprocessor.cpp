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

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::preprocessUsingCH(IntegerUpdateableGraph & graph, IntegerGraph & originalGraph, string outputPath, unsigned int transitNodesAmount, unsigned int regionsCnt) {
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

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        if(i % 100 == 0) {
            cout << "Computed access nodes for '" << i << "' transit nodes." << endl;
        }

        findForwardAccessNodes(i, forwardAccessNodes[i], forwardSearchSpaces[i], transitNodesMapping, transitNodesDistanceTable, chGraph, originalGraph, regions);
        findBackwardAccessNodes(i, backwardAccessNodes[i], backwardSearchSpaces[i], transitNodesMapping, transitNodesDistanceTable, chGraph, originalGraph, regions);
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
            output.write((char *) &regionsOutput, sizeof(regionsOutput));
        }
    }

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
void TNRAFPreprocessor::findForwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & forwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph, IntegerGraph & originalGraph, RegionsStructure & regions) {
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

    }

    computeForwardArcFlags(source, accessNodes, originalGraph, regions);

}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::computeForwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, IntegerGraph & originalGraph, RegionsStructure & regions) {
    vector<unsigned int> distancesFromNode(originalGraph.nodes());
    BasicIntegerDijkstra::computeOneToAllDistances(node, originalGraph, distancesFromNode);
    for(unsigned int i = 0; i < accessNodes.size(); i++) {
        vector<unsigned int> distancesFromAccessNode(originalGraph.nodes());
        unsigned int accessNode = accessNodes[i].accessNodeID;
        BasicIntegerDijkstra::computeOneToAllDistances(accessNode, originalGraph, distancesFromAccessNode);
        unsigned int distanceToAccessNode = distancesFromAccessNode[node];
        for(unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
            vector < unsigned int > & nodesInRegion = regions.nodesInRegion(j);
            for(unsigned int k = 0; k < nodesInRegion.size(); k++) {
                if(distancesFromNode[nodesInRegion[k]] == distancesFromAccessNode[nodesInRegion[k]] + distanceToAccessNode) {
                    accessNodes[i].regionFlags[j] = true;
                    break;
                }
            }
        }
    }
}

// Auxiliary function that will find backward access nodes for a given node. The process consists of first finding all
// the access nodes candidates using a simplified version of the Contraction Hierarchies query algorithm,
// some of the candidates can then be removed by a simple stalling process.
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::findBackwardAccessNodes(unsigned int source, vector <AccessNodeDataArcFlags> & accessNodes, vector < unsigned int > & backwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph, IntegerGraph & originalGraph, RegionsStructure & regions) {
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

    }

    computeBackwardArcFlags(source, accessNodes, originalGraph, regions);

}

//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::computeBackwardArcFlags(unsigned int node, vector<AccessNodeDataArcFlags> & accessNodes, IntegerGraph & originalGraph, RegionsStructure & regions) {
    vector<unsigned int> distancesFromNode(originalGraph.nodes());
    BasicIntegerDijkstra::computeOneToAllDistancesInReversedGraph(node, originalGraph, distancesFromNode);
    for(unsigned int i = 0; i < accessNodes.size(); i++) {
        vector<unsigned int> distancesFromAccessNode(originalGraph.nodes());
        unsigned int accessNode = accessNodes[i].accessNodeID;
        BasicIntegerDijkstra::computeOneToAllDistancesInReversedGraph(accessNode, originalGraph, distancesFromAccessNode);
        unsigned int distanceToAccessNode = distancesFromAccessNode[node];
        for(unsigned int j = 0; j < regions.getRegionsCnt(); j++) {
            vector < unsigned int > & nodesInRegion = regions.nodesInRegion(j);
            for(unsigned int k = 0; k < nodesInRegion.size(); k++) {
                if(distancesFromNode[nodesInRegion[k]] == distancesFromAccessNode[nodesInRegion[k]] + distanceToAccessNode) {
                    accessNodes[i].regionFlags[j] = true;
                    break;
                }
            }
        }
    }
}

// Completely random clustering just for testing purposes. FIXME
//______________________________________________________________________________________________________________________
void TNRAFPreprocessor::generateClustering(IntegerGraph & originalGraph, RegionsStructure & regions, unsigned int clustersCnt) {
    for(unsigned int i = 0; i < originalGraph.nodes(); i++) {
        regions.addNode(i, i % clustersCnt);
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

