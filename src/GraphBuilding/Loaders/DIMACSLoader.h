//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#ifndef TRANSIT_NODE_ROUTING_LOADER_H
#define TRANSIT_NODE_ROUTING_LOADER_H

#include <string>
#include <map>
#include "../Structures/IntegerGraph.h"
#include "../Structures/IntegerUpdateableGraph.h"

using namespace std;

// This class is responsible for loading input files in the DIMACS format. This means either loading the graph into
// an instance of IntegerGraph, which can be used with the Dijkstra algorithm, or an instance of IntegerUpdateableGraph,
// from which the Contraction Hierarchy can be created. This class also allows to transform DIMACS files into DDSG
// format which is used in the reference Contraction Hierarchies implementation.
//
// By DIMACS format I mean the 9th DIMACS Implementation Challenge format. Information about the format along with some
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
