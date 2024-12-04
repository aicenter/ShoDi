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
// Created on: 7.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPATHQUERYMANAGER_H
#define TRANSIT_NODE_ROUTING_CHPATHQUERYMANAGER_H

#include <vector>
#include <queue>
#include "../GraphBuilding/Structures/FlagsGraphWithUnpackingData.h"
#include "../GraphBuilding/Structures/SimpleEdge.h"



/**
 * This class is responsible for the Contraction Hierarchies 'path' queries - when we require the actual path and not
 * only the distance between two points.
 */
class CHPathQueryManager {
public:
    CHPathQueryManager(FlagsGraphWithUnpackingData & g);
    unsigned int findDistanceOutputPath(const unsigned int source, const unsigned int target);
    unsigned int findDistanceOnly(const unsigned int source, const unsigned int target);
    unsigned int findPath(const unsigned int source, const unsigned int target, std::vector<std::pair<unsigned int, unsigned int>> & edges, std::vector<unsigned int> & edgeLengths);
    unsigned int findPath(const unsigned int source, const unsigned int target, std::vector<std::pair<unsigned int, unsigned int>> & edges);
    unsigned int findPath(const unsigned int source, const unsigned int target, std::vector<SimpleEdge> & path);
    void printEdgesForwardShortcut(const unsigned int source, const unsigned int target);
    void printEdgesBackwardShortcut(const unsigned int source, const unsigned int target);
protected:
    unsigned int processQuery(const unsigned int source, const unsigned int target);
    void forwardStall(unsigned int stallnode, unsigned int stalldistance);
    void backwardStall(unsigned int stallnode, unsigned int stalldistance);
    void outputPath(const unsigned int meetingNode);
    void fillPathInfo(const unsigned int meetingNode, std::vector<std::pair<unsigned int, unsigned int>> & edges, std::vector<unsigned int> & edgeLengths);
    void fillPathInfoEdgesOnly(const unsigned int meetingNode, std::vector<std::pair<unsigned int, unsigned int>> & edges);
    void fillPathInfoEdgesOnly(const unsigned int meetingNode, std::vector<SimpleEdge> & edges);
    void fillFromPath(const unsigned int meetingNode, std::vector<std::pair<unsigned int, unsigned int> > & fromPath);
    void fillToPath(const unsigned int meetingNode, std::vector<std::pair<unsigned int, unsigned int> > & toPath);
    void unpackPrevious(std::vector<std::pair<unsigned int, unsigned int> > & fromPath);
    void unpackFollowing(std::vector<std::pair<unsigned int, unsigned int> > & fromPath);
    void getPreviousPathPart(std::vector<std::pair<unsigned int, unsigned int> > & fromPath, std::vector<std::pair<unsigned int, unsigned int>> & edges, std::vector<unsigned int> & edgeLengths);
    void getFollowingPathPart(std::vector<std::pair<unsigned int, unsigned int> > & fromPath, std::vector<std::pair<unsigned int, unsigned int>> & edges, std::vector<unsigned int> & edgeLengths);
    void getPreviousPathPartEdgesOnly(std::vector<std::pair<unsigned int, unsigned int> > & fromPath, std::vector<std::pair<unsigned int, unsigned int>> & edges);
    void getFollowingPathPartEdgesOnly(std::vector<std::pair<unsigned int, unsigned int> > & fromPath, std::vector<std::pair<unsigned int, unsigned int>> & edges);
    void getPreviousPathPartEdgesOnly(std::vector<std::pair<unsigned int, unsigned int> > & fromPath, std::vector<SimpleEdge> & path);
    void getFollowingPathPartEdgesOnly(std::vector<std::pair<unsigned int, unsigned int> > & fromPath, std::vector<SimpleEdge> & path);
    void unpackForwardEdge(unsigned int s, unsigned int t);
    void unpackBackwardEdge(unsigned int s, unsigned int t);
    void getForwardEdge(unsigned int s, unsigned int t, std::vector<std::pair<unsigned int, unsigned int>> & edges, std::vector<unsigned int> & edgeLengths);
    void getBackwardEdge(unsigned int s, unsigned int t, std::vector<std::pair<unsigned int, unsigned int>> & edges, std::vector<unsigned int> & edgeLengths);
    void getForwardEdgeWithoutLength(unsigned int s, unsigned int t, std::vector<std::pair<unsigned int, unsigned int>> & edges);
    void getBackwardEdgeWithoutLength(unsigned int s, unsigned int t, std::vector<std::pair<unsigned int, unsigned int>> & edges);
    void getForwardEdgeWithoutLength(unsigned int s, unsigned int t, std::vector<SimpleEdge> & path);
    void getBackwardEdgeWithoutLength(unsigned int s, unsigned int t, std::vector<SimpleEdge> & path);
    FlagsGraphWithUnpackingData & graph;
    unsigned int upperbound;
    unsigned int meetingNode;
    std::vector<unsigned int> forwardChanged;
    std::vector<unsigned int> backwardChanged;
    std::vector<unsigned int> forwardStallChanged;
    std::vector<unsigned int> backwardStallChanged;
    void prepareStructuresForNextQuery();
};


#endif //TRANSIT_NODE_ROUTING_CHPATHQUERYMANAGER_H
