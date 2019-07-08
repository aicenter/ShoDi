//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FPOINTUPDATEABLEGRAPH_H
#define CONTRACTION_HIERARCHIES_FPOINTUPDATEABLEGRAPH_H

#include "FPointOutputEdge.h"
#include "FPointOutputShortcutEdge.h"
#include "FPointPreprocessingEdgeData.h"
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// This class is used to represent the graph during the preprocessing phase. This representation supports pretty simple
// and reasonably quick edge adding and removing. It also keeps track of which edges are shortcut edges and which
// are original edges. The information in this structure can be directly used to generate a .ch file.
//______________________________________________________________________________________________________________________
class FPointUpdateableGraph{
protected:
    void prepareEdgesForFlushing(vector < FPointOutputEdge > & edges, vector < FPointOutputShortcutEdge > & shortcuts);
    void flushHeader(ostream & output);
    void flushCnts(ostream & output, const unsigned int nodes, const unsigned int edges, const unsigned int shortcuts);
    void flushRanks(ostream & output);
    void flushOriginalEdges(ostream & output, vector < FPointOutputEdge > & edges);
    void flushShortcutEdges(ostream & output, vector < FPointOutputShortcutEdge > & edges);
    void flushTerminator(ostream & output);
    vector< unordered_map < unsigned int, FPointPreprocessingEdgeData > > followingNodes;
    vector< unordered_map < unsigned int, double > > previousNodes;
    vector< unsigned int > ranks;
public:
    FPointUpdateableGraph(unsigned int n);
    void flushInDdsgfFormat(string filePath);
    bool addEdge(unsigned int from, unsigned int to, double weight);
    bool addShortcutEdge(unsigned int from, unsigned int to, double weight, unsigned int middlenode);
    void removeEdge(unsigned int from, unsigned int to);
    void setRank(unsigned int node, unsigned int rank);
    const unsigned int nodes() const;
    const unordered_map<unsigned int, double> & incomingEdges(const unsigned int x)const;
    const unordered_map<unsigned int, FPointPreprocessingEdgeData> & outgoingEdges(const unsigned int x)const;
    const unsigned int degree(unsigned int node)const;
};

#endif //CONTRACTION_HIERARCHIES_FPOINTUPDATEABLEGRAPH_H
