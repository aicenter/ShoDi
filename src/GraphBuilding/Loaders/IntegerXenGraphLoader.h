//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#ifndef CONTRACTION_HIERARCHIES_INTEGERXENGRAPHLOADER_H
#define CONTRACTION_HIERARCHIES_INTEGERXENGRAPHLOADER_H

#include <string>
#include <map>
#include "../Structures/IntegerStructures/IntegerGraph.h"
#include "../Structures/IntegerStructures/IntegerSimpleGraph.h"
#include "../Structures/IntegerStructures/IntegerUpdateableGraph.h"

class IntegerXenGraphLoader {
private:
    string inputFile;
    void parseFirstLine(ifstream & input, unsigned int & nodes, unsigned int & edges);
    void parseEdges(ifstream & input, IntegerSimpleGraph & graph, unsigned int edges);
    void parseEdges(ifstream & input, IntegerUpdateableGraph & graph, unsigned int edges);
    void parseNodesMapping(ifstream & input, unordered_map <long long unsigned int, unsigned int> & mapping);
public:
    IntegerXenGraphLoader(string inputFile);
    IntegerGraph * loadGraph();
    IntegerUpdateableGraph * loadUpdateableGraph();
    void putAllEdgesIntoUpdateableGraph(IntegerUpdateableGraph & graph);
    void loadNodesMapping(unordered_map <long long unsigned int, unsigned int> & mapping);
};


#endif //CONTRACTION_HIERARCHIES_INTEGERXENGRAPHLOADER_H
