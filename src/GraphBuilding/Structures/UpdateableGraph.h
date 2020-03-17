//
// Author: Xenty (Michal Cvach)
// Created on: 21.8.18
//

#ifndef TRANSIT_NODE_ROUTING_UPDATEABLEGRAPH_H
#define TRANSIT_NODE_ROUTING_UPDATEABLEGRAPH_H

#include <vector>
#include <unordered_map>
#include "PreprocessingEdgeData.h"
#include "OutputEdge.h"
#include "OutputShortcutEdge.h"
#include "Graph.h"

using namespace std;

// This class is used to represent the graph during the preprocessing phase. This representation supports pretty simple
// and reasonably quick edge adding and removing. It also keeps track of which edges are shortcut edges and which
// are original edges. The information in this structure can be directly used to generate a .ch file.
//______________________________________________________________________________________________________________________
class UpdateableGraph{
protected:
    void flushHeader(ostream & output);
    void flushCnts(ostream & output, const unsigned int nodes, const unsigned int edges, const unsigned int shortcuts);
    void flushRanks(ostream & output);
    void flushOriginalEdges(ostream & output, vector < OutputEdge > & edges);
    void flushShortcutEdges(ostream & output, vector < OutputShortcutEdge > & edges);
    void flushTerminator(ostream & output);
    vector< unordered_map < unsigned int, PreprocessingEdgeData > > followingNodes;
    vector< unordered_map < unsigned int, unsigned int > > previousNodes;
    vector< unsigned int > ranks;
public:
    Graph * createCopy();
    void prepareEdgesForFlushing(vector < OutputEdge > & edges, vector < OutputShortcutEdge > & shortcuts);
    void prepareEdgesForFlushingWithReinsert(vector < OutputEdge > & edges, vector < OutputShortcutEdge > & shortcuts);
    UpdateableGraph(unsigned int n);
    void flushInDdsgFormat(string filePath);
    bool addEdge(unsigned int from, unsigned int to, unsigned int weight);
    bool addShortcutEdge(unsigned int from, unsigned int to, unsigned int weight, unsigned int middlenode);
    void removeEdge(unsigned int from, unsigned int to);
    void setRank(unsigned int node, unsigned int rank);
    bool isShortcut(const unsigned int from, const unsigned int to);
    void getNodesWithHighestRank(vector< unsigned int > & highestNodes, unsigned int requestedAmount);
    const unsigned int getRank(unsigned int nodeID) const;
    const unsigned int nodes() const;
    const unordered_map<unsigned int, unsigned int> & incomingEdges(const unsigned int x)const;
    const unordered_map<unsigned int, PreprocessingEdgeData> & outgoingEdges(const unsigned int x)const;
    const unsigned int degree(unsigned int node)const;
};




#endif //TRANSIT_NODE_ROUTING_UPDATEABLEGRAPH_H
