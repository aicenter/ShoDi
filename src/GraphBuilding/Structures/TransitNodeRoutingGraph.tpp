//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#include <boost/numeric/conversion/cast.hpp>
#include <climits>
#include "../../TNR/Structures/AccessNodeData.h"
#include "../../DistanceMatrix/Distance_matrix_travel_time_provider.h"

//______________________________________________________________________________________________________________________
template<class T> TransitNodeRoutingGraph<T>::TransitNodeRoutingGraph(unsigned int nodes, unsigned int transitNodesAmount) : FlagsGraph<T>(nodes), forwardAccessNodes(nodes), backwardAccessNodes(nodes), forwardSearchSpaces(nodes), backwardSearchSpaces(nodes), transitNodesDistanceTable(transitNodesAmount, std::vector<unsigned int>(transitNodesAmount)) {

}

//______________________________________________________________________________________________________________________
template<class T> TransitNodeRoutingGraph<T>::~TransitNodeRoutingGraph() = default;

//______________________________________________________________________________________________________________________
template<class T> bool TransitNodeRoutingGraph<T>::isLocalQuery(unsigned int start, unsigned int goal) {
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
template<class T> unsigned int TransitNodeRoutingGraph<T>::findTNRDistance(unsigned int start, unsigned int goal) {
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
template<class T> void TransitNodeRoutingGraph<T>::addMappingPair(unsigned int realID, unsigned int transitNodesID) {
    transitNodeMapping.insert(std::make_pair(realID, transitNodesID));
}

//______________________________________________________________________________________________________________________
template<class T> void TransitNodeRoutingGraph<T>::setDistanceTableValue(unsigned int i, unsigned int j, unsigned int value) {
    transitNodesDistanceTable[i][j] = value;
}

//______________________________________________________________________________________________________________________
template<class T> void TransitNodeRoutingGraph<T>::addForwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance) {
    forwardAccessNodes[node].push_back(AccessNodeData(accessNodeID, accessNodeDistance));
}

//______________________________________________________________________________________________________________________
template<class T> void TransitNodeRoutingGraph<T>::addBackwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance) {
    backwardAccessNodes[node].push_back(AccessNodeData(accessNodeID, accessNodeDistance));
}

//______________________________________________________________________________________________________________________
template<class T> void TransitNodeRoutingGraph<T>::addForwardSearchSpaceNode(unsigned int sourceNode, unsigned int searchSpaceNode) {
    forwardSearchSpaces[sourceNode].push_back(searchSpaceNode);
}

//______________________________________________________________________________________________________________________
template<class T> void TransitNodeRoutingGraph<T>::addBackwardSearchSpaceNode(unsigned int sourceNode, unsigned int searchSpaceNode) {
    backwardSearchSpaces[sourceNode].push_back(searchSpaceNode);
}

//______________________________________________________________________________________________________________________
template<class T> void TransitNodeRoutingGraph<T>::accessNodesTest(Distance_matrix_travel_time_provider<dist_t>& dm) {
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

template<class T> const std::vector<std::vector<AccessNodeData>> &TransitNodeRoutingGraph<T>::getForwardAccessNodes() const {
    return forwardAccessNodes;
}

template<class T> const std::vector<std::vector<AccessNodeData>> &TransitNodeRoutingGraph<T>::getBackwardAccessNodes() const {
    return backwardAccessNodes;
}

template<class T> const std::vector<std::vector<unsigned int>> &TransitNodeRoutingGraph<T>::getForwardSearchSpaces() const {
    return forwardSearchSpaces;
}

template<class T> const std::vector<std::vector<unsigned int>> &TransitNodeRoutingGraph<T>::getBackwardSearchSpaces() const {
    return backwardSearchSpaces;
}

template<class T> const std::vector<std::vector<unsigned int>> &TransitNodeRoutingGraph<T>::getTransitNodesDistanceTable() const {
    return transitNodesDistanceTable;
}

template<class T> const std::unordered_map<unsigned int, unsigned int> &TransitNodeRoutingGraph<T>::getTransitNodeMapping() const {
    return transitNodeMapping;
}
