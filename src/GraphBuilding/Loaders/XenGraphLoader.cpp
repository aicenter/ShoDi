//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#include "../../Timer/Timer.h"
#include "XenGraphLoader.h"
#include <fstream>
#include <iostream>

//______________________________________________________________________________________________________________________
XenGraphLoader::XenGraphLoader(string inputFile) {
    this->inputFile = inputFile;
}

//______________________________________________________________________________________________________________________
Graph * XenGraphLoader::loadGraph() {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes, edges;
    parseFirstLine(input, nodes, edges);

    SimpleGraph * graph = new SimpleGraph(nodes);
    parseEdges(input, *graph, edges);

    //Graph * retvalGraph = new Graph(*graph);

    delete graph;

    input.close();

    //return retvalGraph;
    return NULL;
}

//______________________________________________________________________________________________________________________
UpdateableGraph * XenGraphLoader::loadUpdateableGraph() {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes, edges;
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
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    parseNodesMapping(input, mapping);

    input.close();
}

//______________________________________________________________________________________________________________________
void XenGraphLoader::putAllEdgesIntoUpdateableGraph(UpdateableGraph & graph) {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes, edges;
    parseFirstLine(input, nodes, edges);

    parseEdges(input, graph, edges);

    input.close();

}

//______________________________________________________________________________________________________________________
void XenGraphLoader::parseFirstLine(ifstream & input, unsigned int & nodes, unsigned int & edges) {
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
void XenGraphLoader::parseEdges(ifstream & input, SimpleGraph & graph, unsigned int edges) {
    unsigned int from, to, oneWayFlag, weight;
    for(unsigned int i = 0; i < edges; i++) {
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
void XenGraphLoader::parseEdges(ifstream & input, UpdateableGraph & graph, unsigned int edges) {
    unsigned int from, to, oneWayFlag, weight;
    for(unsigned int i = 0; i < edges; i++) {
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