//
// Author: Xenty (Michal Cvach)
// Created on: 05.08.19
//

#include <climits>
#include <fstream>
#include <iostream>
#include "TNRPreprocessor.h"
#include "../GraphBuilding/Structures/IntegerStructures/IntegerFlagsGraph.h"
#include "../CH/Integer/IntegerCHDistanceQueryManager.h"
#include "Structures/AccessNodeData.h"
#include "../Dijkstra/IntegerDijkstra/IntegerDijkstraNode.h"

// Builds Transit Node Routing structures based on Contraction Hierarchies. User can set the amount of Transit Nodes.
// Generally, more transit nodes mean more memory consumption as more distances must be present in the distance table,
// but could potentially speed up the queries as more queries will hit those new transit nodes.
//______________________________________________________________________________________________________________________
void TNRPreprocessor::preprocessUsingCH(IntegerUpdateableGraph & graph, string outputPath, unsigned int transitNodesAmount) {
    cout << "Getting transit nodes" << endl;
    vector < unsigned int > transitNodes(transitNodesAmount);
    graph.getNodesWithHighestRank(transitNodes, transitNodesAmount);

    cout << "Computing transit nodes distance table" << endl;
    IntegerFlagsGraph chGraph(graph);
    IntegerCHDistanceQueryManager qm(chGraph);
    vector < vector < unsigned int > > transitNodesDistanceTable(transitNodesAmount, vector < unsigned int > (transitNodesAmount));
    for(unsigned int i = 0; i < transitNodesAmount; i++) {
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
    }

    cout << "Computing access nodes" << endl;
    vector < vector < AccessNodeData > > forwardAccessNodes(graph.nodes());
    vector < vector < AccessNodeData > > backwardAccessNodes(graph.nodes());
    vector < vector < unsigned int > > forwardSearchSpaces(graph.nodes());
    vector < vector < unsigned int > > backwardSearchSpaces(graph.nodes());
    unordered_map < unsigned int, unsigned int > transitNodesMapping;
    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        transitNodesMapping.insert(make_pair(transitNodes[i], i));
    }

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        findForwardAccessNodes(i, forwardAccessNodes[i], forwardSearchSpaces[i], transitNodesMapping, transitNodesDistanceTable, chGraph);
        findBackwardAccessNodes(i, backwardAccessNodes[i], backwardSearchSpaces[i], transitNodesMapping, transitNodesDistanceTable, chGraph);
    }

    /*cout << "Computing locality filter" << endl;
    vector < vector < bool > > isLocal(graph.nodes(), vector < bool> (graph.nodes(), false));
    prepareLocalityFilter(isLocal, forwardSearchSpaces, backwardSearchSpaces);*/

    vector < pair < unsigned int, IntegerQueryEdge > > allEdges;
    chGraph.getEdgesForFlushing(allEdges);

    outputGraph(outputPath, graph, allEdges, transitNodes, transitNodesDistanceTable, forwardAccessNodes, backwardAccessNodes, forwardSearchSpaces, backwardSearchSpaces, transitNodesAmount);
}

// Outputs the created Transit Node Routing data-structure with all the information required for the query algorithm
// into a binary file.
//______________________________________________________________________________________________________________________
void TNRPreprocessor::outputGraph(string outputPath, IntegerUpdateableGraph & graph, vector < pair < unsigned int, IntegerQueryEdge > > & allEdges, vector < unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, vector < vector < AccessNodeData > > & forwardAccessNodes, vector < vector < AccessNodeData > > & backwardAccessNodes, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces, unsigned int transitNodesAmount) {
    cout << "Outputting TNR" << endl;
    ofstream output;
    output.open ( outputPath + ".tnrg", ios::binary );
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (outputPath + ".ch").c_str());
    }

    char c1, c2, c3, c4;
    c1 = 'T';
    c2 = 'N';
    c3 = 'R';
    c4 = 'G';
    output.write(&c1, sizeof (c1));
    output.write(&c2, sizeof (c2));
    output.write(&c3, sizeof (c3));
    output.write(&c4, sizeof (c4));
    unsigned int nodes = graph.nodes();
    unsigned int edges = allEdges.size();
    output.write((char *) &nodes, sizeof(nodes));
    output.write((char *) &edges, sizeof(edges));
    output.write((char *) &transitNodesAmount, sizeof(transitNodesAmount));

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
        printf("Check forward shortcut. (%u -> %u, fwflag: %u)\n", allEdges[i].first, allEdges[i].second.targetNode, allEdges[i].second.forward);
        if (allEdges[i].second.forward && graph.isShortcut(allEdges[i].first, allEdges[i].second.targetNode)) {
            output.write((char *) &t, sizeof(t));
        } else {
            output.write((char *) &f, sizeof(f));
        }

        printf("Check backward shortcut. (%u -> %u, bwflag: %u)\n", allEdges[i].second.targetNode, allEdges[i].first, allEdges[i].second.backward);
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

    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        output.write((char *) &transitNodes[i], sizeof(transitNodes[i]));
    }

    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        for(unsigned int j = 0; j < transitNodesAmount; j++) {
            output.write((char *) &transitNodesDistanceTable[i][j], sizeof(transitNodesDistanceTable[i][j]));
        }
    }

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        unsigned int fwSize = forwardAccessNodes[i].size();
        output.write((char *) &fwSize, sizeof(fwSize));
        for(unsigned int j = 0; j < forwardAccessNodes[i].size(); j++) {
            output.write((char *) &forwardAccessNodes[i][j].accessNodeID, sizeof(forwardAccessNodes[i][j].accessNodeID));
            output.write((char *) &forwardAccessNodes[i][j].distanceToNode, sizeof(forwardAccessNodes[i][j].distanceToNode));
        }
        unsigned int bwSize = backwardAccessNodes[i].size();
        output.write((char *) &bwSize, sizeof(bwSize));
        for(unsigned int j = 0; j < backwardAccessNodes[i].size(); j++) {
            output.write((char *) &backwardAccessNodes[i][j].accessNodeID, sizeof(backwardAccessNodes[i][j].accessNodeID));
            output.write((char *) &backwardAccessNodes[i][j].distanceToNode, sizeof(backwardAccessNodes[i][j].distanceToNode));
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
void TNRPreprocessor::findForwardAccessNodes(unsigned int source, vector < AccessNodeData > & accessNodes, vector < unsigned int > & forwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph) {
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
            accessNodes.push_back(AccessNodeData(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode));
        }

    }

}

