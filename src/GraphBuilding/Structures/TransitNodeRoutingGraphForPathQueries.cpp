//
// Author: Xenty (Michal Cvach)
// Created on: 15.10.19
//

#include "TransitNodeRoutingGraphForPathQueries.h"

//______________________________________________________________________________________________________________________
TransitNodeRoutingGraphForPathQueries::TransitNodeRoutingGraphForPathQueries(unsigned int nodes, unsigned int transitNodesAmount) : IntegerFlagsGraphWithUnpackingData(nodes), unpackingGraph(nodes), forwardAccessNodes(nodes), backwardAccessNodes(nodes), transitNodesDistanceTable(transitNodesAmount, vector<unsigned int>(transitNodesAmount)), forwardSearchSpaces(nodes), backwardSearchSpaces(nodes) {

}

//______________________________________________________________________________________________________________________
const vector < pair < unsigned int , unsigned int > > & TransitNodeRoutingGraphForPathQueries::unpackingNeighbours(unsigned int nodeID) {
    return unpackingGraph[nodeID];
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraphForPathQueries::addUnpackingEdge(unsigned int from, unsigned int to, unsigned int weight) {
    unpackingGraph[from].push_back(make_pair(to, weight));
}

// FROM TNRGraph
//______________________________________________________________________________________________________________________
bool TransitNodeRoutingGraphForPathQueries::isLocalQuery(unsigned int source, unsigned int target) {
    for(unsigned int k = 0; k < forwardSearchSpaces[source].size(); k++) {
        for(unsigned int m = 0; m < backwardSearchSpaces[target].size(); m++) {
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
    for(unsigned int i = 0; i < forwardAccessNodes[source].size(); i++) {
        for(unsigned int j = 0; j < backwardAccessNodes[target].size(); j++) {
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
    transitNodeMapping.insert(make_pair(realID, transitNodesID));
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraphForPathQueries::setDistanceTableValue(unsigned int i, unsigned int j, unsigned int value) {
    transitNodesDistanceTable[i][j] = value;
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraphForPathQueries::setLocalityFilterValue(unsigned int i, unsigned int j, bool value) {
    //isLocal[i][j] = value;
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