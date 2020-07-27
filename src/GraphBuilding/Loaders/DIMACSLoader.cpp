//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include <fstream>
#include "DIMACSLoader.h"
#include "../../Timer/Timer.h"
#include "../../Error/Error.h"

//______________________________________________________________________________________________________________________
DIMACSLoader::DIMACSLoader(string inputFile) {
    this->inputFile = inputFile;
}

//______________________________________________________________________________________________________________________
Graph * DIMACSLoader::loadGraph() {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes, edges;
    parseGraphProblemLine(input, nodes, edges);

    SimpleGraph * graph = new SimpleGraph(nodes);
    parseEdges(input, *graph, edges);

    Graph * retvalGraph = new Graph(*graph);

    delete graph;

    input.close();

    return retvalGraph;

}

//______________________________________________________________________________________________________________________
UpdateableGraph * DIMACSLoader::loadUpdateableGraph() {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes, edges;
    parseGraphProblemLine(input, nodes, edges);

    UpdateableGraph * graph = new UpdateableGraph(nodes);
    parseEdges(input, *graph, edges);

    input.close();

    return graph;
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::putAllEdgesIntoUpdateableGraph(UpdateableGraph & graph) {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes, edges;
    parseGraphProblemLine(input, nodes, edges);

    parseEdges(input, graph, edges);

    input.close();

}

//______________________________________________________________________________________________________________________
void DIMACSLoader::parseGraphProblemLine(ifstream &input, unsigned int &nodes, unsigned int &edges) {
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
void DIMACSLoader::processGraphProblemLine(string &buffer, unsigned int &nodes, unsigned int &edges) {
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
void DIMACSLoader::parseEdges(ifstream & input, SimpleGraph & graph, unsigned int edges) {
    unsigned int loadededgescnt = 0;
    while (loadededgescnt < edges) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'a') {
            unsigned int from, to;
            unsigned int weight;
            getEdge(buffer, from, to, weight);
            if(from != to) {
                graph.addEdge(from, to, weight);
            }
            loadededgescnt++;
        }
    }
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::parseEdges(ifstream & input, UpdateableGraph & graph, unsigned int edges) {
    unsigned int loadededgescnt = 0;
    while (loadededgescnt < edges) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'a') {
            unsigned int from, to;
            unsigned int weight;
            getEdge(buffer, from, to, weight);
            if(from != to) {
                graph.addEdge(from, to, weight);
            }
            loadededgescnt++;
        }
    }
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::getEdge(string & buffer, unsigned int & from, unsigned int & to, unsigned int & weight) {
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

vector<int> DIMACSLoader::loadAdjacencyMatrix() {
    throw not_implemented_error("loadAdjacencyMatrix() is not implemented for DIMACSLoader class");
}