// Alternative approach, does not seem to be working correctly.
/*
void TNRPreprocessor::findForwardAccessNodes(unsigned int source, vector < AccessNodeData > & accessNodes, vector < unsigned int > & forwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph) {
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
        if(transitNodes.count(curNode) == 1 && curNode != source) {
            accessNodesSuperset.push_back(AccessNodeData(curNode, curLen));

            // Check distances of neighbours for possible improvements.
            const vector<IntegerQueryEdge> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                // Skip edge if it is only in the other direction.
                if (!(*iter).forward) {
                    continue;
                }

                if (settled[(*iter).targetNode]) {
                    continue;
                }

                // We change the distances here, but don't push into the queue if the distance improves.
                if (graph.data((*iter).targetNode).rank > graph.data(curNode).rank) {
                    long long unsigned int newlen = curLen + (*iter).weight;
                    if (newlen < distances[(*iter).targetNode]) {
                        distances[(*iter).targetNode] = newlen;
                    }
                }
            }
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
            accessNodes.push_back(AccessNodeData(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode));
        }

    }

}
*/

// Auxiliary function that will find backward access nodes for a given node. The process consists of first finding all
// the access nodes candidates using a simplified version of the Contraction Hierarchies query algorithm,
// some of the candidates can then be removed by a simple stalling process.
//______________________________________________________________________________________________________________________
void TNRPreprocessor::findBackwardAccessNodes(unsigned int source, vector < AccessNodeData > & accessNodes, vector < unsigned int > & backwardSearchSpaces, unordered_map< unsigned int, unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, IntegerFlagsGraph & graph) {
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
            accessNodes.push_back(AccessNodeData(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode));
        }

    }

}

// Auxiliary function that will prepare the locality filter based on the search spaces for each node found during the
// access node computation process. Please note that currently the locality filter is a table of |n|*|n| bool values.
// This doesn't scale well as this means a quadratic space complexity. I currently use this locality filter for testing
// purposes and it is sufficient at least when using a map with around 30000 nodes. Some better method should probably
// be used later though, the article suggests "Graph Voronoi Label Compression".
//______________________________________________________________________________________________________________________
void TNRPreprocessor::prepareLocalityFilter(vector < vector < bool > > & isLocal, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces) {
    for(unsigned int i = 0; i < isLocal.size(); i++) {
        for(unsigned int j = 0; j < isLocal.size(); j++) {
            if(i != j) {
                isLocal[i][j] = notEmptySearchSpacesUnion(i, j, forwardSearchSpaces, backwardSearchSpaces);
            }
        }
    }
}

// Auxiliary function which basically takes two search spaces and validates if they have an non-empty union.
// This is required by the locality filter, non-empty union means that the query is local.
// (TNR query algorithm can't be used on local queries, fallback to some other routing algorithm is required)
//______________________________________________________________________________________________________________________
bool TNRPreprocessor::notEmptySearchSpacesUnion(unsigned int i, unsigned int j, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces) {
    //cout << "Preparing locality filter value for query: '" << i << " -> " << j << "'." << endl;
    //cout << "Forward search space size: " << forwardSearchSpaces[i].size() << ", backward size: " << backwardSearchSpaces[j].size() << endl;
    for(unsigned int k = 0; k < forwardSearchSpaces[i].size(); k++) {
        for(unsigned int m = 0; m < backwardSearchSpaces[j].size(); m++) {
            if(forwardSearchSpaces[i][k] == backwardSearchSpaces[j][m]) {
                return true;
            }
        }
    }
    return false;
}