//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#ifndef TRANSIT_NODE_ROUTING_LOADER_H
#define TRANSIT_NODE_ROUTING_LOADER_H

#include <string>
#include <map>
#include "Structures/IntegerStructures/IntegerGraph.h"
#include "Structures/IntegerStructures/IntegerUpdateableGraph.h"

using namespace std;

// This function is responsible for loading all the input files. It can load graphs, trips, data for Contraction
// Hierarchies (node ranks, unpacking data)...
// For graphs, the 9th DIMACS Implementation Challenge format is used. Information about the format along with some
// sample graphs that can be directly used with this program can be found here:
// http://www.dis.uniroma1.it/challenge9/download.shtml
//______________________________________________________________________________________________________________________
class DIMACSLoader{
private:
    string inputFile;
    void parseGraphProblemLine(ifstream & input, unsigned int & nodes, unsigned int & edges);
    void parseEdges(ifstream & input, IntegerSimpleGraph & graph, unsigned int edges);
    void parseEdges(ifstream & input, IntegerUpdateableGraph & graph, unsigned int edges);
    void processGraphProblemLine(string & buffer, unsigned int & nodes, unsigned int & edges);
    void getEdge(string & buffer, unsigned int & from, unsigned int & to, long long unsigned int & weight);
    void transformEdges(ifstream & input, ofstream & output, unsigned int edges);
public:
    DIMACSLoader(string inputFile);
    IntegerGraph * loadGraph();
    IntegerUpdateableGraph * loadUpdateableGraph();
    void transformToDDSG(string DIMACSfile);
    void putAllEdgesIntoUpdateableGraph(IntegerUpdateableGraph & graph);
};

#endif //TRANSIT_NODE_ROUTING_LOADER_H
