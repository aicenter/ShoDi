//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#include <climits>
#include <iostream>
#include "TransitNodeRoutingArcFlagsGraph.h"
#include "../../../TNRAF/Structures/AccessNodeDataArcFlags.h"

//______________________________________________________________________________________________________________________
TransitNodeRoutingArcFlagsGraph::TransitNodeRoutingArcFlagsGraph(unsigned int nodes, unsigned int transitNodesAmount) : TransitNodeRoutingGraph(nodes, transitNodesAmount), forwardAccessNodes(nodes), backwardAccessNodes(nodes), nodesData(nodes) {

}

//______________________________________________________________________________________________________________________
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
    unsigned int sourceRegion = nodesData[source].region;
    unsigned int targetRegion = nodesData[target].region;

    //printf("Finding shortest distance between '%u' (rank: %u, %lu access nodes) and '%u' (rank: %u, %lu access nodes) via TNR.\n", source, data(source).rank, forwardAccessNodes[source].size(), target, data(target).rank, backwardAccessNodes[target].size());
    for(unsigned int i = 0; i < forwardAccessNodes[source].size(); i++) {

        // FIXME only debug print
        /*printf("Access node %u - ID: %u, distance: %u, region flag to source: ", i,
                forwardAccessNodes[source][i].accessNodeID,
                forwardAccessNodes[source][i].distanceToNode);
        cout << forwardAccessNodes[source][i].regionFlags[sourceRegion] << endl << "Region flags:";
        for(unsigned int q = 0; q < forwardAccessNodes[source][i].regionFlags.size(); q++) {
            cout << " " << forwardAccessNodes[source][i].regionFlags[q];
        }
        printf("\n");*/
        // end of FIXME

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

// FIXME: Trying this for debug purposes.

//______________________________________________________________________________________________________________________
void TransitNodeRoutingArcFlagsGraph::resetForwardInfo(const unsigned int node) {
    //printf("TransitNodeRoutingArcFlagsGraph resetForwardInfo was called.\n");
    nodesData[node].forwardDist = ULLONG_MAX;
    nodesData[node].forwardSettled = false;
    nodesData[node].forwardReached = false;
}


//______________________________________________________________________________________________________________________
void TransitNodeRoutingArcFlagsGraph::resetBackwardInfo(const unsigned int node) {
    //printf("TransitNodeRoutingArcFlagsGraph resetBackwardInfo was called.\n");
    nodesData[node].backwardDist = ULLONG_MAX;
    nodesData[node].backwardSettled = false;
    nodesData[node].backwardReached = false;
}