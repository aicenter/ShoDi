//
// Author: Xenty (Michal Cvach)
// Created on: 29.8.18
//

#ifndef TRANSIT_NODE_ROUTING_PREPROCESSINGEDGE_H
#define TRANSIT_NODE_ROUTING_PREPROCESSINGEDGE_H


class PreprocessingEdgeData {
public:
    PreprocessingEdgeData(const long long unsigned int b, const unsigned int c, const bool d);
    long long unsigned int weight;
    unsigned int middleNode;
    bool isShortcut;
};


#endif //TRANSIT_NODE_ROUTING_PREPROCESSINGEDGE_H
