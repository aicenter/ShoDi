//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_XENGRAPHLOADER_H
#define CONTRACTION_HIERARCHIES_XENGRAPHLOADER_H

#include <string>
#include <map>
#include "../Structures/FloatingPointStructures/FPointGraph.h"
#include "../Structures/FloatingPointStructures/FPointSimpleGraph.h"
#include "../Structures/FloatingPointStructures/FPointUpdateableGraph.h"

class XenGraphLoader {
private:
    string inputFile;
    void parseFirstLine(ifstream & input, unsigned int & nodes, unsigned int & edges);
    void parseEdges(ifstream & input, FPointSimpleGraph & graph, unsigned int edges);
    void parseEdges(ifstream & input, FPointUpdateableGraph & graph, unsigned int edges);
public:
    XenGraphLoader(string inputFile);
    FPointGraph * loadGraph();
    FPointUpdateableGraph * loadUpdateableGraph();
    void putAllEdgesIntoUpdateableGraph(FPointUpdateableGraph & graph);
};


#endif //CONTRACTION_HIERARCHIES_XENGRAPHLOADER_H
