//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#include <climits>
#include <iostream>
#include "TransitNodeRoutingArcFlagsGraph.h"

//______________________________________________________________________________________________________________________
TransitNodeRoutingArcFlagsGraph::TransitNodeRoutingArcFlagsGraph(unsigned int nodes, unsigned int transitNodesAmount) : TransitNodeRoutingGraph(nodes, transitNodesAmount), forwardAccessNodes(nodes), backwardAccessNodes(nodes) {
}

//______________________________________________________________________________________________________________________
TransitNodeRoutingArcFlagsGraph::~TransitNodeRoutingArcFlagsGraph() = default;

//______________________________________________________________________________________________________________________
void TransitNodeRoutingArcFlagsGraph::addForwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance, unsigned int regionsCnt, uint32_t regionsFlags, std::vector<unsigned int> & powersOf2) {
    forwardAccessNodes[node].push_back(AccessNodeDataArcFlags(accessNodeID, accessNodeDistance, regionsCnt, regionsFlags, powersOf2));
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingArcFlagsGraph::addBackwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance, unsigned int regionsCnt, uint32_t regionsFlags, std::vector<unsigned int> & powersOf2) {
    backwardAccessNodes[node].push_back(AccessNodeDataArcFlags(accessNodeID, accessNodeDistance, regionsCnt, regionsFlags, powersOf2));
}

//______________________________________________________________________________________________________________________
unsigned int TransitNodeRoutingArcFlagsGraph::findTNRAFDistance(unsigned int start, unsigned int goal) {
    unsigned int shortestDistance = UINT_MAX;
    unsigned int sourceRegion = nodesData[start].region;
    unsigned int targetRegion = nodesData[goal].region;

    for(size_t i = 0; i < forwardAccessNodes[start].size(); i++) {
        if (forwardAccessNodes[start][i].regionFlags[targetRegion]) {
            for(size_t j = 0; j < backwardAccessNodes[goal].size(); j++) {
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

const std::vector<std::vector<AccessNodeDataArcFlags>> &
TransitNodeRoutingArcFlagsGraph::getForwardAccessNodes1() const {
    return forwardAccessNodes;
}

const std::vector<std::vector<AccessNodeDataArcFlags>> &
TransitNodeRoutingArcFlagsGraph::getBackwardAccessNodes1() const {
    return backwardAccessNodes;
}
