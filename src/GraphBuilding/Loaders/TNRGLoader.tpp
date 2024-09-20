//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include <fstream>
#include <iostream>
#include "../../Timer/Timer.h"
#include "../Structures/Graph.h"

//______________________________________________________________________________________________________________________
template<class T> TNRGLoader<T>::TNRGLoader(std::string inputFile) : inputFile(inputFile) {

}

//______________________________________________________________________________________________________________________
template<class T> TransitNodeRoutingGraph<NodeData>* TNRGLoader<T>::loadTNRforDistanceQueries() {
    std::ifstream input;
    input.open(this->inputFile, std::ios::binary);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes, edges, tnodesAmount;
    parseFirstLine(input, nodes, edges, tnodesAmount);

    TransitNodeRoutingGraph<NodeData>* graph = new TransitNodeRoutingGraph(nodes, tnodesAmount);
    parseEdgesForDistanceQueries(input, *graph, edges);
    parseRanks(input, *graph, nodes);
    parseTransitNodesMapping(input, *graph, tnodesAmount);
    parseTransitNodesDistanceTable(input, *graph, tnodesAmount);
    parseAccessNodes(input, *graph, nodes);
    parseSearchSpaces(input, *graph, nodes);

    input.close();

    return graph;
}

//______________________________________________________________________________________________________________________
template<class T> TransitNodeRoutingGraphForPathQueries * TNRGLoader<T>::loadTNRforPathQueries() {
    std::ifstream input;
    input.open(this->inputFile, std::ios::binary);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes, edges, tnodesAmount;
    parseFirstLine(input, nodes, edges, tnodesAmount);

    TransitNodeRoutingGraphForPathQueries * graph = new TransitNodeRoutingGraphForPathQueries(nodes, tnodesAmount);
    parseEdgesForPathQueries(input, *graph, edges);
    parseRanks(input, *graph, nodes);
    parseTransitNodesMapping(input, *graph, tnodesAmount);
    parseTransitNodesDistanceTable(input, *graph, tnodesAmount);
    parseAccessNodes(input, *graph, nodes);
    parseSearchSpaces(input, *graph, nodes);

    input.close();

    return graph;
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseFirstLine(std::ifstream & input, unsigned int & nodes, unsigned int & edges, unsigned int & tnodesAmount) {
    char c1, c2, c3, c4;
    input.read (&c1, sizeof(c1));
    input.read (&c2, sizeof(c2));
    input.read (&c3, sizeof(c3));
    input.read (&c4, sizeof(c4));
    if (c1 != 'T' || c2 != 'N' || c3 != 'R' || c4 != 'G') {
        std::cout << "The input file is missing the expected header!" << std::endl
             << "Transit Node Routing Graph file should begin with the std::string 'TNRG'." << std::endl
             << "Are you sure the input file is in the correct format?" << std::endl
             << "The loading will proceed but the loaded graph might be corrupted." << std::endl;
    }

    input.read ((char *) &nodes, sizeof(nodes));
    input.read ((char *) &edges, sizeof(edges));
    input.read ((char *) &tnodesAmount, sizeof(tnodesAmount));
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseEdgesForDistanceQueries(std::ifstream& input, T& graph, unsigned int edges) {
    unsigned int from, to, weight;
    bool forward, backward, fwShortcutFlag, bwShortcutFlag;

    for(unsigned int i = 0; i < edges; i++) {
        input.read ((char *) &from, sizeof(from));
        input.read ((char *) &to, sizeof(to));
        input.read ((char *) &weight, sizeof(weight));
        input.read ((char *) &forward, sizeof(forward));
        input.read ((char *) &backward, sizeof(backward));
        input.read ((char *) &fwShortcutFlag, sizeof(fwShortcutFlag));
        input.read ((char *) &bwShortcutFlag, sizeof(bwShortcutFlag));
        graph.addEdge(from, to, weight, forward, backward);
    }
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseEdgesForPathQueries(std::ifstream & input, TransitNodeRoutingGraphForPathQueries & graph, unsigned int edges) {
    unsigned int from, to, weight;
    bool forward, backward, fwShortcutFlag, bwShortcutFlag;

    for(unsigned int i = 0; i < edges; i++) {
        input.read ((char *) &from, sizeof(from));
        input.read ((char *) &to, sizeof(to));
        input.read ((char *) &weight, sizeof(weight));
        input.read ((char *) &forward, sizeof(forward));
        input.read ((char *) &backward, sizeof(backward));
        input.read ((char *) &fwShortcutFlag, sizeof(fwShortcutFlag));
        input.read ((char *) &bwShortcutFlag, sizeof(bwShortcutFlag));
        graph.addEdge(from, to, weight, forward, backward);
        if(forward && !fwShortcutFlag) {
            graph.addUnpackingEdge(from, to, weight);
        }
        if (backward && !bwShortcutFlag) {
            graph.addUnpackingEdge(to, from, weight);
        }
    }
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseRanks(std::ifstream& input, T& graph, unsigned int nodes) {
    unsigned int rank;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char*) &rank, sizeof(rank));
        graph.data(i).rank = rank;
    }
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseTransitNodesMapping(std::ifstream& input, T& graph, unsigned int tnodesAmount) {
    unsigned int originalID;
    for(unsigned int i = 0; i < tnodesAmount; i++) {
        input.read ((char*) &originalID, sizeof(originalID));
        graph.addMappingPair(originalID, i);
    }
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseTransitNodesDistanceTable(std::ifstream& input, T& graph, unsigned int tnodesAmount) {
    unsigned int value;
    for(unsigned int i = 0; i < tnodesAmount; i++) {
        for(unsigned int j = 0; j < tnodesAmount; j++) {
            input.read ((char*) &value, sizeof(value));
            graph.setDistanceTableValue(i, j, value);
        }
    }
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseAccessNodes(std::ifstream& input, T& graph, unsigned int nodes) {
    unsigned int forwardNodes, backwardNodes, nodeID, nodeDistance;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char *) &forwardNodes, sizeof(forwardNodes));
        for(unsigned int j = 0; j < forwardNodes; j++) {
            input.read ((char *) &nodeID, sizeof(nodeID));
            input.read ((char *) &nodeDistance, sizeof(nodeDistance));
            graph.addForwardAccessNode(i, nodeID, nodeDistance);
        }
        input.read ((char *) &backwardNodes, sizeof(backwardNodes));
        for(unsigned int j = 0; j < backwardNodes; j++) {
            input.read ((char *) &nodeID, sizeof(nodeID));
            input.read ((char *) &nodeDistance, sizeof(nodeDistance));
            graph.addBackwardAccessNode(i, nodeID, nodeDistance);
        }
    }
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseSearchSpaces(std::ifstream& input, T& graph, unsigned int nodes) {
    for(unsigned int i = 0; i < nodes; i++) {
        unsigned int fwSearchSpaceSize, bwSearchSpaceSize, searchSpaceNode;
        input.read((char *) &fwSearchSpaceSize, sizeof(fwSearchSpaceSize));
        for(unsigned int j = 0; j < fwSearchSpaceSize; j++) {
            input.read((char *) &searchSpaceNode, sizeof(searchSpaceNode));
            graph.addForwardSearchSpaceNode(i, searchSpaceNode);
        }

        input.read((char *) &bwSearchSpaceSize, sizeof(bwSearchSpaceSize));
        for(unsigned int j = 0; j < bwSearchSpaceSize; j++) {
            input.read((char *) &searchSpaceNode, sizeof(searchSpaceNode));
            graph.addBackwardSearchSpaceNode(i, searchSpaceNode);
        }
    }
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseRanks(std::ifstream& input, TransitNodeRoutingGraphForPathQueries& graph, unsigned int nodes) {
    unsigned int rank;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char*) &rank, sizeof(rank));
        graph.data(i).rank = rank;
    }
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseTransitNodesMapping(std::ifstream& input, TransitNodeRoutingGraphForPathQueries & graph, unsigned int tnodesAmount) {
    unsigned int originalID;
    for(unsigned int i = 0; i < tnodesAmount; i++) {
        input.read ((char*) &originalID, sizeof(originalID));
        graph.addMappingPair(originalID, i);
    }
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseTransitNodesDistanceTable(std::ifstream & input, TransitNodeRoutingGraphForPathQueries & graph, unsigned int tnodesAmount) {
    unsigned int value;
    for(unsigned int i = 0; i < tnodesAmount; i++) {
        for(unsigned int j = 0; j < tnodesAmount; j++) {
            input.read ((char*) &value, sizeof(value));
            graph.setDistanceTableValue(i, j, value);
        }
    }
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseAccessNodes(std::ifstream & input, TransitNodeRoutingGraphForPathQueries & graph, unsigned int nodes) {
    unsigned int forwardNodes, backwardNodes, nodeID, nodeDistance;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char *) &forwardNodes, sizeof(forwardNodes));
        for(unsigned int j = 0; j < forwardNodes; j++) {
            input.read ((char *) &nodeID, sizeof(nodeID));
            input.read ((char *) &nodeDistance, sizeof(nodeDistance));
            graph.addForwardAccessNode(i, nodeID, nodeDistance);
        }
        input.read ((char *) &backwardNodes, sizeof(backwardNodes));
        for(unsigned int j = 0; j < backwardNodes; j++) {
            input.read ((char *) &nodeID, sizeof(nodeID));
            input.read ((char *) &nodeDistance, sizeof(nodeDistance));
            graph.addBackwardAccessNode(i, nodeID, nodeDistance);
        }
    }
}

//______________________________________________________________________________________________________________________
template<class T> void TNRGLoader<T>::parseSearchSpaces(std::ifstream & input, TransitNodeRoutingGraphForPathQueries & graph, unsigned int nodes) {
    for(unsigned int i = 0; i < nodes; i++) {
        unsigned int fwSearchSpaceSize, bwSearchSpaceSize, searchSpaceNode;
        input.read((char *) &fwSearchSpaceSize, sizeof(fwSearchSpaceSize));
        for(unsigned int j = 0; j < fwSearchSpaceSize; j++) {
            input.read((char *) &searchSpaceNode, sizeof(searchSpaceNode));
            graph.addForwardSearchSpaceNode(i, searchSpaceNode);
        }

        input.read((char *) &bwSearchSpaceSize, sizeof(bwSearchSpaceSize));
        for(unsigned int j = 0; j < bwSearchSpaceSize; j++) {
            input.read((char *) &searchSpaceNode, sizeof(searchSpaceNode));
            graph.addBackwardSearchSpaceNode(i, searchSpaceNode);
        }
    }
}
