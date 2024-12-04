/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#include <fstream>
#include <iostream>
#include "TGAFLoader.h"
#include "../Structures/TransitNodeRoutingArcFlagsGraph.h"
#include "../../Timer/Timer.h"
#include "../../TNRAF/TNRAFPreprocessor.h"

//______________________________________________________________________________________________________________________
TGAFLoader::TGAFLoader(std::string inputFile) : TNRGLoader(inputFile) {

}

//______________________________________________________________________________________________________________________
TransitNodeRoutingArcFlagsGraph* TGAFLoader::loadTNRAFforDistanceQueries() {
    std::ifstream input;
    input.open(this->inputFile, std::ios::binary);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes, edges, tnodesAmount, regionsCnt;
    parseFirstLine(input, nodes, edges, tnodesAmount, regionsCnt);

    TransitNodeRoutingArcFlagsGraph* graph = new TransitNodeRoutingArcFlagsGraph(nodes, tnodesAmount);
    parseEdgesForDistanceQueries(input, *graph, edges);
    parseRanks(input, *graph, nodes);
    parseRegions(input, *graph, nodes);
    parseTransitNodesMapping(input, *graph, tnodesAmount);
    parseTransitNodesDistanceTable(input, *graph, tnodesAmount);
    parseAccessNodes(input, *graph, nodes, regionsCnt);
    parseSearchSpaces(input, *graph, nodes);

    input.close();

    return graph;
}

//______________________________________________________________________________________________________________________
void TGAFLoader::parseFirstLine(std::ifstream& input, unsigned int& nodes, unsigned int& edges, unsigned int& tnodesAmount, unsigned int& regionsCnt) {
    char c1, c2, c3, c4;
    input.read (&c1, sizeof(c1));
    input.read (&c2, sizeof(c2));
    input.read (&c3, sizeof(c3));
    input.read (&c4, sizeof(c4));
    if (c1 != 'T' || c2 != 'G' || c3 != 'A' || c4 != 'F') {
        std::cout << "The input file is missing the expected header!" << std::endl
             << "Transit Node Routing Graph file should begin with the std::string 'TNRG'." << std::endl
             << "Are you sure the input file is in the correct format?" << std::endl
             << "The loading will proceed but the loaded graph might be corrupted." << std::endl;
    }

    input.read ((char *) &nodes, sizeof(nodes));
    input.read ((char *) &edges, sizeof(edges));
    input.read ((char *) &tnodesAmount, sizeof(tnodesAmount));
    input.read ((char *) &regionsCnt, sizeof(regionsCnt));
}

//______________________________________________________________________________________________________________________
void TGAFLoader::parseRanks(std::ifstream& input, TransitNodeRoutingArcFlagsGraph& graph, unsigned int nodes) {
    unsigned int rank;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char*) &rank, sizeof(rank));
        graph.data(i).rank = rank;
    }
}

//______________________________________________________________________________________________________________________
void TGAFLoader::parseRegions(std::ifstream& input, TransitNodeRoutingArcFlagsGraph& graph, unsigned int nodes) {
    unsigned int region;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char*) &region, sizeof(region));
        graph.data(i).region = region;
    }
}

//______________________________________________________________________________________________________________________
void TGAFLoader::parseAccessNodes(std::ifstream& input, TransitNodeRoutingArcFlagsGraph& graph, unsigned int nodes, unsigned int regionsCnt) {
    unsigned int forwardNodes, backwardNodes, nodeID, nodeDistance;
    uint32_t regionFlags;

    std::vector<unsigned int> powersOf2(regionsCnt);
    TNRAFPreprocessor::getPowersOf2(powersOf2);

    unsigned int validFlags = 0;
    unsigned int processedFlags = 0;
    unsigned int processedAcessNodes = 0;
    unsigned int noTrueFlags = 0;
    unsigned int forwardAccessNodesCnt = 0;
    unsigned int backwardAccessNodesCnt = 0;
    for(unsigned int i = 0; i < nodes; i++) {
        input.read ((char *) &forwardNodes, sizeof(forwardNodes));
        forwardAccessNodesCnt += forwardNodes;
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
        backwardAccessNodesCnt += backwardNodes;
        for(unsigned int j = 0; j < backwardNodes; j++) {
            input.read ((char *) &nodeID, sizeof(nodeID));
            input.read ((char *) &nodeDistance, sizeof(nodeDistance));
            input.read ((char *) &regionFlags, sizeof(regionFlags));
            graph.addBackwardAccessNode(i, nodeID, nodeDistance, regionsCnt, regionFlags, powersOf2);
        }
    }
}