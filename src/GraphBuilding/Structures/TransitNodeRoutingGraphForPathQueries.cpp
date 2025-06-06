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
// Created on: 15.10.19
//

#include "TransitNodeRoutingGraphForPathQueries.h"

//______________________________________________________________________________________________________________________
TransitNodeRoutingGraphForPathQueries::TransitNodeRoutingGraphForPathQueries(unsigned int nodes, unsigned int transitNodesAmount) : FlagsGraphWithUnpackingData(nodes), unpackingGraph(nodes), forwardAccessNodes(nodes), backwardAccessNodes(nodes), forwardSearchSpaces(nodes), backwardSearchSpaces(nodes), transitNodesDistanceTable(transitNodesAmount, std::vector<unsigned int>(transitNodesAmount)) {

}

//______________________________________________________________________________________________________________________
const std::vector < std::pair< unsigned int , unsigned int > > & TransitNodeRoutingGraphForPathQueries::unpackingNeighbours(unsigned int nodeID) {
    return unpackingGraph[nodeID];
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraphForPathQueries::addUnpackingEdge(unsigned int from, unsigned int to, unsigned int weight) {
    unpackingGraph[from].push_back(std::make_pair(to, weight));
}

// FROM TNRGraph
//______________________________________________________________________________________________________________________
bool TransitNodeRoutingGraphForPathQueries::isLocalQuery(unsigned int source, unsigned int target) {
    for(size_t k = 0; k < forwardSearchSpaces[source].size(); k++) {
        for(size_t m = 0; m < backwardSearchSpaces[target].size(); m++) {
            if(forwardSearchSpaces[source][k] == backwardSearchSpaces[target][m]) {
                return true;
            }
        }
    }
    return false;


    //return isLocal[source][target];
}

// Finds the distance between two nodes based on the TNR data-structure. This is used for the non-local queries.
// In that case, all pairs of access nodes of source and target are checked and the shortest distance from those
// pairs is returned.
//______________________________________________________________________________________________________________________
unsigned int TransitNodeRoutingGraphForPathQueries::findTNRDistance(unsigned int source, unsigned int target) {
    unsigned int shortestDistance = UINT_MAX;

    //printf("Finding shortest distance between '%u' (rank: %u, %lu access nodes) and '%u' (rank: %u, %lu access nodes) via TNR.\n", source, data(source).rank, forwardAccessNodes[source].size(), target, data(target).rank, backwardAccessNodes[target].size());
    for(size_t i = 0; i < forwardAccessNodes[source].size(); i++) {
        for(size_t j = 0; j < backwardAccessNodes[target].size(); j++) {
            unsigned int id1 = transitNodeMapping[forwardAccessNodes[source][i].accessNodeID];
            unsigned int id2 = transitNodeMapping[backwardAccessNodes[target][j].accessNodeID];
            unsigned int newDistance = forwardAccessNodes[source][i].distanceToNode + transitNodesDistanceTable[id1][id2] + backwardAccessNodes[target][j].distanceToNode;
            //printf("Possible candidate is pair of transit nodes: %u -> %u, distance %u (%u, %u, %u)\n", forwardAccessNodes[source][i].accessNodeID, backwardAccessNodes[target][j].accessNodeID, newDistance, forwardAccessNodes[source][i].distanceToNode, transitNodesDistanceTable[id1][id2], backwardAccessNodes[target][j].distanceToNode);
            if(newDistance < shortestDistance && transitNodesDistanceTable[id1][id2] != UINT_MAX) {
                shortestDistance = newDistance;
            }
        }
    }

    return shortestDistance;
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraphForPathQueries::addMappingPair(unsigned int realID, unsigned int transitNodesID) {
    transitNodeMapping.insert(std::make_pair(realID, transitNodesID));
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraphForPathQueries::setDistanceTableValue(unsigned int i, unsigned int j, unsigned int value) {
    transitNodesDistanceTable[i][j] = value;
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraphForPathQueries::addForwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance) {
    forwardAccessNodes[node].push_back(AccessNodeData(accessNodeID, accessNodeDistance));
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraphForPathQueries::addBackwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance) {
    backwardAccessNodes[node].push_back(AccessNodeData(accessNodeID, accessNodeDistance));
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraphForPathQueries::addForwardSearchSpaceNode(unsigned int sourceNode, unsigned int searchSpaceNode) {
    forwardSearchSpaces[sourceNode].push_back(searchSpaceNode);
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraphForPathQueries::addBackwardSearchSpaceNode(unsigned int sourceNode, unsigned int searchSpaceNode) {
    backwardSearchSpaces[sourceNode].push_back(searchSpaceNode);
}
