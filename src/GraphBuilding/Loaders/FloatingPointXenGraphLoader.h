//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_FLOATINGPOINTXENGRAPHLOADER_H
#define CONTRACTION_HIERARCHIES_FLOATINGPOINTXENGRAPHLOADER_H

#include <string>
#include <map>
#include "../Structures/FloatingPointStructures/FPointGraph.h"
#include "../Structures/FloatingPointStructures/FPointSimpleGraph.h"
#include "../Structures/FloatingPointStructures/FPointUpdateableGraph.h"

class FloatingPointXenGraphLoader {
private:
    string inputFile;
    void parseFirstLine(ifstream & input, unsigned int & nodes, unsigned int & edges);
    void parseEdges(ifstream & input, FPointSimpleGraph & graph, unsigned int edges);
    void parseEdges(ifstream & input, FPointUpdateableGraph & graph, unsigned int edges);
    void parseNodesMapping(ifstream & input, unordered_map <long long unsigned int, unsigned int> & mapping);
public:
    FloatingPointXenGraphLoader(string inputFile);
    FPointGraph * loadGraph();
    FPointUpdateableGraph * loadUpdateableGraph();
    void putAllEdgesIntoUpdateableGraph(FPointUpdateableGraph & graph);
    void loadNodesMapping(unordered_map <long long unsigned int, unsigned int> & mapping);
};


#endif //CONTRACTION_HIERARCHIES_FLOATINGPOINTXENGRAPHLOADER_H