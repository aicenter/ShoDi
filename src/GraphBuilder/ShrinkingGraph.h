//
// Author: Xenty (Michal Cvach)
// Created on: 22.8.18
//

#ifndef TRANSIT_NODE_ROUTING_SHRINKINGGRAPH_H
#define TRANSIT_NODE_ROUTING_SHRINKINGGRAPH_H

#include <string>
#include "UpdateableGraph.h"

using namespace std;

class ShrinkingGraph : public UpdateableGraph {
public:
    ShrinkingGraph(unsigned int n);
    void removeUnnecesarryEdges(vector<unsigned int> & ranks);
    void flushGraph(string filePath);
private:
    void removeFollowing(vector<unsigned int> & ranks);
    void removePrevious(vector<unsigned int> & ranks);
};


#endif //TRANSIT_NODE_ROUTING_SHRINKINGGRAPH_H
