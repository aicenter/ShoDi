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
// Created on: 15.10.19
//

#ifndef CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGGRAPHFORPATHQUERIES_H
#define CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGGRAPHFORPATHQUERIES_H

#include "FlagsGraphWithUnpackingData.h"
#include "../../TNR/Structures/AccessNodeData.h"

// A Transit Node Routing data structure. This variant can be used to also obtain the actual shortest paths and not
// just the shortest distances.
//______________________________________________________________________________________________________________________
class TransitNodeRoutingGraphForPathQueries : public FlagsGraphWithUnpackingData {
public:
    TransitNodeRoutingGraphForPathQueries(unsigned int nodes, unsigned int transitNodesAmount);
    void addUnpackingEdge(unsigned int from, unsigned int to, unsigned int weight);
    const std::vector < std::pair< unsigned int , unsigned int > > & unpackingNeighbours(unsigned int nodeID);

    // From TNRGraph
    bool isLocalQuery(unsigned int source, unsigned int target);
    unsigned int findTNRDistance(unsigned int source, unsigned int target);
    void addMappingPair(unsigned int realID, unsigned int transitNodesID);
    void setDistanceTableValue(unsigned int i, unsigned int j, unsigned int value);
    void addForwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance);
    void addBackwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance);
    void addForwardSearchSpaceNode(unsigned int sourceNode, unsigned int searchSpaceNode);
    void addBackwardSearchSpaceNode(unsigned int sourceNode, unsigned int searchSpaceNode);
protected:
    std::vector < std::vector < std::pair< unsigned int, unsigned int > > > unpackingGraph;

    // From TNRGraph
    std::vector < std::vector < AccessNodeData > > forwardAccessNodes;
    std::vector < std::vector < AccessNodeData > > backwardAccessNodes;
    std::vector < std::vector < unsigned int > > forwardSearchSpaces;
    std::vector < std::vector < unsigned int > > backwardSearchSpaces;
    std::vector < std::vector < unsigned int > > transitNodesDistanceTable;
    std::unordered_map< unsigned int, unsigned int > transitNodeMapping;
};


#endif //CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGGRAPHFORPATHQUERIES_H
