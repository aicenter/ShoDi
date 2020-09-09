//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include <boost/numeric/conversion/cast.hpp>
#include <climits>
#include <fstream>
#include <memory>
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
        throw runtime_error(string("Couldn't open file '") + this->inputFile + "'!");
    }

    unsigned int nodes;
    size_t edges;
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
        throw runtime_error(string("Couldn't open file '") + this->inputFile + "'!");
    }

    unsigned int nodes;
    size_t edges;
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
        throw runtime_error(string("Couldn't open file '") + this->inputFile + "'!");
    }

    unsigned int nodes;
    size_t edges;
    parseGraphProblemLine(input, nodes, edges);

    parseEdges(input, graph, edges);

    input.close();

}

//______________________________________________________________________________________________________________________
void DIMACSLoader::parseGraphProblemLine(ifstream &input, unsigned int &nodes, size_t &edges) {
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
void DIMACSLoader::processGraphProblemLine(string &buffer, unsigned int &nodes, size_t &edges) {
    size_t position = 5;
    unsigned int tmpnodes = 0;
    while (buffer[position] != ' ') {
        tmpnodes *= 10;
        tmpnodes += (buffer[position] - 48);
        position++;
    }

    position++;
    size_t tmpedges = 0;
    while(position < buffer.size()) {
        tmpedges *= 10;
        tmpedges += (buffer[position] - 48);
        position++;
    }

    nodes = tmpnodes;
    edges = tmpedges;
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::parseEdges(ifstream & input, SimpleGraph & graph, size_t edges) {
    size_t loadededgescnt = 0;
    while (loadededgescnt < edges) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'a') {
            unsigned int from, to;
            dist_t weight;
            getEdge(buffer, from, to, weight);
            if(from != to) {
                graph.addEdge(from, to, weight);
            }
            loadededgescnt++;
        }
    }
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::parseEdges(ifstream & input, UpdateableGraph & graph, size_t edges) {
    size_t loadededgescnt = 0;
    while (loadededgescnt < edges) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'a') {
            unsigned int from, to;
            dist_t weight;
            getEdge(buffer, from, to, weight);
            if(from != to) {
                graph.addEdge(from, to, weight);
            }
            loadededgescnt++;
        }
    }
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::getEdge(string & buffer, unsigned int & from, unsigned int & to, dist_t & weight) {
    size_t position = 2;
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
    dist_t tmpweight = 0;
    while(position < buffer.size()) {
        tmpweight *= 10;
        tmpweight += (buffer[position] - 48);
        position++;
    }

    from = tmpfrom - 1;
    to = tmpto - 1;
    weight = tmpweight;
}


vector<dist_t> DIMACSLoader::loadAdjacencyMatrix() {
    // TODO can be done faster - without first making it into a graph and then copying to adjacency matrix

    std::unique_ptr<Graph> graph {loadGraph()};

    const auto nodes = graph->nodes();
    vector<dist_t> adj(((size_t) nodes) * ((size_t) nodes), std::numeric_limits<dist_t>::max());

    for(unsigned int i = 0; i < nodes; ++i) {
        for(auto &pair : graph->outgoingEdges(i)) {
            adj[((size_t) i) * ((size_t) nodes) + ((size_t) pair.first)] = pair.second;
        }
    }

    return adj;
}
