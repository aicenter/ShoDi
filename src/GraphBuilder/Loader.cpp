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

    printf("Started loading graph!\n");

    Timer graphLoadTimer("Graph loading");
    graphLoadTimer.begin();

    unsigned int nodes, edges;
    parseGraphProblemLine(input, nodes, edges);
    //printf("Nodes: %u, edges: %u.\n", nodes, edges);

    Graph * graph = new Graph(nodes);
    parseEdges(input, *graph, edges);

    graphLoadTimer.finish();
    graphLoadTimer.printMeasuredTime();

    input.close();

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

    input.close();
}

//______________________________________________________________________________________________________________________
void Loader::loadCoordinates(vector < pair < int, int > > & x) {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes;
    parseCoordinatesProblemLine(input, nodes);
    x.resize(nodes);
    parseNodesCoordinates(input, x, nodes);

    input.close();
}

//______________________________________________________________________________________________________________________
void Loader::loadRanks(vector < unsigned int > & x) {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    parseRanks(input, x);

    input.close();
}

//______________________________________________________________________________________________________________________
void Loader::parseGraphProblemLine(ifstream &input, unsigned int &nodes, unsigned int &edges) {
    while (true) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'p') {
            processGraphProblemLine(buffer, nodes, edges);
            return;
        }
    }
}

//______________________________________________________________________________________________________________________
void Loader::processGraphProblemLine(string &buffer, unsigned int &nodes, unsigned int &edges) {
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
            unsigned int from, to;
            long long unsigned int weight;
            getEdge(buffer, from, to, weight);
            graph.addEdge(from, to, weight);
            //printf("Got edge: %u -> %u, weight: %u.\n", from, to, weight);
            loadededgescnt++;
        }
    }
}

//______________________________________________________________________________________________________________________
void Loader::getEdge(string & buffer, unsigned int & from, unsigned int & to, long long unsigned int & weight) {
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
    long long unsigned int tmpweight = 0;
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

//______________________________________________________________________________________________________________________
void Loader::parseRanks(ifstream & input, vector < unsigned int > & x) {
    unsigned int nodes;
    input >> nodes;
    x.resize(nodes);
    for (unsigned int i = 0; i < nodes; i++) {
        input >> x[i];
    }
}

//______________________________________________________________________________________________________________________
void Loader::parseCoordinatesProblemLine(ifstream & input, unsigned int & nodes) {
    while (true) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'p') {
            processCoordinatesProblemLine(buffer, nodes);
            return;
        }
    }
}

//______________________________________________________________________________________________________________________
void Loader::parseNodesCoordinates(ifstream & input, vector < pair < int, int > > & x, unsigned int nodes) {
    unsigned int loadednodescnt = 0;
    while (loadednodescnt < nodes) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'v') {
            unsigned int node;
            int coord1, coord2;
            getNodeCoordinates(buffer, node, coord1, coord2);
            x.at(node).first = coord1;
            x.at(node).second = coord2;
            loadednodescnt++;
        }
    }
}

//______________________________________________________________________________________________________________________
void Loader::processCoordinatesProblemLine(string & buffer, unsigned int & nodes) {
    unsigned int position = 12;
    unsigned int tmpnodes = 0;
    while (position < buffer.size()) {
        tmpnodes *= 10;
        tmpnodes += (buffer[position] - 48);
        position++;
    }

    nodes = tmpnodes;
}

//______________________________________________________________________________________________________________________
void Loader::getNodeCoordinates(string & buffer, unsigned int & node, int & coord1, int & coord2) {
    unsigned int position = 2;
    unsigned int tmpnode = 0;
    while (buffer[position] != ' ') {
        tmpnode *= 10;
        tmpnode += (buffer[position] - 48);
        position++;
    }

    position++;
    int tmpcoord1 = 0;
    bool coord1lessthanzero;
    if (buffer[position] == '-') {
        coord1lessthanzero = true;
        position++;
    } else {
        coord1lessthanzero = false;
    }
    while(buffer[position] != ' ') {
        tmpcoord1 *= 10;
        tmpcoord1 += (buffer[position] - 48);
        position++;
    }

    position++;
    int tmpcoord2 = 0;
    bool coord2lessthanzero;
    if (buffer[position] == '-') {
        coord2lessthanzero = true;
        position++;
    } else {
        coord2lessthanzero = false;
    }
    while(position < buffer.size()) {
        tmpcoord2 *= 10;
        tmpcoord2 += (buffer[position] - 48);
        position++;
    }

    if(coord1lessthanzero) {
        tmpcoord1 = -tmpcoord1;
    }
    if(coord2lessthanzero) {
        tmpcoord2 = -tmpcoord2;
    }

    node = tmpnode - 1;
    coord1 = tmpcoord1;
    coord2 = tmpcoord2;
}