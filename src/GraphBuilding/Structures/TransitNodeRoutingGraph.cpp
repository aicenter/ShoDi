//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include <climits>
#include "TransitNodeRoutingGraph.h"

//______________________________________________________________________________________________________________________
TransitNodeRoutingGraph::TransitNodeRoutingGraph(unsigned int nodes, unsigned int transitNodesAmount) : FlagsGraph(nodes), forwardAccessNodes(nodes), backwardAccessNodes(nodes), transitNodesDistanceTable(transitNodesAmount, vector<unsigned int>(transitNodesAmount)), forwardSearchSpaces(nodes), backwardSearchSpaces(nodes) {

}

// Determines whether the query is local or global. Global queries can be answered using the transit node set, local
// queries must be answered using some fallback algorithm (Contraction Hierarchies are used here). In our case, a query
// is considered local if the intersection of the forward search space of source and the backward search space of target
// is unempty (this function then returns true). If it is empty, the query is global (this function returns false).
//______________________________________________________________________________________________________________________
bool TransitNodeRoutingGraph::isLocalQuery(unsigned int source, unsigned int target) {
    for(unsigned int k = 0; k < forwardSearchSpaces[source].size(); k++) {
        for(unsigned int m = 0; m < backwardSearchSpaces[target].size(); m++) {
            if(forwardSearchSpaces[source][k] == backwardSearchSpaces[target][m]) {
                return true;
            }
        }
    }
    return false;
}

// Finds the distance between two nodes based on the TNR data-structure. This is used for the non-local queries.
// In that case, all pairs of access nodes of source and target are checked and the shortest distance from those
// pairs is returned.
//______________________________________________________________________________________________________________________
unsigned int TransitNodeRoutingGraph::findTNRDistance(unsigned int source, unsigned int target) {
    unsigned int shortestDistance = UINT_MAX;

    for(unsigned int i = 0; i < forwardAccessNodes[source].size(); i++) {
        for(unsigned int j = 0; j < backwardAccessNodes[target].size(); j++) {
            unsigned int id1 = transitNodeMapping[forwardAccessNodes[source][i].accessNodeID];
            unsigned int id2 = transitNodeMapping[backwardAccessNodes[target][j].accessNodeID];
            unsigned int newDistance = forwardAccessNodes[source][i].distanceToNode + transitNodesDistanceTable[id1][id2] + backwardAccessNodes[target][j].distanceToNode;
            if(newDistance < shortestDistance && transitNodesDistanceTable[id1][id2] != UINT_MAX) {
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
void TransitNodeRoutingGraph::addForwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance) {
    forwardAccessNodes[node].push_back(AccessNodeData(accessNodeID, accessNodeDistance));
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraph::addBackwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance) {
    backwardAccessNodes[node].push_back(AccessNodeData(accessNodeID, accessNodeDistance));
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraph::addForwardSearchSpaceNode(unsigned int sourceNode, unsigned int searchSpaceNode) {
    forwardSearchSpaces[sourceNode].push_back(searchSpaceNode);
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraph::addBackwardSearchSpaceNode(unsigned int sourceNode, unsigned int searchSpaceNode) {
    backwardSearchSpaces[sourceNode].push_back(searchSpaceNode);
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraph::accessNodesTest(DistanceMatrix & dm) {
    unsigned int allAccessNodes = 0;
    unsigned int invalidDistanceNodes = 0;

    for(unsigned int i = 0; i < forwardAccessNodes.size(); ++i) {
        for(unsigned int j = 0; j < forwardAccessNodes[i].size(); ++j) {
            allAccessNodes++;
            if(forwardAccessNodes[i][j].distanceToNode != dm.findDistance(i, forwardAccessNodes[i][j].accessNodeID)) {
                invalidDistanceNodes++;
            }
        }
    }

    printf("There are %u out of %u access nodes that have invalid distances. This means %lf %%.\n", invalidDistanceNodes, allAccessNodes, ((double) invalidDistanceNodes / allAccessNodes) * 100);
}