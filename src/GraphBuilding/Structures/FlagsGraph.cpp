//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#include <climits>
#include "FlagsGraph.h"

//______________________________________________________________________________________________________________________
FlagsGraph::FlagsGraph(unsigned int n) {
    neighbours.resize(n);
    nodesData.resize(n);
}

// Build a FlagsGraph based on an instance of UpdateableGraph. This is intented to be used with an UpdateableGraph that
// was already processed using the CHPreprocessor. This initializes a FlagsGraph which can be used for further CH
// queries without the need to flush the CH into a file during the process.
//______________________________________________________________________________________________________________________
FlagsGraph::FlagsGraph(UpdateableGraph & g) {
    unsigned int nodesCnt = g.nodes();
    neighbours.resize(nodesCnt);
    nodesData.resize(nodesCnt);

    for(unsigned int i = 0; i < nodesCnt; i++) {
        data(i).rank = g.getRank(i);
    }

    vector < OutputEdge > edges;
    vector < OutputShortcutEdge > shortcuts;
    g.prepareEdgesForFlushingWithReinsert(edges, shortcuts);

    processOriginalEdges(edges);
    processShortcuts(shortcuts);
}

//______________________________________________________________________________________________________________________
void FlagsGraph::getEdgesForFlushing(vector < pair < unsigned int, QueryEdge > > & allEdges) {
    for(unsigned int i = 0; i < neighbours.size(); i++) {
        for(unsigned int j = 0; j < neighbours[i].size(); j++) {
            allEdges.push_back(make_pair(i, QueryEdge(neighbours[i][j])));
        }
    }
}

//______________________________________________________________________________________________________________________
void FlagsGraph::processOriginalEdges(vector < OutputEdge > & edges) {
    for(unsigned int i = 0; i < edges.size(); i++) {
        unsigned int from, to, weight, flags;
        from = edges[i].sourceNode;
        to = edges[i].targetNode;
        weight = edges[i].weight;
        flags = edges[i].flags;

        bool forward = false;
        bool backward = false;
        if((flags & 1) == 1) {
            forward = true;
        }
        if((flags & 2) == 2) {
            backward = true;
        }
        if ( data(from).rank < data(to).rank ) {
            addEdge(from, to, weight, forward, backward);
        } else {
            addEdge(to, from, weight, forward, backward);
        }
    }
}

//______________________________________________________________________________________________________________________
void FlagsGraph::processShortcuts( vector < OutputShortcutEdge > & shortcuts) {
    for(unsigned int i = 0; i < shortcuts.size(); i++) {
        unsigned int from, to, weight, flags;
        from = shortcuts[i].sourceNode;
        to = shortcuts[i].targetNode;
        weight = shortcuts[i].weight;
        flags = shortcuts[i].flags;

        bool forward = false;
        bool backward = false;
        if((flags & 1) == 1) {
            forward = true;
        }
        if((flags & 2) == 2) {
            backward = true;
        }
        if ( data(from).rank < data(to).rank ) {
            addEdge(from, to, weight, forward, backward);
        } else {
            addEdge(to, from, weight, forward, backward);
        }
    }
}

//______________________________________________________________________________________________________________________
void FlagsGraph::addEdge(unsigned int from, unsigned int to, unsigned int weight, bool fw, bool bw) {
    neighbours.at(from).push_back(QueryEdge(to, weight, fw, bw));
}

//______________________________________________________________________________________________________________________
const unsigned int FlagsGraph::nodes() const {
    return neighbours.size();
}

// Returns all neighbours of a given node 'x'. Keep in mind that this returns neighbour both in the forward and
// backward direction, so we always have to check if the neighbour is in the correct direction when expanding a node.
//______________________________________________________________________________________________________________________
const vector< QueryEdge > & FlagsGraph::nextNodes(const unsigned int x)const {
    return neighbours.at(x);
}

// Returns the data for a certain node.
//______________________________________________________________________________________________________________________
NodeData & FlagsGraph::data(unsigned int node) {
    return nodesData[node];
}

//______________________________________________________________________________________________________________________
void FlagsGraph::resetForwardInfo(const unsigned int node) {
    nodesData[node].forwardDist = UINT_MAX;
    nodesData[node].forwardSettled = false;
    nodesData[node].forwardReached = false;

}

//______________________________________________________________________________________________________________________
void FlagsGraph::resetBackwardInfo(const unsigned int node) {
    nodesData[node].backwardDist = UINT_MAX;
    nodesData[node].backwardSettled = false;
    nodesData[node].backwardReached = false;
}

//______________________________________________________________________________________________________________________
void FlagsGraph::resetForwardStall(const unsigned int node) {
    nodesData[node].forwardStalled = false;
}

//______________________________________________________________________________________________________________________
void FlagsGraph::resetBackwardStall(const unsigned int node) {
    nodesData[node].backwardStalled = false;
}