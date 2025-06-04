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

#ifndef CONTRACTION_HIERARCHIES_REGIONSSTRUCTURE_H
#define CONTRACTION_HIERARCHIES_REGIONSSTRUCTURE_H

#include <vector>
#include "../../GraphBuilding/Structures/Graph.h"

/**
 * Auxiliary structure that is used during the Arc Flags computation. This structure gathers information about the
 * partitioning of the graph into k regions.
 */
class RegionsStructure {
public:
    /**
     * Initializes the region structure.
     *
     * @param assignedClusters[in] A vector where the index is the node ID and the value is the region ID.
     * @param regionsCnt[in] The amount of regions we will be working with.
     */
    RegionsStructure(
            const std::vector<unsigned int>& assignedClusters,
            unsigned int regionsCnt);

    /**
     * Returns a std::vector of nodes assigned to a certain region.
     *
     * @param regionId[in] The region we are interested in.
     * @return Reference to a std::vector containing all the nodes corresponding to the given region.
     */
    std::vector < unsigned int > & nodesInRegion(
            unsigned int regionId);

    /**
     * Returns a region for a given node.
     *
     * @param nodeId[in] The node we are interested in.
     * @return The region ID for the given node.
     */
    unsigned int getRegion(
            unsigned int nodeId);

    /**
     * Returns the amount of regions we are working with.
     *
     * @return The number of regions.
     */
    unsigned int getRegionsCnt();

private:
    std::vector < std::vector < unsigned int > > regions;
    std::vector < unsigned int > mapping;
    unsigned int regionsCnt;
};

/**
 * Extends RegionsStructure to also identify and store border nodes for each region.
 * A border node is a node within a region that has at least one edge (incoming or outgoing)
 * connected to a node in a different region.
 */
class Regions_with_borders : public RegionsStructure {
public:
    /**
     * Initializes the regions structure and computes border nodes.
     *
     * @param assignedClusters[in] A vector where the index is the node ID and the value is the region ID.
     * @param regionsCnt[in] The amount of regions.
     * @param originalGraph[in] The graph used to determine connectivity for border nodes.
     */
    Regions_with_borders(
            const std::vector<unsigned int>& assignedClusters,
            unsigned int regionsCnt,
            Graph& originalGraph);

    /**
     * Returns a list of border nodes for a given region.
     *
     * @param regionId[in] The region we are interested in.
     * @return Constant reference to a std::vector containing the border nodes of the given region.
     */
    const std::vector<unsigned int>& getBorderNodes(unsigned int regionId) const;

private:
    std::vector<std::vector<unsigned int>> border_nodes_by_region_;

    /**
     * Computes border nodes for all regions based on the original graph.
     * @param originalGraph The graph to check for inter-region connectivity.
     */
    void computeBorderNodes(Graph& originalGraph);
};


#endif //CONTRACTION_HIERARCHIES_REGIONSSTRUCTURE_H
