//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#ifndef TRANSIT_NODE_ROUTING_LOADER_H
#define TRANSIT_NODE_ROUTING_LOADER_H

#include <string>
#include <map>
#include "Graph.h"
#include "UpdateableGraph.h"
#include "ShrinkingGraph.h"

using namespace std;

// This function is responsible for loading all the input files. It can load graphs, trips, data for Contraction
// Hierarchies (node ranks, unpacking data)...
// For graphs, the 9th DIMACS Implementation Challenge format is used. Information about the format along with some
// sample graphs that can be directly used with this program can be found here:
// http://www.dis.uniroma1.it/challenge9/download.shtml
//______________________________________________________________________________________________________________________
class Loader{
private:
    string inputFile;
    void parseGraphProblemLine(ifstream & input, unsigned int & nodes, unsigned int & edges);
    void parseCoordinatesProblemLine(ifstream & input, unsigned int & nodes);
    void parseEdges(ifstream & input, SimpleGraph & graph, unsigned int edges);
    void parseEdges(ifstream & input, UpdateableGraph & graph, unsigned int edges);
    void parseCHEdges(ifstream & input, Graph & graph, unsigned int edges);
    void parseNodesCoordinates(ifstream & input, vector < pair < int, int > > & x, unsigned int nodes);
    void processGraphProblemLine(string & buffer, unsigned int & nodes, unsigned int & edges);
    void processCoordinatesProblemLine(string & buffer, unsigned int & nodes);
    void getEdge(string & buffer, unsigned int & from, unsigned int & to, long long unsigned int & weight);
    void parseTrips(ifstream & input, vector < pair < unsigned int, unsigned int > > & x);
    void getNodeCoordinates(string & buffer, unsigned int & node, int & coord1, int & coord2);
    void parseShortcuts(ifstream & input, SimpleGraph & graph);
    void parseShortcuts(ifstream & input, UpdateableGraph & graph);
    void parseRanks(ifstream & input, vector < unsigned int > & x);
    void parseUnpackingData(ifstream & input, map < pair < unsigned int, unsigned int >, unsigned int > & x);
    void transformEdges(ifstream & input, ofstream & output, unsigned int edges);
public:
    Loader(string inputFile);
    Graph * loadGraph();
    UpdateableGraph * loadUpdateableGraph();
    ShrinkingGraph * loadCHWithShortcutsIntoShrinkingGraph(string shortcutsFile);
    Graph * loadCHGraph();
    Graph * loadCHGraphWithShortcuts(string shortcutsFile);
    void loadTrips(vector < pair < unsigned int, unsigned int > > & x);
    void loadCoordinates(vector < pair <int, int > > & x);
    void loadRanks(vector < unsigned int > & x);
    void loadUnpackingData(map < pair < unsigned int, unsigned int >, unsigned int > & x);
    void transformToDDSG(string DIMACSfile);
    void putAllEdgesIntoUpdateableGraph(UpdateableGraph & graph);

};

#endif //TRANSIT_NODE_ROUTING_LOADER_H
