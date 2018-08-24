//
// Author: Xenty (Michal Cvach)
// Created on: 23.8.18
//

#ifndef TRANSIT_NODE_ROUTING_DDSGLOADER_H
#define TRANSIT_NODE_ROUTING_DDSGLOADER_H

#include <string>
#include "Graph.h"

using namespace std;

class DDSGLoader {
private:
    string inputFile;
    bool verifyHeader(ifstream & input);
    bool verifyFooter(ifstream & input);
    void loadCnts(ifstream & input, unsigned int & nodes, unsigned int & edges, unsigned int & shortcutEdges);
    void loadRanks(ifstream & input, unsigned int nodes, vector <unsigned int> & ranks);
    void loadOriginalEdges(ifstream & input, unsigned int edges, Graph & graph);
    void loadShortcutEdges(ifstream & input, unsigned int shortcutEdges, Graph & graph);
public:
    DDSGLoader(string inputFile);
    Graph * loadGraphWithRanks(vector <unsigned int> & ranks);
};


#endif //TRANSIT_NODE_ROUTING_DDSGLOADER_H
