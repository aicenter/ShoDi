//
// Author: Xenty (Michal Cvach)
// Created on: 29.8.18
//

#ifndef TRANSIT_NODE_ROUTING_PREPROCESSINGEDGE_H
#define TRANSIT_NODE_ROUTING_PREPROCESSINGEDGE_H

// This structure represents one edge during the preprocessing phase. We have to keep track of whether the edge
// is a shortcut or not and if it's a shortcut we also need to know it's middle node.
//______________________________________________________________________________________________________________________
class IntegerPreprocessingEdgeData {
public:
    IntegerPreprocessingEdgeData(const long long unsigned int b, const unsigned int c, const bool d);
    IntegerPreprocessingEdgeData(const IntegerPreprocessingEdgeData & data);
    long long unsigned int weight;
    unsigned int middleNode;
    bool isShortcut;
};


#endif //TRANSIT_NODE_ROUTING_PREPROCESSINGEDGE_H
