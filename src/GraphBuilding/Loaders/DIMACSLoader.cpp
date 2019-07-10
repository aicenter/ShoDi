//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include <fstream>
#include "DIMACSLoader.h"
#include "Timer.h"

//______________________________________________________________________________________________________________________
DIMACSLoader::DIMACSLoader(string inputFile) {
    this->inputFile = inputFile;
}

// Function used to load a graph for the Dijkstra's algorithm. One minor improvement is that we first load the edges
// into an 'IntegerSimpleGraph' instance, which automatically removes multiple (parallel) edges and then construct a 'IntegerGraph'
// from that.
//______________________________________________________________________________________________________________________
IntegerGraph * DIMACSLoader::loadGraph() {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    printf("Started loading graph!\n");

    Timer graphLoadTimer("IntegerGraph loading");
    graphLoadTimer.begin();

    unsigned int nodes, edges;
    parseGraphProblemLine(input, nodes, edges);

    IntegerSimpleGraph * graph = new IntegerSimpleGraph(nodes);
    parseEdges(input, *graph, edges);

    IntegerGraph * retvalGraph = new IntegerGraph(*graph);

    delete graph;

    graphLoadTimer.finish();
    graphLoadTimer.printMeasuredTime();

    input.close();

    return retvalGraph;

}

// This function is used to load the graph data into an 'IntegerUpdateableGraph' instance, which can be used for the
// preprocessing to create a Contraction Hierarchy from the input graph.
//______________________________________________________________________________________________________________________
IntegerUpdateableGraph * DIMACSLoader::loadUpdateableGraph() {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    printf("Started loading graph!\n");

    Timer graphLoadTimer("IntegerGraph loading");
    graphLoadTimer.begin();

    unsigned int nodes, edges;
    parseGraphProblemLine(input, nodes, edges);

    IntegerUpdateableGraph * graph = new IntegerUpdateableGraph(nodes);
    parseEdges(input, *graph, edges);

    graphLoadTimer.finish();
    graphLoadTimer.printMeasuredTime();

    input.close();

    return graph;
}

// This function is used to reinsert all the original edges into the graph after the preprocessing has been finished.
//______________________________________________________________________________________________________________________
void DIMACSLoader::putAllEdgesIntoUpdateableGraph(IntegerUpdateableGraph & graph) {
    ifstream input;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    printf("Putting all original edges back into updateable graph!\n");

    Timer graphLoadTimer("Putting back original edges");
    graphLoadTimer.begin();

    unsigned int nodes, edges;
    parseGraphProblemLine(input, nodes, edges);

    parseEdges(input, graph, edges);

    graphLoadTimer.finish();
    graphLoadTimer.printMeasuredTime();

    input.close();

}

// This is used to transorm a file from the DIMACS challenge format to the format used for input graphs in the reference
// implementation. Output of this function isn't used anywhere in this implementation, but can be useful for debug
// purposes.
//______________________________________________________________________________________________________________________
void DIMACSLoader::transformToDDSG(string DIMACSfile) {
    ifstream input;
    ofstream output;
    input.open(this->inputFile);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }
    output.open(DIMACSfile);
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", DIMACSfile.c_str());
    }

    unsigned int nodes, edges;
    parseGraphProblemLine(input, nodes, edges);

    output << "d" << endl;
    output << nodes << " " << edges << endl;

    transformEdges(input, output, edges);

    input.close();
    output.close();
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
void DIMACSLoader::transformEdges(ifstream & input, ofstream & output, unsigned int edges) {
    unsigned int loadededgescnt = 0;
    while (loadededgescnt < edges) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'a') {
            unsigned int from, to;
            long long unsigned int weight;
            getEdge(buffer, from, to, weight);
            output << from << " " << to << " " << weight << " 1" << endl;
            loadededgescnt++;
        }
    }
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::parseEdges(ifstream & input, IntegerSimpleGraph & graph, unsigned int edges) {
    unsigned int loadededgescnt = 0;
    while (loadededgescnt < edges) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'a') {
            unsigned int from, to;
            long long unsigned int weight;
            getEdge(buffer, from, to, weight);
            if(from != to) {
                graph.addEdge(from, to, weight);
            }
            loadededgescnt++;
        }
    }
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::parseEdges(ifstream & input, IntegerUpdateableGraph & graph, unsigned int edges) {
    unsigned int loadededgescnt = 0;
    while (loadededgescnt < edges) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'a') {
            unsigned int from, to;
            long long unsigned int weight;
            getEdge(buffer, from, to, weight);
            if(from != to) {
                graph.addEdge(from, to, weight);
            }
            loadededgescnt++;
        }
    }
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::getEdge(string & buffer, unsigned int & from, unsigned int & to, long long unsigned int & weight) {
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
