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
// Created on: 21.8.18
//

#include <fstream>
#include <boost/numeric/conversion/cast.hpp>
#include <stdexcept>
#include "UpdateableGraph.h"

//______________________________________________________________________________________________________________________
Graph * UpdateableGraph::createCopy() {
    auto n = nodes();
    Graph * newInst = new Graph(n);
    for(unsigned int i = 0; i < n; i++) {
        for (auto iter = followingNodes[i].begin(); iter != followingNodes[i].end(); ++iter) {
            unsigned int target = (*iter).first;
            unsigned int weight = (*iter).second.weight;
            newInst->addEdge(i, target, weight);
        }
    }
    return newInst;
}

//______________________________________________________________________________________________________________________
UpdateableGraph::UpdateableGraph(unsigned int n) {
    this->followingNodes.resize(n);
    this->previousNodes.resize(n);
    this->ranks.resize(n);
}

//______________________________________________________________________________________________________________________
bool UpdateableGraph::addEdge(unsigned int from, unsigned int to, dist_t weight) {
    if (followingNodes.at(from).count(to) == 1) {
        if (followingNodes.at(from).at(to).weight > weight) {
            followingNodes.at(from).at(to).weight = weight;
            previousNodes.at(to).at(from) = weight;
            return true;
        }
        return false;
    } else {
        followingNodes.at(from).insert(std::make_pair(to, PreprocessingEdgeData(weight, 0, false)));
        previousNodes.at(to).insert(std::make_pair(from, weight));
        return true;
    }
}

