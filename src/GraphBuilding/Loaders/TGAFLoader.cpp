//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#include <fstream>
#include <iostream>
#include "TGAFLoader.h"
#include "../Structures/IntegerStructures/TransitNodeRoutingArcFlagsGraph.h"
#include "../../Timer/Timer.h"
#include "../../TNRAF/TNRAFPreprocessor.h"

//______________________________________________________________________________________________________________________
TGAFLoader::TGAFLoader(string inputFile) : TNRGLoader(inputFile) {

}

//______________________________________________________________________________________________________________________
TransitNodeRoutingArcFlagsGraph * TGAFLoader::loadTNRAFforDistanceQueries() {
    ifstream input;
    input.open(this->inputFile, ios::binary);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    printf("Started loading graph!\n");

    Timer graphLoadTimer("TNR AF Graph loading");
    graphLoadTimer.begin();

    unsigned int nodes, edges, tnodesAmount, regionsCnt;
    parseFirstLine(input, nodes, edges, tnodesAmount, regionsCnt);

    TransitNodeRoutingArcFlagsGraph * graph = new TransitNodeRoutingArcFlagsGraph(nodes, tnodesAmount);
    parseEdgesForDistanceQueries(input, *graph, edges);
    printf("Parsed edges.\n");
    parseRanks(input, graph, nodes);
    printf("Parsed ranks.\n");
    parseRegions(input, *graph, nodes);
    printf("Parsed regions.\n");
    parseTransitNodesMapping(input, *graph, tnodesAmount);
    printf("Parsed transit nodes mapping.\n");
    parseTransitNodesDistanceTable(input, *graph, tnodesAmount);
    printf("Parsed distance table.\n");
    parseAccessNodes(input, *graph, nodes, regionsCnt);
    printf("Parsed access nodes.\n");
    parseSearchSpaces(input, *graph, nodes);
    printf("Parsed search spaces.\n");

    graphLoadTimer.finish();
    graphLoadTimer.printMeasuredTime();

    input.close();

    return graph;
}

//______________________________________________________________________________________________________________________
void TGAFLoader::parseFirstLine(ifstream & input, unsigned int & nodes, unsigned int & edges, unsigned int & tnodesAmount, unsigned int & regionsCnt) {
    char c1, c2, c3, c4;
    input.read (&c1, sizeof(c1));
    input.read (&c2, sizeof(c2));
    input.read (&c3, sizeof(c3));
    input.read (&c4, sizeof(c4));
    if (c1 != 'T' || c2 != 'G' || c3 != 'A' || c4 != 'F') {
        cout << "The input file is missing the expected header!" << endl
             << "Transit Node Routing Graph file should begin with the string 'TNRG'." << endl
             << "Are you sure the input file is in the correct format?" << endl
             << "The loading will proceed but the loaded graph might be corrupted." << endl;
    }

    input.read ((char *) &nodes, sizeof(nodes));
    input.read ((char *) &edges, sizeof(edges));
    input.read ((char *) &tnodesAmount, sizeof(tnodesAmount));
    input.read ((char *) &regionsCnt, sizeof(regionsCnt));
}

//______________________________________________________________________________________________________________________
void TGAFLoader::parseRanks(ifstream & input, TransitNodeRoutingArcFlagsGraph * graph, unsigned int nodes) {
    unsigned int rank;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char*) &rank, sizeof(rank));
        graph->data(i).rank = rank;
    }
}

//______________________________________________________________________________________________________________________
void TGAFLoader::parseRegions(ifstream & input, TransitNodeRoutingArcFlagsGraph & graph, unsigned int nodes) {
    unsigned int region;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char*) &region, sizeof(region));
        graph.data(i).region = region;
    }
}

//______________________________________________________________________________________________________________________
void TGAFLoader::parseAccessNodes(ifstream & input, TransitNodeRoutingArcFlagsGraph & graph, unsigned int nodes, unsigned int regionsCnt) {
    unsigned int forwardNodes, backwardNodes, nodeID, nodeDistance;
    uint32_t regionFlags;

    vector<unsigned int> powersOf2(regionsCnt);
    TNRAFPreprocessor::getPowersOf2(powersOf2);

    unsigned int validFlags = 0;
    unsigned int processedFlags = 0;
    unsigned int processedAcessNodes = 0;
    unsigned int noTrueFlags = 0;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char *) &forwardNodes, sizeof(forwardNodes));
        for(unsigned int j = 0; j < forwardNodes; j++) {
            input.read ((char *) &nodeID, sizeof(nodeID));
            input.read ((char *) &nodeDistance, sizeof(nodeDistance));
            input.read ((char *) &regionFlags, sizeof(regionFlags));
            graph.addForwardAccessNode(i, nodeID, nodeDistance, regionsCnt, regionFlags, powersOf2);
            processedAcessNodes++;
            processedFlags += regionsCnt;
            if(regionFlags == 0) {
                noTrueFlags++;
            }
            for(unsigned int q = 0; q < regionsCnt; q++) {
                if((regionFlags & powersOf2[q]) == powersOf2[q]) {
                    validFlags++;
                }
            }
        }
        input.read ((char *) &backwardNodes, sizeof(backwardNodes));
        for(unsigned int j = 0; j < backwardNodes; j++) {
            input.read ((char *) &nodeID, sizeof(nodeID));
            input.read ((char *) &nodeDistance, sizeof(nodeDistance));
            input.read ((char *) &regionFlags, sizeof(regionFlags));
            graph.addBackwardAccessNode(i, nodeID, nodeDistance, regionsCnt, regionFlags, powersOf2);
        }
    }
    printf("%u flags out of %u were True. That is %lf %%.\n", validFlags, processedFlags, ((double) validFlags / processedFlags) * 100);
    printf("%u acess nodes out of %u had no True region flags. That is %lf %%.\n", noTrueFlags, processedAcessNodes, ((double) noTrueFlags / processedAcessNodes) * 100);
}