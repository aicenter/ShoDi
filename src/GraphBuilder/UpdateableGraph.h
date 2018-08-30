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

using namespace std;

class UpdateableGraph{
protected:
    void prepareEdgesForFlushing(vector < OutputEdge > & edges, vector < OutputShortcutEdge > & shortcuts);
    void flushHeader(ostream & output);
    void flushCnts(ostream & output, const unsigned int nodes, const unsigned int edges, const unsigned int shortcuts);
    void flushRanks(ostream & output);
    void flushOriginalEdges(ostream & output, vector < OutputEdge > & edges);
    void flushShortcutEdges(ostream & output, vector < OutputShortcutEdge > & edges);
    void flushTerminator(ostream & output);
    vector< unordered_map < unsigned int, PreprocessingEdgeData > > followingNodes;
    vector< unordered_map < unsigned int, long long unsigned int > > previousNodes;
    vector< unsigned int > ranks;
public:
    UpdateableGraph(unsigned int n);
    void flushInDdsgFormat(string filePath);
    bool addEdge(unsigned int from, unsigned int to, long long unsigned int weight);
    bool addShortcutEdge(unsigned int from, unsigned int to, long long unsigned int weight, unsigned int middlenode);
    void removeEdge(unsigned int from, unsigned int to);
    void setRank(unsigned int node, unsigned int rank);
    const unsigned int nodes() const;
    const unordered_map<unsigned int, long long unsigned int> & incomingEdges(const unsigned int x)const;
    const unordered_map<unsigned int, PreprocessingEdgeData> & outgoingEdges(const unsigned int x)const;
    const unsigned int degree(unsigned int node)const;
};




#endif //TRANSIT_NODE_ROUTING_UPDATEABLEGRAPH_H
