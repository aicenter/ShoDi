//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#ifndef TRANSIT_NODE_ROUTING_LOADER_H
#define TRANSIT_NODE_ROUTING_LOADER_H

#include <string>
#include "Graph.h"

using namespace std;

class Loader{
private:
    string inputFile;
    void parseProblemLine(ifstream  & input, unsigned int & nodes, unsigned int & edges);
    void parseEdges(ifstream & input, Graph & graph, unsigned int edges);
    void processProblemLine(string & buffer, unsigned int & nodes, unsigned int & edges);
    void getEdge(string & buffer, unsigned int & from, unsigned int & to, unsigned int & weight);
    void parseTrips(ifstream & input, vector < pair < unsigned int, unsigned int > > & x);
public:
    Loader(string inputFile);
    Graph * loadGraph();
    void loadTrips(vector < pair < unsigned int, unsigned int > > & x);
};

#endif //TRANSIT_NODE_ROUTING_LOADER_H