//______________________________________________________________________________________________________________________
bool UpdateableGraph::addShortcutEdge(unsigned int from, unsigned int to, dist_t weight, unsigned int middlenode) {
    if (followingNodes.at(from).count(to) == 1) {
        if (followingNodes.at(from).at(to).weight > weight) {
            followingNodes.at(from).at(to).weight = weight;
            followingNodes.at(from).at(to).middleNode = middlenode;
            followingNodes.at(from).at(to).isShortcut = true;
            previousNodes.at(to).at(from) = weight;
            return true;
        }
        return false;
    } else {
        followingNodes.at(from).insert(std::make_pair(to, PreprocessingEdgeData(weight, middlenode, true)));
        previousNodes.at(to).insert(std::make_pair(from, weight));
        return true;
    }
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::removeEdge(unsigned int from, unsigned int to) {
    followingNodes.at(from).erase(to);
    previousNodes.at(to).erase(from);
}

//______________________________________________________________________________________________________________________
unsigned int UpdateableGraph::getRank(unsigned int nodeID) const {
    return ranks[nodeID];
}

//______________________________________________________________________________________________________________________
unsigned int UpdateableGraph::nodes() const {
    return boost::numeric_cast<unsigned int>(followingNodes.size());
}

//______________________________________________________________________________________________________________________
bool UpdateableGraph::isShortcut(const unsigned int from, const unsigned int to) {
    return followingNodes[from].at(to).isShortcut;
}

//______________________________________________________________________________________________________________________
const std::unordered_map<unsigned int, unsigned int> & UpdateableGraph::incomingEdges(const unsigned int x)const {
    return previousNodes.at(x);
}

//______________________________________________________________________________________________________________________
const std::unordered_map<unsigned int, PreprocessingEdgeData> & UpdateableGraph::outgoingEdges(const unsigned int x)const {
    return followingNodes.at(x);
}

//______________________________________________________________________________________________________________________
unsigned int UpdateableGraph::degree(unsigned int node)const {
    return boost::numeric_cast<unsigned int>(followingNodes.at(node).size() + previousNodes.at(node).size());
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::setRank(unsigned int node, unsigned int rank) {
    ranks[node] = rank;
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::getNodesWithHighestRank(std::vector< unsigned int > & highestNodes, unsigned int requestedAmount) {
    unsigned int curPosition = 0;
    for(unsigned int i = 0; i < requestedAmount; i++) {
        while(ranks[curPosition] <= nodes() - requestedAmount) {
            curPosition++;
        }
        highestNodes[i] = curPosition;
        curPosition++;
    }
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushInDdsgFormat(std::string filePath) {
    std::vector < OutputEdge > edges;
    std::vector < OutputShortcutEdge > shortcuts;
    printf("Reordering edges for file writing\n");
    prepareEdgesForFlushing(edges, shortcuts);

    std::ofstream output;
    output.open ( filePath + ".ch", std::ios::binary );
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (filePath + ".ch").c_str());
    }

    printf("Actually flushing the CH graph into the file\n");
    flushHeader(output);
    flushCnts(output, boost::numeric_cast<unsigned int>(followingNodes.size()), boost::numeric_cast<unsigned int>(edges.size()), boost::numeric_cast<unsigned int>(shortcuts.size()));
    flushRanks(output);
    flushOriginalEdges(output, edges);
    flushShortcutEdges(output, shortcuts);
    flushTerminator(output);

    output.close();

}

//______________________________________________________________________________________________________________________
void UpdateableGraph::outputAsXenGraph(std::string filePath) {
    std::ofstream output;
    output.open ( filePath + ".xeng" );
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (filePath + ".xeng").c_str());
    }

    size_t edges = 0;
    for(unsigned int i = 0; i < nodes(); ++i) {
        edges += followingNodes[i].size();
    }

    output << "XGI " << nodes() << " " << edges << std::endl;

    for(unsigned int i = 0; i < nodes(); ++i) {
        for(auto iter = followingNodes[i].begin(); iter != followingNodes[i].end(); ++iter) {
            output << i << " " << (*iter).first << " " << (*iter).second.weight << " 1" << std::endl;
        }
    }

    output.close();
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushHeader(std::ostream & output) {
    char a, b, c, d;
    a = 0x43;
    b = 0x48;
    c = 0x0d;
    d = 0x0a;
    unsigned int version = 1;
    output.write(&a, sizeof (a));
    output.write(&b, sizeof (b));
    output.write(&c, sizeof (c));
    output.write(&d, sizeof (d));
    output.write((char *) &version, sizeof (version));
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushCnts(std::ostream & output, const unsigned int nodes, const unsigned int edges, const unsigned int shortcuts) {
    output.write((char *) &nodes, sizeof (nodes));
    output.write((char *) &edges, sizeof (edges));
    output.write((char *) &shortcuts, sizeof (shortcuts));
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushRanks(std::ostream & output) {
    for(size_t i = 0; i < ranks.size(); i++) {
        output.write((char *) &ranks[i], sizeof (unsigned int));
    }
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushOriginalEdges(std::ostream & output, std::vector < OutputEdge > & edges) {
    for(size_t i = 0; i < edges.size(); i++) {
        output.write((char *) &edges[i].sourceNode, sizeof (unsigned int));
        output.write((char *) &edges[i].targetNode, sizeof (unsigned int));
        output.write((char *) &edges[i].weight, sizeof (unsigned int));
        output.write((char *) &edges[i].flags, sizeof (unsigned int));
    }
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushShortcutEdges(std::ostream & output, std::vector < OutputShortcutEdge > & edges) {
    for(size_t i = 0; i < edges.size(); i++) {
        output.write((char *) &edges[i].sourceNode, sizeof (unsigned int));
        output.write((char *) &edges[i].targetNode, sizeof (unsigned int));
        output.write((char *) &edges[i].weight, sizeof (unsigned int));
        output.write((char *) &edges[i].flags, sizeof (unsigned int));
        output.write((char *) &edges[i].middleNode, sizeof (unsigned int));
    }
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushTerminator(std::ostream & output) {
    unsigned int footerNum = 0x12345678;
    output.write((char *) &footerNum, sizeof (footerNum));
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::prepareEdgesForFlushing(std::vector < OutputEdge > & edges, std::vector < OutputShortcutEdge > & shortcuts) {
   const unsigned int followingNodesCnt = boost::numeric_cast<unsigned int>(followingNodes.size());

   for(unsigned int i = 0; i < followingNodesCnt; i++) {
        for(auto iter = followingNodes[i].begin(); iter != followingNodes[i].end(); ++iter) {
            if (ranks[i] < ranks[(*iter).first]) {
                unsigned int flags = 1;
                if (followingNodes[(*iter).first].count(i) == 1 && followingNodes[(*iter).first].at(i).weight == (*iter).second.weight && followingNodes[(*iter).first].at(i).middleNode == (*iter).second.middleNode) {
                    flags += 2;
                    followingNodes[(*iter).first].erase(i);
                }
                if ((*iter).second.isShortcut) {
                    flags += 4;
                    shortcuts.push_back(OutputShortcutEdge(i, (*iter).first, (*iter).second.weight, flags, (*iter).second.middleNode));
                } else {
                    edges.push_back(OutputEdge(i, (*iter).first, (*iter).second.weight, flags));
                }
            } else {
                unsigned int flags = 2;
                if (followingNodes[(*iter).first].count(i) == 1 && followingNodes[(*iter).first].at(i).weight == (*iter).second.weight && followingNodes[(*iter).first].at(i).middleNode == (*iter).second.middleNode) {
                    flags += 1;
                    followingNodes[(*iter).first].erase(i);
                }
                if ((*iter).second.isShortcut) {
                    flags += 4;
                    shortcuts.push_back(OutputShortcutEdge((*iter).first, i, (*iter).second.weight, flags, (*iter).second.middleNode));
                } else {
                    edges.push_back(OutputEdge((*iter).first, i, (*iter).second.weight, flags));
                }
            }
        }
    }
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::prepareEdgesForFlushingWithReinsert(std::vector < OutputEdge > & edges, std::vector < OutputShortcutEdge > & shortcuts) {
    std::vector<std::pair<std::pair<unsigned int, unsigned int>, PreprocessingEdgeData>> removedEdges;

   const unsigned int followingNodesCnt = boost::numeric_cast<unsigned int>(followingNodes.size());

   for(unsigned int i = 0; i < followingNodesCnt; i++) {
        for(auto iter = followingNodes[i].begin(); iter != followingNodes[i].end(); ++iter) {
            if (ranks[i] < ranks[(*iter).first]) {
                unsigned int flags = 1;
                if (followingNodes[(*iter).first].count(i) == 1 && followingNodes[(*iter).first].at(i).weight == (*iter).second.weight && followingNodes[(*iter).first].at(i).middleNode == (*iter).second.middleNode) {
                    flags += 2;
                    removedEdges.push_back(std::make_pair(std::make_pair((*iter).first, i), PreprocessingEdgeData(followingNodes[(*iter).first].at(i))));
                    followingNodes[(*iter).first].erase(i);
                }
                if ((*iter).second.isShortcut) {
                    flags += 4;
                    shortcuts.push_back(OutputShortcutEdge(i, (*iter).first, (*iter).second.weight, flags, (*iter).second.middleNode));
                } else {
                    edges.push_back(OutputEdge(i, (*iter).first, (*iter).second.weight, flags));
                }
            } else {
                unsigned int flags = 2;
                if (followingNodes[(*iter).first].count(i) == 1 && followingNodes[(*iter).first].at(i).weight == (*iter).second.weight && followingNodes[(*iter).first].at(i).middleNode == (*iter).second.middleNode) {
                    flags += 1;
                    removedEdges.push_back(std::make_pair(std::make_pair((*iter).first, i), PreprocessingEdgeData(followingNodes[(*iter).first].at(i))));
                    followingNodes[(*iter).first].erase(i);
                }
                if ((*iter).second.isShortcut) {
                    flags += 4;
                    shortcuts.push_back(OutputShortcutEdge((*iter).first, i, (*iter).second.weight, flags, (*iter).second.middleNode));
                } else {
                    edges.push_back(OutputEdge((*iter).first, i, (*iter).second.weight, flags));
                }
            }
        }
    }

    for(size_t i = 0; i < removedEdges.size(); i++) {
        followingNodes[removedEdges[i].first.first].insert(std::make_pair(removedEdges[i].first.second, PreprocessingEdgeData(removedEdges[i].second)));
    }
}
