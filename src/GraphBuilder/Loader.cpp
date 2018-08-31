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

// Function used to load a graph for the Dijkstra's algorithm. One minor improvement is that we first load the edges
// into an 'SimpleGraph' instance, which automatically removes multiple (parallel) edges and then construct a 'Graph'
// from that.
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

    SimpleGraph * graph = new SimpleGraph(nodes);
    parseEdges(input, *graph, edges);

    Graph * retvalGraph = new Graph(*graph);

    delete graph;

    graphLoadTimer.finish();
    graphLoadTimer.printMeasuredTime();

    input.close();

    return retvalGraph;

}

// This function is used to load the graph data into an 'UpdateableGraph' instance, which can be used for the
// preprocessing to create a Contraction Hierarchy from the input graph.
//______________________________________________________________________________________________________________________
UpdateableGraph * Loader::loadUpdateableGraph() {
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

    UpdateableGraph * graph = new UpdateableGraph(nodes);
    parseEdges(input, *graph, edges);

    graphLoadTimer.finish();
    graphLoadTimer.printMeasuredTime();

    input.close();

    return graph;
}

// This function is used to reinsert all the original edges into the graph after the preprocessing has been finished.
//______________________________________________________________________________________________________________________
void Loader::putAllEdgesIntoUpdateableGraph(UpdateableGraph & graph) {
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
void Loader::transformToDDSG(string DIMACSfile) {
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

// This function just loads trips in a simple format.
// The trips file should be a text file starting with an unsigned int with the number of trips "n" and then
// "n" rows should follow each row containing two integers "source" and "target" separated by a space.
// This function doesn't validate that the sources and targets are valid (that means that they are lower number than
// the amount of edges in the used graph), it just loads the pairs. Do any validations yourself, if necessary.
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
void Loader::transformEdges(ifstream & input, ofstream & output, unsigned int edges) {
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
void Loader::parseEdges(ifstream & input, SimpleGraph & graph, unsigned int edges) {
    unsigned int loadededgescnt = 0;
    while (loadededgescnt < edges) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'a') {
            unsigned int from, to;
            long long unsigned int weight;
            getEdge(buffer, from, to, weight);
            graph.addEdge(from, to, weight);
            loadededgescnt++;
        }
    }
}

//______________________________________________________________________________________________________________________
void Loader::parseEdges(ifstream & input, UpdateableGraph & graph, unsigned int edges) {
    unsigned int loadededgescnt = 0;
    while (loadededgescnt < edges) {
        string buffer;
        getline(input, buffer);
        if (buffer[0] == 'a') {
            unsigned int from, to;
            long long unsigned int weight;
            getEdge(buffer, from, to, weight);
            graph.addEdge(from, to, weight);
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
