//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include <boost/numeric/conversion/cast.hpp>
#include <climits>
#include "TransitNodeRoutingGraph.h"

//______________________________________________________________________________________________________________________
TransitNodeRoutingGraph::TransitNodeRoutingGraph(unsigned int nodes, unsigned int transitNodesAmount) : FlagsGraph(nodes), forwardAccessNodes(nodes), backwardAccessNodes(nodes), forwardSearchSpaces(nodes), backwardSearchSpaces(nodes), transitNodesDistanceTable(transitNodesAmount, std::vector<unsigned int>(transitNodesAmount)) {

}

//______________________________________________________________________________________________________________________
TransitNodeRoutingGraph::~TransitNodeRoutingGraph() = default;

//______________________________________________________________________________________________________________________
bool TransitNodeRoutingGraph::isLocalQuery(unsigned int start, unsigned int goal) {
    for(size_t k = 0; k < forwardSearchSpaces[start].size(); k++) {
        for(size_t m = 0; m < backwardSearchSpaces[goal].size(); m++) {
            if(forwardSearchSpaces[start][k] == backwardSearchSpaces[goal][m]) {
                return true;
            }
        }
    }
    return false;
}

//______________________________________________________________________________________________________________________
unsigned int TransitNodeRoutingGraph::findTNRDistance(unsigned int start, unsigned int goal) {
    unsigned int shortestDistance = UINT_MAX;

    for(size_t i = 0; i < forwardAccessNodes[start].size(); i++) {
        for(size_t j = 0; j < backwardAccessNodes[goal].size(); j++) {
            unsigned int id1 = transitNodeMapping[forwardAccessNodes[start][i].accessNodeID];
            unsigned int id2 = transitNodeMapping[backwardAccessNodes[goal][j].accessNodeID];
            unsigned int newDistance = forwardAccessNodes[start][i].distanceToNode + transitNodesDistanceTable[id1][id2] + backwardAccessNodes[goal][j].distanceToNode;
            if(newDistance < shortestDistance && transitNodesDistanceTable[id1][id2] != UINT_MAX) {
                shortestDistance = newDistance;
            }
        }
    }

    return shortestDistance;
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingGraph::addMappingPair(unsigned int realID, unsigned int transitNodesID) {
    transitNodeMapping.insert(std::make_pair(realID, transitNodesID));
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
void TransitNodeRoutingGraph::accessNodesTest(Distance_matrix_travel_time_provider & dm) {
    unsigned int allAccessNodes = 0;
    unsigned int invalidDistanceNodes = 0;

    const unsigned int fwNodesCnt = boost::numeric_cast<unsigned int>(forwardAccessNodes.size());

    for(unsigned int i = 0; i < fwNodesCnt; ++i) {
        for(size_t j = 0; j < forwardAccessNodes[i].size(); ++j) {
            allAccessNodes++;
            if(forwardAccessNodes[i][j].distanceToNode != dm.findDistance(i, forwardAccessNodes[i][j].accessNodeID)) {
                invalidDistanceNodes++;
            }
        }
    }

    printf("There are %u out of %u access nodes that have invalid distances. This means %lf %%.\n", invalidDistanceNodes, allAccessNodes, ((double) invalidDistanceNodes / allAccessNodes) * 100);
}

const std::vector<std::vector<AccessNodeData>> &TransitNodeRoutingGraph::getForwardAccessNodes() const {
    return forwardAccessNodes;
}

const std::vector<std::vector<AccessNodeData>> &TransitNodeRoutingGraph::getBackwardAccessNodes() const {
    return backwardAccessNodes;
}

const std::vector<std::vector<unsigned int>> &TransitNodeRoutingGraph::getForwardSearchSpaces() const {
    return forwardSearchSpaces;
}

const std::vector<std::vector<unsigned int>> &TransitNodeRoutingGraph::getBackwardSearchSpaces() const {
    return backwardSearchSpaces;
}

const std::vector<std::vector<unsigned int>> &TransitNodeRoutingGraph::getTransitNodesDistanceTable() const {
    return transitNodesDistanceTable;
}

const std::unordered_map<unsigned int, unsigned int> &TransitNodeRoutingGraph::getTransitNodeMapping() const {
    return transitNodeMapping;
}
