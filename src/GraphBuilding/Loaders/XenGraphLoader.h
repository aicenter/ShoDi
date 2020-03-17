//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#ifndef CONTRACTION_HIERARCHIES_XENGRAPHLOADER_H
#define CONTRACTION_HIERARCHIES_XENGRAPHLOADER_H

#include <string>
#include <map>
#include "../Structures/Graph.h"
#include "../Structures/SimpleGraph.h"
#include "../Structures/UpdateableGraph.h"

// Loader used for loading integer graphs in the XenGraph format. The format is a really simple text format and is
// described in the README.md of this project. This loader allows the loading into an Graph, which can be used
// for the Dijkstra implementation, UpdateableGraph, which is used to create a Contraction Hierarchy,
// and additionally also can read the XenGraph indices mapping files, which must be used when the user wants to query
// using the original indices from the .geojson files.
//______________________________________________________________________________________________________________________
class XenGraphLoader {
private:
    string inputFile;
    void parseFirstLine(ifstream & input, unsigned int & nodes, unsigned int & edges);
    void parseEdges(ifstream & input, SimpleGraph & graph, unsigned int edges);
    void parseEdges(ifstream & input, UpdateableGraph & graph, unsigned int edges);
    void parseNodesMapping(ifstream & input, unordered_map <long long unsigned int, unsigned int> & mapping);
public:
    XenGraphLoader(string inputFile);
    Graph * loadGraph();
    UpdateableGraph * loadUpdateableGraph();
    void putAllEdgesIntoUpdateableGraph(UpdateableGraph & graph);
    void loadNodesMapping(unordered_map <long long unsigned int, unsigned int> & mapping);
};


#endif //CONTRACTION_HIERARCHIES_XENGRAPHLOADER_H
