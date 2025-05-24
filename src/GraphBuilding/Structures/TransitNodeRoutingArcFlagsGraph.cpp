/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#include <climits>
#include <iostream>
#include "TransitNodeRoutingArcFlagsGraph.h"

//______________________________________________________________________________________________________________________
TransitNodeRoutingArcFlagsGraph::TransitNodeRoutingArcFlagsGraph(unsigned int nodes, unsigned int transitNodesAmount) :
	TransitNodeRoutingGraph(nodes, transitNodesAmount) {
}

//______________________________________________________________________________________________________________________
TransitNodeRoutingArcFlagsGraph::~TransitNodeRoutingArcFlagsGraph() = default;

//______________________________________________________________________________________________________________________
void TransitNodeRoutingArcFlagsGraph::addForwardAccessNode(
	unsigned int node,
	unsigned int accessNodeID,
	unsigned int accessNodeDistance,
	unsigned int regionsCnt,
	uint32_t regionsFlags,
	std::vector<unsigned int>& powersOf2
) {
	forwardAccessNodes[node].push_back(
		AccessNodeDataArcFlags(accessNodeID, accessNodeDistance, regionsCnt, regionsFlags, powersOf2)
	);
}

//______________________________________________________________________________________________________________________
void TransitNodeRoutingArcFlagsGraph::addBackwardAccessNode(
	unsigned int node,
	unsigned int accessNodeID,
	unsigned int accessNodeDistance,
	unsigned int regionsCnt,
	uint32_t regionsFlags,
	std::vector<unsigned int>& powersOf2
) {
	backwardAccessNodes[node].push_back(
		AccessNodeDataArcFlags(accessNodeID, accessNodeDistance, regionsCnt, regionsFlags, powersOf2)
	);
}

//______________________________________________________________________________________________________________________
unsigned int TransitNodeRoutingArcFlagsGraph::findTNRAFDistance(unsigned int start, unsigned int goal) {
	unsigned int shortestDistance = UINT_MAX;
	unsigned int sourceRegion = nodesData[start].region;
	unsigned int targetRegion = nodesData[goal].region;

	for (size_t i = 0; i < forwardAccessNodes[start].size(); i++) {
		if (forwardAccessNodes[start][i].regionFlags[targetRegion]) {
			for (size_t j = 0; j < backwardAccessNodes[goal].size(); j++) {
				if (backwardAccessNodes[goal][j].regionFlags[sourceRegion]) {
					unsigned int id1 = transitNodeMapping[forwardAccessNodes[start][i].accessNodeID];
					unsigned int id2 = transitNodeMapping[backwardAccessNodes[goal][j].accessNodeID];
					unsigned int newDistance = forwardAccessNodes[start][i].distanceToNode + transitNodesDistanceTable[
						id1][id2] + backwardAccessNodes[goal][j].distanceToNode;
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
