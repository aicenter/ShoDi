//
// Author: Xenty (Michal Cvach)
// Created on: 05.08.19
//

#include <climits>
#include <fstream>
#include <iostream>
#include "TNRPreprocessor.h"
#include "../GraphBuilding/Structures/FlagsGraph.h"
#include "../CH/CHDistanceQueryManager.h"
#include "Structures/AccessNodeData.h"
#include "../Dijkstra/DijkstraNode.h"
#include "../DistanceMatrix/DistanceMatrix.h"
#include "../DistanceMatrix/DistanceMatrixComputorSlow.h"
#include "../Dijkstra/BasicDijkstra.h"

//______________________________________________________________________________________________________________________
void TNRPreprocessor::preprocessUsingCH(UpdateableGraph & graph, string outputPath, unsigned int transitNodesAmount) {
    cout << "Getting transit nodes" << endl;
    vector < unsigned int > transitNodes(transitNodesAmount);
    graph.getNodesWithHighestRank(transitNodes, transitNodesAmount);

    cout << "Computing transit nodes distance table" << endl;
    FlagsGraph chGraph(graph);
    CHDistanceQueryManager qm(chGraph);
    vector < vector < unsigned int > > transitNodesDistanceTable(transitNodesAmount, vector < unsigned int > (transitNodesAmount));
    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        if(i % 100 == 0) {
            cout << "\rComputed '" << i << "' transit nodes distance table rows.";
        }
        for(unsigned int j = 0; j < transitNodesAmount; j++) {
            if (i == j) {
                transitNodesDistanceTable[i][j] = 0;
            } else {
                transitNodesDistanceTable[i][j] = qm.findDistance(transitNodes[i], transitNodes[j]);
            }
        }
    }

    cout << "\rComputed the transit nodes distance table." << endl;

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
        if (i % 100 == 0) {
            cout << "\rComputed access nodes for '" << i << "' nodes.";
        }

        findForwardAccessNodes(i, forwardAccessNodes[i], forwardSearchSpaces[i], transitNodesMapping,chGraph);
        findBackwardAccessNodes(i, backwardAccessNodes[i], backwardSearchSpaces[i], transitNodesMapping, chGraph);
    }

    cout << "\rComputed acess nodes for all the nodes in the graph." << endl;

    vector < pair < unsigned int, QueryEdge > > allEdges;
    chGraph.getEdgesForFlushing(allEdges);

    outputGraph(outputPath, graph, allEdges, transitNodes, transitNodesDistanceTable, forwardAccessNodes, backwardAccessNodes, forwardSearchSpaces, backwardSearchSpaces, transitNodesAmount);
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::preprocessUsingCHslower(UpdateableGraph & graph, Graph & originalGraph, string outputPath, unsigned int transitNodesAmount) {
    cout << "Getting transit nodes" << endl;
    vector < unsigned int > transitNodes(transitNodesAmount);
    graph.getNodesWithHighestRank(transitNodes, transitNodesAmount);

    cout << "Computing transit nodes distance table" << endl;
    FlagsGraph chGraph(graph);
    CHDistanceQueryManager qm(chGraph);
    vector < vector < unsigned int > > transitNodesDistanceTable(transitNodesAmount, vector < unsigned int > (transitNodesAmount));
    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        if(i % 100 == 0) {
            cout << "\rComputed '" << i << "' transit nodes distance table rows.";
        }

        vector<unsigned int> distancesFromNodeI(originalGraph.nodes());
        BasicDijkstra::computeOneToAllDistances(transitNodes[i], originalGraph, distancesFromNodeI);
        for(unsigned int j = 0; j < transitNodesAmount; j++) {
            transitNodesDistanceTable[i][j] = distancesFromNodeI[transitNodes[j]];
        }
    }

    cout << "\rComputed the transit nodes distance table." << endl;

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
        if (i % 100 == 0) {
            cout << "\rComputed access nodes for '" << i << "' nodes.";
        }

        vector<unsigned int> forwardDistsFromNode(graph.nodes());
        BasicDijkstra::computeOneToAllDistances(i, originalGraph, forwardDistsFromNode);
        findForwardAccessNodes(i, forwardAccessNodes[i], forwardSearchSpaces[i], transitNodesMapping, forwardDistsFromNode, chGraph);
        vector<unsigned int> backwardDistsFromNode(graph.nodes());
        BasicDijkstra::computeOneToAllDistancesInReversedGraph(i, originalGraph, backwardDistsFromNode);
        findBackwardAccessNodes(i, backwardAccessNodes[i], backwardSearchSpaces[i], transitNodesMapping, backwardDistsFromNode, chGraph);
    }

    cout << "\rComputed acess nodes for all the nodes in the graph." << endl;

    vector < pair < unsigned int, QueryEdge > > allEdges;
    chGraph.getEdgesForFlushing(allEdges);

    outputGraph(outputPath, graph, allEdges, transitNodes, transitNodesDistanceTable, forwardAccessNodes, backwardAccessNodes, forwardSearchSpaces, backwardSearchSpaces, transitNodesAmount);
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::preprocessWithDMvalidation(UpdateableGraph & graph, Graph & originalGraph, string outputPath, unsigned int transitNodesAmount) {
    cout << "Getting transit nodes" << endl;
    vector < unsigned int > transitNodes(transitNodesAmount);
    graph.getNodesWithHighestRank(transitNodes, transitNodesAmount);

    FlagsGraph chGraph(graph);

    DistanceMatrix * distanceMatrix;
    {
        DistanceMatrixComputorSlow dmComputor;
        dmComputor.computeDistanceMatrix(originalGraph);
        distanceMatrix = dmComputor.getDistanceMatrixInstance();
    }

    cout << "Computing transit nodes distance table" << endl;
    vector < vector < unsigned int > > transitNodesDistanceTable(transitNodesAmount, vector < unsigned int > (transitNodesAmount));
    for(unsigned int i = 0; i < transitNodesAmount; i++) {
        for(unsigned int j = 0; j < transitNodesAmount; j++) {
            transitNodesDistanceTable[i][j] = distanceMatrix->findDistance(transitNodes[i], transitNodes[j]);
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
        if (i % 100 == 0) {
            cout << "\rComputed forward access nodes for '" << i << "' nodes.";
        }

        findForwardAccessNodes(i, forwardAccessNodes[i], forwardSearchSpaces[i], transitNodesMapping, chGraph, *distanceMatrix);
    }

    cout << "\rComputed forward acess nodes for all the nodes in the graph." << endl;

    {
        delete distanceMatrix;
        DistanceMatrixComputorSlow dmComputor;
        dmComputor.computeDistanceMatrixInReversedGraph(originalGraph);
        distanceMatrix = dmComputor.getDistanceMatrixInstance();
    }
    for(unsigned int i = 0; i < graph.nodes(); i++) {
        if (i % 100 == 0) {
            cout << "\rComputed backward access nodes for '" << i << "' nodes.";
        }

        findBackwardAccessNodes(i, backwardAccessNodes[i], backwardSearchSpaces[i], transitNodesMapping, chGraph, *distanceMatrix);
    }

    cout << "\rComputed backward acess nodes for all the nodes in the graph." << endl;

    delete distanceMatrix;

    vector < pair < unsigned int, QueryEdge > > allEdges;
    chGraph.getEdgesForFlushing(allEdges);

    outputGraph(outputPath, graph, allEdges, transitNodes, transitNodesDistanceTable, forwardAccessNodes, backwardAccessNodes, forwardSearchSpaces, backwardSearchSpaces, transitNodesAmount);
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::outputGraph(string outputPath, UpdateableGraph & graph, vector < pair < unsigned int, QueryEdge > > & allEdges, vector < unsigned int > & transitNodes, vector < vector < unsigned int > > & transitNodesDistanceTable, vector < vector < AccessNodeData > > & forwardAccessNodes, vector < vector < AccessNodeData > > & backwardAccessNodes, vector < vector < unsigned int > > & forwardSearchSpaces, vector < vector < unsigned int > > & backwardSearchSpaces, unsigned int transitNodesAmount) {
    cout << "Outputting TNR" << endl;
    ofstream output;
    output.open ( outputPath + ".tnrg", ios::binary );
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (outputPath + ".tnrg").c_str());
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
    unsigned int edges = (unsigned int) allEdges.size();
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
        unsigned int fwSize = (unsigned int) forwardAccessNodes[i].size();
        output.write((char *) &fwSize, sizeof(fwSize));
        for(unsigned int j = 0; j < forwardAccessNodes[i].size(); j++) {
            output.write((char *) &forwardAccessNodes[i][j].accessNodeID, sizeof(forwardAccessNodes[i][j].accessNodeID));
            output.write((char *) &forwardAccessNodes[i][j].distanceToNode, sizeof(forwardAccessNodes[i][j].distanceToNode));
        }
        unsigned int bwSize = (unsigned int) backwardAccessNodes[i].size();
        output.write((char *) &bwSize, sizeof(bwSize));
        for(unsigned int j = 0; j < backwardAccessNodes[i].size(); j++) {
            output.write((char *) &backwardAccessNodes[i][j].accessNodeID, sizeof(backwardAccessNodes[i][j].accessNodeID));
            output.write((char *) &backwardAccessNodes[i][j].distanceToNode, sizeof(backwardAccessNodes[i][j].distanceToNode));
        }
    }

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

    output.close();
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findForwardAccessNodes(unsigned int source, vector < AccessNodeData > & accessNodes, vector < unsigned int > & forwardSearchSpace, unordered_map< unsigned int, unsigned int > & transitNodes, FlagsGraph & graph) {
    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> forwardQ(cmp);
    vector<unsigned int> distances(graph.nodes(), UINT_MAX);
    vector<bool> settled(graph.nodes(), false);

    forwardQ.push(DijkstraNode(source, 0));

    distances[source] = 0;

    while (! forwardQ.empty() ) {
        unsigned int curNode = forwardQ.top().ID;
        unsigned int curLen = forwardQ.top().weight;
        forwardQ.pop();

        if(settled[curNode]) {
            continue;
        }

        settled[curNode] = true;
        if(transitNodes.count(curNode) == 1) {
            accessNodes.push_back(AccessNodeData(curNode, curLen));
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
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findForwardAccessNodes(unsigned int source, vector < AccessNodeData> & accessNodes, vector < unsigned int > & forwardSearchSpace, unordered_map< unsigned int, unsigned int > & transitNodes, vector < unsigned int > & distsFromNode, FlagsGraph & graph) {
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

    for(unsigned int i = 0; i < accessNodesSuperset.size(); i++) {
        unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
        unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
        unsigned int realDistance = distsFromNode[accessNode];
        if (realDistance == accessNodeDistance) {
            accessNodes.push_back(AccessNodeData(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode));
        }
    }
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findForwardAccessNodes(unsigned int source, vector < AccessNodeData > & accessNodes, vector < unsigned int > & forwardSearchSpace, unordered_map< unsigned int, unsigned int > & transitNodes, FlagsGraph & graph, DistanceMatrix & dm) {
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

    for(unsigned int i = 0; i < accessNodesSuperset.size(); i++) {
        unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
        unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
        unsigned int realDistance = dm.findDistance(source, accessNode);
        if (realDistance == accessNodeDistance) {
            accessNodes.push_back(AccessNodeData(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode));
        }
    }

}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findBackwardAccessNodes(unsigned int source, vector < AccessNodeData > & accessNodes, vector < unsigned int > & backwardSearchSpace, unordered_map< unsigned int, unsigned int > & transitNodes, FlagsGraph & graph) {
    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> backwardQ(cmp);
    vector<unsigned int> distances(graph.nodes(), UINT_MAX);
    vector<bool> settled(graph.nodes(), false);

    backwardQ.push(DijkstraNode(source, 0));

    distances[source] = 0;

    while (! backwardQ.empty() ) {
        unsigned int curNode = backwardQ.top().ID;
        unsigned int curLen = backwardQ.top().weight;
        backwardQ.pop();

        if(settled[curNode]) {
            continue;
        }

        settled[curNode] = true;
        if(transitNodes.count(curNode) == 1) {
            accessNodes.push_back(AccessNodeData(curNode, curLen));
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
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findBackwardAccessNodes(unsigned int source, vector < AccessNodeData> & accessNodes, vector < unsigned int > & backwardSearchSpace, unordered_map< unsigned int, unsigned int > & transitNodes, vector < unsigned int > & distsFromNode, FlagsGraph & graph) {
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


    for(unsigned int i = 0; i < accessNodesSuperset.size(); i++) {
        unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
        unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
        unsigned int realDistance = distsFromNode[accessNode];
        if (realDistance == accessNodeDistance) {
            accessNodes.push_back(AccessNodeData(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode));
        }
    }
}

//______________________________________________________________________________________________________________________
void TNRPreprocessor::findBackwardAccessNodes(unsigned int source, vector < AccessNodeData > & accessNodes, vector < unsigned int > & backwardSearchSpace, unordered_map< unsigned int, unsigned int > & transitNodes, FlagsGraph & graph, DistanceMatrix & dm) {
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


    for(unsigned int i = 0; i < accessNodesSuperset.size(); i++) {
        unsigned int accessNode = accessNodesSuperset[i].accessNodeID;
        unsigned int accessNodeDistance = accessNodesSuperset[i].distanceToNode;
        unsigned int realDistance = dm.findDistance(source, accessNode);
        if (realDistance == accessNodeDistance) {
            accessNodes.push_back(AccessNodeData(accessNodesSuperset[i].accessNodeID, accessNodesSuperset[i].distanceToNode));
        }
    }

}
