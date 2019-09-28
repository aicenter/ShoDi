//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include <climits>
#include "TransitNodeRoutingGraph.h"

//______________________________________________________________________________________________________________________
TransitNodeRoutingGraph::TransitNodeRoutingGraph(unsigned int nodes, unsigned int transitNodesAmount) : IntegerFlagsGraph(nodes), forwardAccessNodes(nodes), backwardAccessNodes(nodes), transitNodesDistanceTable(transitNodesAmount, vector<unsigned int>(transitNodesAmount)), isLocal(nodes, vector<bool>(nodes)) {

}

//______________________________________________________________________________________________________________________
bool TransitNodeRoutingGraph::isLocalQuery(unsigned int source, unsigned int target) {
    return isLocal[source][target];
}

// Finds the distance between two nodes based on the TNR data-structure. This is used for the non-local queries.
// In that case, all pairs of access nodes of source and target are checked and the shortest distance from those
// pairs is returned.
//______________________________________________________________________________________________________________________
unsigned int TransitNodeRoutingGraph::findTNRDistance(unsigned int source, unsigned int target) {
    unsigned int shortestDistance = UINT_MAX;

    printf("Finding shortest distance between '%u' (rank: %u, %lu access nodes) and '%u' (rank: %u, %lu access nodes) via TNR.\n", source, data(source).rank, forwardAccessNodes[source].size(), target, data(target).rank, backwardAccessNodes[target].size());
    for(unsigned int i = 0; i < forwardAccessNodes[source].size(); i++) {
        for(unsigned int j = 0; j < backwardAccessNodes[target].size(); j++) {
            unsigned int id1 = transitNodeMapping[forwardAccessNodes[source][i].accessNodeID];
            unsigned int id2 = transitNodeMapping[backwardAccessNodes[target][j].accessNodeID];
            unsigned int newDistance = forwardAccessNodes[source][i].distanceToNode + transitNodesDistanceTable[id1][id2] + backwardAccessNodes[target][j].distanceToNode;
            printf("Possible candidate is pair of transit nodes: %u -> %u, distance %u (%u, %u, %u)\n", forwardAccessNodes[source][i].accessNodeID, backwardAccessNodes[target][j].accessNodeID, newDistance, forwardAccessNodes[source][i].distanceToNode, transitNodesDistanceTable[id1][id2], backwardAccessNodes[target][j].distanceToNode);
            if(newDistance < shortestDistance) {
                shortestDistance = newDistance;
            }
        }
    }

    return shortestDistance;
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraph::addMappingPair(unsigned int realID, unsigned int transitNodesID) {
    transitNodeMapping.insert(make_pair(realID, transitNodesID));
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraph::setDistanceTableValue(unsigned int i, unsigned int j, unsigned int value) {
    transitNodesDistanceTable[i][j] = value;
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraph::setLocalityFilterValue(unsigned int i, unsigned int j, bool value) {
    isLocal[i][j] = value;
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraph::addForwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance) {
    forwardAccessNodes[node].push_back(AccessNodeData(accessNodeID, accessNodeDistance));
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraph::addBackwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance) {
    backwardAccessNodes[node].push_back(AccessNodeData(accessNodeID, accessNodeDistance));
}