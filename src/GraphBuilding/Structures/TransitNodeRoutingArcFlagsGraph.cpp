//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#include <climits>
#include <iostream>
#include "TransitNodeRoutingArcFlagsGraph.h"
#include "../../TNRAF/Structures/AccessNodeDataArcFlags.h"

//______________________________________________________________________________________________________________________
TransitNodeRoutingArcFlagsGraph::TransitNodeRoutingArcFlagsGraph(unsigned int nodes, unsigned int transitNodesAmount) : TransitNodeRoutingGraph(nodes, transitNodesAmount), forwardAccessNodes(nodes), backwardAccessNodes(nodes), nodesData(nodes) {

}

//______________________________________________________________________________________________________________________
NodeDataRegions & TransitNodeRoutingArcFlagsGraph::data(unsigned int node) {
    return nodesData[node];
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingArcFlagsGraph::addForwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance, unsigned int regionsCnt, uint32_t regionsFlags, vector<unsigned int> & powersOf2) {
    forwardAccessNodes[node].push_back(AccessNodeDataArcFlags(accessNodeID, accessNodeDistance, regionsCnt, regionsFlags, powersOf2));
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingArcFlagsGraph::addBackwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance, unsigned int regionsCnt, uint32_t regionsFlags, vector<unsigned int> & powersOf2) {
    backwardAccessNodes[node].push_back(AccessNodeDataArcFlags(accessNodeID, accessNodeDistance, regionsCnt, regionsFlags, powersOf2));
}

//______________________________________________________________________________________________________________________
unsigned int TransitNodeRoutingArcFlagsGraph::findTNRAFDistance(unsigned int start, unsigned int goal) {
    unsigned int shortestDistance = UINT_MAX;
    unsigned int sourceRegion = nodesData[start].region;
    unsigned int targetRegion = nodesData[goal].region;

    for(unsigned int i = 0; i < forwardAccessNodes[start].size(); i++) {
        if (forwardAccessNodes[start][i].regionFlags[targetRegion]) {
            for (unsigned int j = 0; j < backwardAccessNodes[goal].size(); j++) {
                if (backwardAccessNodes[goal][j].regionFlags[sourceRegion]) {
                    unsigned int id1 = transitNodeMapping[forwardAccessNodes[start][i].accessNodeID];
                    unsigned int id2 = transitNodeMapping[backwardAccessNodes[goal][j].accessNodeID];
                    unsigned int newDistance =
                            forwardAccessNodes[start][i].distanceToNode + transitNodesDistanceTable[id1][id2] +
                            backwardAccessNodes[goal][j].distanceToNode;
                    if (newDistance < shortestDistance && transitNodesDistanceTable[id1][id2] != UINT_MAX) {
                        shortestDistance = newDistance;
                    }
                }
            }
        }
    }

    return shortestDistance;
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingArcFlagsGraph::resetForwardInfo(const unsigned int node) {
    nodesData[node].forwardDist = UINT_MAX;
    nodesData[node].forwardSettled = false;
    nodesData[node].forwardReached = false;
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingArcFlagsGraph::resetBackwardInfo(const unsigned int node) {
    nodesData[node].backwardDist = UINT_MAX;
    nodesData[node].backwardSettled = false;
    nodesData[node].backwardReached = false;
}