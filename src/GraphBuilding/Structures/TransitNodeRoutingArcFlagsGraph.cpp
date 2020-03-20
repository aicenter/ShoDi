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

// Adds a forward access node to some node.
//______________________________________________________________________________________________________________________
void TransitNodeRoutingArcFlagsGraph::addForwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance, unsigned int regionsCnt, uint32_t regionsFlags, vector<unsigned int> & powersOf2) {
    forwardAccessNodes[node].push_back(AccessNodeDataArcFlags(accessNodeID, accessNodeDistance, regionsCnt, regionsFlags, powersOf2));
}

// Adds a backward access node to some node.
//______________________________________________________________________________________________________________________
void TransitNodeRoutingArcFlagsGraph::addBackwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance, unsigned int regionsCnt, uint32_t regionsFlags, vector<unsigned int> & powersOf2) {
    backwardAccessNodes[node].push_back(AccessNodeDataArcFlags(accessNodeID, accessNodeDistance, regionsCnt, regionsFlags, powersOf2));
}

// Returns the shortest distance from source to target using the Transit Node Routing with Arc Flags query algorithm.
// This should only be called when we know that the query is a global query. This function compares all the meaningful
// combinations of access nodes and then returns the smallest of the distances. This means that we consider all forward
// access nodes of source and all backward access nodes of target first. We obtain the region of source, let us call it
// reg_source, and the region of target, reg_target. We can then discard all forward access nodes of source for which
// the flag for reg_target is set to false, and analogically we can discard all backward access nodes of target for
// which the flag for reg_source is set to false. For the remaining access nodes, we check all combinations of a access
// node of source and b access node of target, we compute sums d(source, a) + d(a, b) + d(b, target) using the values
// precomputed in the structures, and then return the smallest result.
//______________________________________________________________________________________________________________________
unsigned int TransitNodeRoutingArcFlagsGraph::findTNRAFDistance(unsigned int source, unsigned int target) {
    unsigned int shortestDistance = UINT_MAX;
    unsigned int sourceRegion = nodesData[source].region;
    unsigned int targetRegion = nodesData[target].region;

    for(unsigned int i = 0; i < forwardAccessNodes[source].size(); i++) {
        if (forwardAccessNodes[source][i].regionFlags[targetRegion]) {
            for (unsigned int j = 0; j < backwardAccessNodes[target].size(); j++) {
                if (backwardAccessNodes[target][j].regionFlags[sourceRegion]) {
                    unsigned int id1 = transitNodeMapping[forwardAccessNodes[source][i].accessNodeID];
                    unsigned int id2 = transitNodeMapping[backwardAccessNodes[target][j].accessNodeID];
                    unsigned int newDistance =
                            forwardAccessNodes[source][i].distanceToNode + transitNodesDistanceTable[id1][id2] +
                            backwardAccessNodes[target][j].distanceToNode;
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