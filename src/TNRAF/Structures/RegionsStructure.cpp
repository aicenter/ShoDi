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
// Created on: 02.12.19
//

#include "RegionsStructure.h"
#include <stdexcept> // For std::out_of_range

//______________________________________________________________________________________________________________________
RegionsStructure::RegionsStructure(const std::vector<unsigned int>& assignedClusters, unsigned int regionsCnt)
    : regions(regionsCnt), mapping(assignedClusters), regionsCnt(regionsCnt) {
    for (unsigned int nodeId = 0; nodeId < assignedClusters.size(); ++nodeId) {
        unsigned int regionId = assignedClusters[nodeId];
        if (regionId < regionsCnt) { // Ensure regionId is valid
            regions[regionId].push_back(nodeId);
        }
        // Else: handle error or skip if regionId is out of bounds, though assignedClusters should be correct.
    }
}

//______________________________________________________________________________________________________________________
std::vector < unsigned int > & RegionsStructure::nodesInRegion(unsigned int regionId) {
    return regions[regionId];
}

//______________________________________________________________________________________________________________________
unsigned int RegionsStructure::getRegion(unsigned int nodeId) {
    return mapping[nodeId];
}

//______________________________________________________________________________________________________________________
unsigned int RegionsStructure::getRegionsCnt() {
    return regionsCnt;
}

//______________________________________________________________________________________________________________________
// Implementation for Regions_with_borders
//______________________________________________________________________________________________________________________

Regions_with_borders::Regions_with_borders(
    const std::vector<unsigned int>& assignedClusters,
    unsigned int regionsCnt,
    Graph& originalGraph)
    : RegionsStructure(assignedClusters, regionsCnt),
      border_nodes_by_region_(regionsCnt) {
    computeBorderNodes(originalGraph);
}

//______________________________________________________________________________________________________________________
void Regions_with_borders::computeBorderNodes(Graph& originalGraph) {
    for (unsigned int regionId = 0; regionId < getRegionsCnt(); ++regionId) {
        const std::vector<unsigned int>& nodes_in_current_region = nodesInRegion(regionId);
        for (unsigned int nodeId : nodes_in_current_region) {
            bool is_border_node = false;

            // Check outgoing edges
            const auto& outgoing = originalGraph.outgoingEdges(nodeId);
            for (const auto& edge_pair : outgoing) {
                unsigned int neighborNodeId = edge_pair.first;
                if (getRegion(neighborNodeId) != regionId) {
                    is_border_node = true;
                    break;
                }
            }

            if (is_border_node) {
                border_nodes_by_region_[regionId].push_back(nodeId);
                continue; // Already identified as border node
            }

            // Check incoming edges if not already identified by an outgoing edge
            const auto& incoming = originalGraph.incomingEdges(nodeId);
            for (const auto& edge_pair : incoming) {
                unsigned int neighborNodeId = edge_pair.first;
                if (getRegion(neighborNodeId) != regionId) {
                    is_border_node = true;
                    break;
                }
            }

            if (is_border_node) {
                border_nodes_by_region_[regionId].push_back(nodeId);
            }
        }
    }
}

//______________________________________________________________________________________________________________________
const std::vector<unsigned int>& Regions_with_borders::getBorderNodes(unsigned int regionId) const {
    if (regionId >= border_nodes_by_region_.size()) {
        throw std::out_of_range("Region ID out of range in getBorderNodes.");
    }
    return border_nodes_by_region_[regionId];
}