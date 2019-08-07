//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include <fstream>
#include <iostream>
#include "TNRGLoader.h"
#include "../../Timer/Timer.h"
#include "../Structures/IntegerStructures/IntegerGraph.h"

//______________________________________________________________________________________________________________________
TNRGLoader::TNRGLoader(string inputFile) : inputFile(inputFile) {

}

//______________________________________________________________________________________________________________________
TransitNodeRoutingGraph * TNRGLoader::loadTNR() {
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
    parseEdges(input, *graph, edges);
    parseRanks(input, *graph, nodes);
    parseTransitNodesMapping(input, *graph, tnodesAmount);
    parseTransitNodesDistanceTable(input, *graph, tnodesAmount);
    parseAccessNodes(input, *graph, nodes);
    parseLocalityFilter(input, *graph, nodes);

    graphLoadTimer.finish();
    graphLoadTimer.printMeasuredTime();

    input.close();

    return graph;
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseFirstLine(ifstream & input, unsigned int & nodes, unsigned int & edges, unsigned int & tnodesAmount) {
    char c1, c2, c3, c4;
    input.read (&c1, sizeof(c1));
    input.read (&c1, sizeof(c2));
    input.read (&c1, sizeof(c3));
    input.read (&c1, sizeof(c4));
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

//______________________________________________________________________________________________________________________
void TNRGLoader::parseEdges(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int edges) {
    unsigned int from, to, weight;
    bool forward, backward;

    for(unsigned int i = 0; i < edges; i++) {
        input.read ((char *) &from, sizeof(from));
        input.read ((char *) &to, sizeof(to));
        input.read ((char *) &weight, sizeof(weight));
        input.read ((char *) &forward, sizeof(forward));
        input.read ((char *) &backward, sizeof(backward));
        graph.addEdge(from, to, weight, forward, backward);
    }
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseRanks(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int nodes) {
    unsigned int rank;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char*) &rank, sizeof(rank));
        graph.data(i).rank = rank;
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
void TNRGLoader::parseLocalityFilter(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int nodes) {
    bool localQueryFlag;
    for(unsigned int i = 0; i < nodes; i++) {
        for(unsigned int j = 0; j < nodes; j++) {
            input.read ((char *) &localQueryFlag, sizeof(localQueryFlag));
            graph.setLocalityFilterValue(i, j, localQueryFlag);
        }
    }
}