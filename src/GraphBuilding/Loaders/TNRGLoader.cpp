//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include <fstream>
#include <iostream>
#include "TNRGLoader.h"
#include "../../Timer/Timer.h"
#include "../Structures/Graph.h"

//______________________________________________________________________________________________________________________
TNRGLoader::TNRGLoader(string inputFile) : inputFile(inputFile) {

}

// Can be used to load Transit Node Routing data structures from a file given to the loader during its initialization.
// This function provides an initialized TransitNodeRoutingGraph instance as output, which can be used to answer
// distance queries (queries, where we only care about the shortest path between two points and not about the actual
// shortest path).
//______________________________________________________________________________________________________________________
TransitNodeRoutingGraph * TNRGLoader::loadTNRforDistanceQueries() {
    ifstream input;
    input.open(this->inputFile, ios::binary);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    printf("Started loading graph!\n");

    Timer graphLoadTimer("TNR Graph loading");
    graphLoadTimer.begin();

    unsigned int nodes, edges, tnodesAmount;
    parseFirstLine(input, nodes, edges, tnodesAmount);

    TransitNodeRoutingGraph * graph = new TransitNodeRoutingGraph(nodes, tnodesAmount);
    parseEdgesForDistanceQueries(input, *graph, edges);
    parseRanks(input, graph, nodes);
    parseTransitNodesMapping(input, *graph, tnodesAmount);
    parseTransitNodesDistanceTable(input, *graph, tnodesAmount);
    parseAccessNodes(input, *graph, nodes);
    parseSearchSpaces(input, *graph, nodes);

    graphLoadTimer.finish();
    graphLoadTimer.printMeasuredTime();

    input.close();

    return graph;
}

//______________________________________________________________________________________________________________________
TransitNodeRoutingGraphForPathQueries * TNRGLoader::loadTNRforPathQueries() {
    ifstream input;
    input.open(this->inputFile, ios::binary);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    printf("Started loading graph!\n");

    Timer graphLoadTimer("TNR Graph loading");
    graphLoadTimer.begin();

    unsigned int nodes, edges, tnodesAmount;
    parseFirstLine(input, nodes, edges, tnodesAmount);

    TransitNodeRoutingGraphForPathQueries * graph = new TransitNodeRoutingGraphForPathQueries(nodes, tnodesAmount);
    parseEdgesForPathQueries(input, *graph, edges);
    parseRanks(input, *graph, nodes);
    parseTransitNodesMapping(input, *graph, tnodesAmount);
    parseTransitNodesDistanceTable(input, *graph, tnodesAmount);
    parseAccessNodes(input, *graph, nodes);
    parseSearchSpaces(input, *graph, nodes);

    graphLoadTimer.finish();
    graphLoadTimer.printMeasuredTime();

    input.close();

    return graph;
}

// Parses the header. The file should start with the string "TNRG" which is used as some sort of a magic number to check
// the integrity of the file. Then three unsigned ints should follow denoting the number of nodes, the number of edges
// and the size of the transit node set.
//______________________________________________________________________________________________________________________
void TNRGLoader::parseFirstLine(ifstream & input, unsigned int & nodes, unsigned int & edges, unsigned int & tnodesAmount) {
    char c1, c2, c3, c4;
    input.read (&c1, sizeof(c1));
    input.read (&c2, sizeof(c2));
    input.read (&c3, sizeof(c3));
    input.read (&c4, sizeof(c4));
    if (c1 != 'T' || c2 != 'N' || c3 != 'R' || c4 != 'G') {
        cout << "The input file is missing the expected header!" << endl
             << "Transit Node Routing Graph file should begin with the string 'TNRG'." << endl
             << "Are you sure the input file is in the correct format?" << endl
             << "The loading will proceed but the loaded graph might be corrupted." << endl;
    }

    input.read ((char *) &nodes, sizeof(nodes));
    input.read ((char *) &edges, sizeof(edges));
    input.read ((char *) &tnodesAmount, sizeof(tnodesAmount));
}

// Processes edges.
//______________________________________________________________________________________________________________________
void TNRGLoader::parseEdgesForDistanceQueries(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int edges) {
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

// Processes edges.
//______________________________________________________________________________________________________________________
void TNRGLoader::parseEdgesForPathQueries(ifstream & input, TransitNodeRoutingGraphForPathQueries & graph, unsigned int edges) {
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
        graph.addEdge(from, to, weight, forward, backward); // FIXME - here middle node would have to be added in order to have faster path queries.
        if(forward && !fwShortcutFlag) {
            graph.addUnpackingEdge(from, to, weight);
        }
        if (backward && !bwShortcutFlag) {
            graph.addUnpackingEdge(to, from, weight);
        }
    }
}

// Gets ranks for all the nodes in the graph.
//______________________________________________________________________________________________________________________
void TNRGLoader::parseRanks(ifstream & input, TransitNodeRoutingGraph * graph, unsigned int nodes) {
    unsigned int rank;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char*) &rank, sizeof(rank));
        graph->data(i).rank = rank;
    }
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseTransitNodesMapping(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int tnodesAmount) {
    unsigned int originalID;
    for(unsigned int i = 0; i < tnodesAmount; i++) {
        input.read ((char*) &originalID, sizeof(originalID));
        graph.addMappingPair(originalID, i);
    }
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseTransitNodesDistanceTable(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int tnodesAmount) {
    unsigned int value;
    for(unsigned int i = 0; i < tnodesAmount; i++) {
        for(unsigned int j = 0; j < tnodesAmount; j++) {
            input.read ((char*) &value, sizeof(value));
            graph.setDistanceTableValue(i, j, value);
        }
    }
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseAccessNodes(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int nodes) {
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
void TNRGLoader::parseSearchSpaces(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int nodes) {
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
void TNRGLoader::parseRanks(ifstream & input, TransitNodeRoutingGraphForPathQueries & graph, unsigned int nodes) {
    unsigned int rank;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char*) &rank, sizeof(rank));
        graph.data(i).rank = rank;
    }
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseTransitNodesMapping(ifstream & input, TransitNodeRoutingGraphForPathQueries & graph, unsigned int tnodesAmount) {
    unsigned int originalID;
    for(unsigned int i = 0; i < tnodesAmount; i++) {
        input.read ((char*) &originalID, sizeof(originalID));
        graph.addMappingPair(originalID, i);
    }
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseTransitNodesDistanceTable(ifstream & input, TransitNodeRoutingGraphForPathQueries & graph, unsigned int tnodesAmount) {
    unsigned int value;
    for(unsigned int i = 0; i < tnodesAmount; i++) {
        for(unsigned int j = 0; j < tnodesAmount; j++) {
            input.read ((char*) &value, sizeof(value));
            graph.setDistanceTableValue(i, j, value);
        }
    }
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseAccessNodes(ifstream & input, TransitNodeRoutingGraphForPathQueries & graph, unsigned int nodes) {
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
void TNRGLoader::parseSearchSpaces(ifstream & input, TransitNodeRoutingGraphForPathQueries & graph, unsigned int nodes) {
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
