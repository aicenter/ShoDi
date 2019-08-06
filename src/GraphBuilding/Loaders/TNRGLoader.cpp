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
    input.open(this->inputFile);
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
    input >> c1 >> c2 >> c3 >> c4;
    if (c1 != 'T' || c2 != 'N' || c3 != 'R' || c4 != 'G') {
        cout << "The input file is missing the expected header!" << endl
             << "Transit Node Routing Graph file should begin with the string 'TNRG'." << endl
             << "Are you sure the input file is in the correct format?" << endl
             << "The loading will proceed but the loaded graph might be corrupted." << endl;
    }

    input >> nodes >> edges >> tnodesAmount;
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseEdges(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int edges) {
    unsigned int from, to, weight;
    bool forward, backward;
    for(unsigned int i = 0; i < edges; i++) {
        input >> from >> to >> weight >> forward >> backward;
        graph.addEdge(from, to, weight, forward, backward);
    }
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseRanks(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int nodes) {
    unsigned int rank;
    for(unsigned int i = 0; i < nodes; i++) {
        input >> rank;
        graph.data(i).rank = rank;
    }
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseTransitNodesMapping(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int tnodesAmount) {
    unsigned int originalID;
    for(unsigned int i = 0; i < tnodesAmount; i++) {
        input >> originalID;
        graph.addMappingPair(originalID, i);
    }
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseTransitNodesDistanceTable(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int tnodesAmount) {
    unsigned int value;
    for(unsigned int i = 0; i < tnodesAmount; i++) {
        for(unsigned int j = 0; j < tnodesAmount; j++) {
            input >> value;
            graph.setDistanceTableValue(i, j, value);
        }
    }
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseAccessNodes(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int nodes) {
    unsigned int forwardNodes, backwardNodes, nodeID, nodeDistance;
    for(unsigned int i = 0; i < nodes; i++) {
        input >> forwardNodes;
        for(unsigned int j = 0; j < forwardNodes; j++) {
            input >> nodeID >> nodeDistance;
            graph.addForwardAccessNode(i, nodeID, nodeDistance);
        }
        input >> backwardNodes;
        for(unsigned int j = 0; j < backwardNodes; j++) {
            input >> nodeID >> nodeDistance;
            graph.addBackwardAccessNode(i, nodeID, nodeDistance);
        }
    }
}

//______________________________________________________________________________________________________________________
void TNRGLoader::parseLocalityFilter(ifstream & input, TransitNodeRoutingGraph & graph, unsigned int nodes) {
    bool localQueryFlag;
    for(unsigned int i = 0; i < nodes; i++) {
        for(unsigned int j = 0; j < nodes; j++) {
            input >> localQueryFlag;
            graph.setLocalityFilterValue(i, j, localQueryFlag);
        }
    }
}