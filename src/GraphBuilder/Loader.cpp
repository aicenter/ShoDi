//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include <fstream>
#include "Loader.h"
#include "../Timer/Timer.h"

//______________________________________________________________________________________________________________________
Loader::Loader(string inputFile) {
    this->inputFile = inputFile;
}

//______________________________________________________________________________________________________________________
Graph * Loader::loadGraph() {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    Timer graphLoadTimer("Graph loading");
    graphLoadTimer.begin();

    unsigned int nodes, edges;
    parseProblemLine(input, nodes, edges);
    //printf("Nodes: %u, edges: %u.\n", nodes, edges);

    Graph * graph = new Graph(nodes);
    parseEdges(input, *graph, edges);

    graphLoadTimer.finish();
    graphLoadTimer.printMeasuredTime();

    return graph;

}

//______________________________________________________________________________________________________________________
void Loader::loadTrips(vector < pair < unsigned int, unsigned int > > & x) {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    parseTrips(input, x);
}

//______________________________________________________________________________________________________________________
void Loader::parseProblemLine(ifstream  & input, unsigned int & nodes, unsigned int & edges) {
    while (true) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'p') {
            processProblemLine(buffer, nodes, edges);
            return;
        }
    }
}

//______________________________________________________________________________________________________________________
void Loader::processProblemLine(string & buffer, unsigned int & nodes, unsigned int & edges) {
    unsigned int position = 5;
    unsigned int tmpnodes = 0;
    while (buffer[position] != ' ') {
        tmpnodes *= 10;
        tmpnodes += (buffer[position] - 48);
        position++;
    }

    position++;
    unsigned int tmpedges = 0;
    while(position < buffer.size()) {
        tmpedges *= 10;
        tmpedges += (buffer[position] - 48);
        position++;
    }

    nodes = tmpnodes;
    edges = tmpedges;
}

//______________________________________________________________________________________________________________________
void Loader::parseEdges(ifstream & input, Graph & graph, unsigned int edges) {
    unsigned int loadededgescnt = 0;
    while (loadededgescnt < edges) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'a') {
            unsigned int from, to, weight;
            getEdge(buffer, from, to, weight);
            graph.addEdge(from, to, weight);
            //printf("Got edge: %u -> %u, weight: %u.\n", from, to, weight);
            loadededgescnt++;
        }
    }
}

//______________________________________________________________________________________________________________________
void Loader::getEdge(string & buffer, unsigned int & from, unsigned int & to, unsigned int & weight) {
    unsigned int position = 2;
    unsigned int tmpfrom = 0;
    while (buffer[position] != ' ') {
        tmpfrom *= 10;
        tmpfrom += (buffer[position] - 48);
        position++;
    }

    position++;
    unsigned int tmpto = 0;
    while(buffer[position] != ' ') {
        tmpto *= 10;
        tmpto += (buffer[position] - 48);
        position++;
    }

    position++;
    unsigned int tmpweight = 0;
    while(position < buffer.size()) {
        tmpweight *= 10;
        tmpweight += (buffer[position] - 48);
        position++;
    }

    from = tmpfrom - 1;
    to = tmpto - 1;
    weight = tmpweight;
}

//______________________________________________________________________________________________________________________
void Loader::parseTrips(ifstream & input, vector < pair < unsigned int, unsigned int > > & x) {
    unsigned int tripscnt;
    input >> tripscnt;
    x.resize(tripscnt);
    for (unsigned int i = 0; i < tripscnt; i++) {
        input >> x.at(i).first >> x.at(i).second;
    }
}