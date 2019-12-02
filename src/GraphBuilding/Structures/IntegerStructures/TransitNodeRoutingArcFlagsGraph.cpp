//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#include <climits>
#include "TransitNodeRoutingArcFlagsGraph.h"
#include "../../../TNRAF/Structures/AccessNodeDataArcFlags.h"

TransitNodeRoutingArcFlagsGraph::TransitNodeRoutingArcFlagsGraph(unsigned int nodes, unsigned int transitNodesAmount) : TransitNodeRoutingGraph(nodes, transitNodesAmount) {

}

IntegerNodeDataRegions & TransitNodeRoutingArcFlagsGraph::data(unsigned int node) {
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
unsigned int TransitNodeRoutingArcFlagsGraph::findTNRAFDistance(unsigned int source, unsigned int target) {
    unsigned int shortestDistance = UINT_MAX;
    unsigned int sourceRegion = regions[source];
    unsigned int targetRegion = regions[target];

    //printf("Finding shortest distance between '%u' (rank: %u, %lu access nodes) and '%u' (rank: %u, %lu access nodes) via TNR.\n", source, data(source).rank, forwardAccessNodes[source].size(), target, data(target).rank, backwardAccessNodes[target].size());
    for(unsigned int i = 0; i < forwardAccessNodes[source].size(); i++) {
        if (forwardAccessNodes[source][i].regionFlags[targetRegion]) {
            for (unsigned int j = 0; j < backwardAccessNodes[target].size(); j++) {
                if (backwardAccessNodes[target][j].regionFlags[sourceRegion]) {
                    unsigned int id1 = transitNodeMapping[forwardAccessNodes[source][i].accessNodeID];
                    unsigned int id2 = transitNodeMapping[backwardAccessNodes[target][j].accessNodeID];
                    unsigned int newDistance =
                            forwardAccessNodes[source][i].distanceToNode + transitNodesDistanceTable[id1][id2] +
                            backwardAccessNodes[target][j].distanceToNode;
                    //printf("Possible candidate is pair of transit nodes: %u -> %u, distance %u (%u, %u, %u)\n", forwardAccessNodes[source][i].accessNodeID, backwardAccessNodes[target][j].accessNodeID, newDistance, forwardAccessNodes[source][i].distanceToNode, transitNodesDistanceTable[id1][id2], backwardAccessNodes[target][j].distanceToNode);
                    if (newDistance < shortestDistance && transitNodesDistanceTable[id1][id2] != UINT_MAX) {
                        shortestDistance = newDistance;
                    }
                }
            }
        }
    }

    return shortestDistance;
}