//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#include "../../Timer/Timer.h"
#include "XenGraphLoader.h"
#include "../../Error/Error.h"
#include <fstream>
#include <climits>
#include <iostream>
#include <limits>
#include <memory>

//______________________________________________________________________________________________________________________
XenGraphLoader::XenGraphLoader(string inputFile) {
    this->inputFile = inputFile;
}

//______________________________________________________________________________________________________________________
Graph * XenGraphLoader::loadGraph() {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        throw runtime_error(string("Couldn't open file '") + this->inputFile + "'!");
    }

    unsigned int nodes;
    size_t edges;
    parseFirstLine(input, nodes, edges);

    SimpleGraph * graph = new SimpleGraph(nodes);
    parseEdges(input, *graph, edges);

    Graph * retvalGraph = new Graph(*graph);

    delete graph;

    input.close();

    return retvalGraph;
}

//______________________________________________________________________________________________________________________
UpdateableGraph * XenGraphLoader::loadUpdateableGraph() {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        throw runtime_error(string("Couldn't open file '") + this->inputFile + "'!");
    }

    unsigned int nodes;
    size_t edges;
    parseFirstLine(input, nodes, edges);

    UpdateableGraph * graph = new UpdateableGraph(nodes);
    parseEdges(input, *graph, edges);

    input.close();

    return graph;
}

//______________________________________________________________________________________________________________________
void XenGraphLoader::loadNodesMapping(unordered_map <long long unsigned int, unsigned int> & mapping) {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        throw runtime_error(string("Couldn't open file '") + this->inputFile + "'!");
    }

    parseNodesMapping(input, mapping);

    input.close();
}

//______________________________________________________________________________________________________________________
void XenGraphLoader::putAllEdgesIntoUpdateableGraph(UpdateableGraph & graph) {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        throw runtime_error(string("Couldn't open file '") + this->inputFile + "'!");
    }

    unsigned int nodes;
    size_t edges;
    parseFirstLine(input, nodes, edges);

    parseEdges(input, graph, edges);

    input.close();

}

//______________________________________________________________________________________________________________________
void XenGraphLoader::parseFirstLine(ifstream & input, unsigned int & nodes, size_t & edges) {
    char c1, c2, c3;
    input >> c1 >> c2 >> c3;
    if (c1 != 'X' || c2 != 'G' || c3 != 'I') {
        cout << "The input file is missing the XenGraph header." << endl
             << "Are you sure the input file is in the correct format?" << endl
             << "The loading will proceed but the loaded graph might be corrupted." << endl;
    }

    input >> nodes >> edges;
}

//______________________________________________________________________________________________________________________
void XenGraphLoader::parseEdges(ifstream & input, SimpleGraph & graph, size_t edges) {
    unsigned int from, to, oneWayFlag, weight;
    for(size_t i = 0; i < edges; i++) {
        input >> from >> to >> weight >> oneWayFlag;
        if (from != to) {
            if (oneWayFlag == 1) {
                graph.addEdge(from, to, weight);
            } else {
                graph.addEdge(from, to, weight);
                graph.addEdge(to, from, weight);
            }
        }
    }
}

//______________________________________________________________________________________________________________________
void XenGraphLoader::parseEdges(ifstream & input, UpdateableGraph & graph, size_t edges) {
    unsigned int from, to, oneWayFlag, weight;
    for(size_t i = 0; i < edges; i++) {
        input >> from >> to >> weight >> oneWayFlag;
        if (from != to) {
            if (oneWayFlag == 1) {
                graph.addEdge(from, to, weight);
            } else {
                graph.addEdge(from, to, weight);
                graph.addEdge(to, from, weight);
            }
        }
    }
}

//______________________________________________________________________________________________________________________
void XenGraphLoader::parseNodesMapping(ifstream & input, unordered_map <long long unsigned int, unsigned int> & mapping) {
    unsigned int nodes;
    long long unsigned int cur;

    char c1, c2, c3;
    input >> c1 >> c2 >> c3;
    if (c1 != 'X' || c2 != 'I' || c3 != 'D') {
        cout << "The input file is missing the XenGraph indices file header." << endl
             << "Are you sure the input file is in the correct format?" << endl
             << "The loading will proceed but the mapping might be corrupted." << endl;
    }

    input >> nodes;

    for(unsigned int i = 0; i < nodes; i++) {
        input >> cur;
        mapping.insert(make_pair(cur, i));
    }

}

vector<dist_t> XenGraphLoader::loadAdjacencyMatrix() {
    // TODO can be done faster - without first making it into a graph and then copying to adjacency matrix

    std::unique_ptr<Graph> graph {loadGraph()};

    const size_t nodes = graph->nodes();
    vector<dist_t> adj(nodes * nodes, std::numeric_limits<dist_t>::max());

    for(size_t i = 0; i < nodes; ++i) {
        for(auto &pair : graph->outgoingEdges(i)) {
            adj[i * nodes + pair.first] = pair.second;
        }
    }

    return adj;
}
