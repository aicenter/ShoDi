//
// Author: Xenty (Michal Cvach)
// Created on: 30.8.18
//

#ifndef TRANSIT_NODE_ROUTING_FLAGSGRAPHWITHUNPACKINGDATA_H
#define TRANSIT_NODE_ROUTING_FLAGSGRAPHWITHUNPACKINGDATA_H

#include <map>
#include "FlagsGraph.h"
#include "QueryEdgeWithUnpackingData.h"

/**
 * Class representing the data structure required for the Contraction Hierarchies query algorithm. This structure
 * contains additional information that can be used to obtain actual shortest paths and not just the shortest distances.
 * The downside of this is that the memory required by this class is bigger than in the case of the normal FlagsGraph.
 */
class FlagsGraphWithUnpackingData {
protected:
    vector< vector < QueryEdgeWithUnpackingData > > neighbours;
    vector< NodeData > nodesData;
    vector<unsigned int> forwardPrev;
    vector<unsigned int> backwardPrev;
public:
    FlagsGraphWithUnpackingData(unsigned int n);
    unsigned int getMiddleNode(unsigned int source, unsigned int target, bool direction);
    unsigned int getDistance(unsigned int node1, unsigned int node2, bool direction);
    void setForwardPrev(unsigned int x, unsigned int y);
    void setBackwardPrev(unsigned int x, unsigned int y);
    unsigned int getForwardPrev(unsigned int x);
    unsigned int getBackwardPrev(unsigned int x);
    void resetForwardPrev(unsigned int x);
    void resetBackwardPrev(unsigned int x);
    void addEdge(unsigned int from, unsigned int to, unsigned int weight, bool fw, bool bw, unsigned int mNode = UINT_MAX);
    const unsigned int nodes() const;
    const vector< QueryEdgeWithUnpackingData > & nextNodes(const unsigned int x)const;
    NodeData & data(unsigned int node);
    void resetForwardInfo(const unsigned int node);
    void resetBackwardInfo(const unsigned int node);
    void resetForwardStall(const unsigned int node);
    void resetBackwardStall(const unsigned int node);
};


#endif //TRANSIT_NODE_ROUTING_FLAGSGRAPHWITHUNPACKINGDATA_H
