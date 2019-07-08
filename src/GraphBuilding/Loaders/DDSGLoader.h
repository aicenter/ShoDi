//
// Author: Xenty (Michal Cvach)
// Created on: 23.8.18
//

#ifndef TRANSIT_NODE_ROUTING_DDSGLOADER_H
#define TRANSIT_NODE_ROUTING_DDSGLOADER_H

#include <string>
#include "Structures/IntegerStructures/IntegerGraph.h"
#include "Structures/IntegerStructures/IntegerFlagsGraph.h"
#include "Structures/IntegerStructures/IntegerFlagsGraphWithUnpackingData.h"

using namespace std;

// This class is responsible for loading the Contraction Hierarchy files. To save the Contraction Hierarchies, we use
// the binary format used by the Karlsruhe researchers in their implementation.
// The format is described at the bottom of this file.
//______________________________________________________________________________________________________________________
class DDSGLoader {
private:
    string inputFile;
    bool verifyHeader(ifstream & input);
    bool verifyFooter(ifstream & input);
    void loadCnts(ifstream & input, unsigned int & nodes, unsigned int & edges, unsigned int & shortcutEdges);
    void loadRanks(ifstream & input, unsigned int nodes, IntegerFlagsGraph & graph);
    void loadOriginalEdges(ifstream & input, unsigned int edges, IntegerFlagsGraph & graph);
    void loadShortcutEdges(ifstream & input, unsigned int shortcutEdges, IntegerFlagsGraph & graph);
    void loadShortcutEdgesWithUnpackingData(ifstream & input, unsigned int shortcutEdges, IntegerFlagsGraphWithUnpackingData & graph);
public:
    DDSGLoader(string inputFile);
    IntegerFlagsGraph * loadFlagsGraph();
    IntegerFlagsGraphWithUnpackingData * loadFlagsGraphWithUnpackingData();
};

// ~~~ DESCRIPTION OF THE CH FORMAT ~~~
// - a binary file, 32-bit-interger organized
// - layout:
//    "CH\r\n" (0x32 0x48 0x0d 0x0a)
//    unsigned int: version (currently "1", shold be == compared)
//    unsigned int: number of nodes (= n)
//    unsigned int: number of original edges (= m1)
//    unsigned int: number of shortcut edges (= m2)
//    n times, for each node 0..(n-1):
//        unsigned int: level
//    m1 times, original edges:
//        unsigned int: source node
//        unsigned int: target node
//        unsigned int: weight
//        unsigned int: flags
//    m2 times, shortcut edges:
//        unsigned int: source node
//        unsigned int: target node
//        unsigned int: weight
//        unsigned int: flags
//        unsigned int: shortcut middle node
//    unsigned int: 0x12345678 as terminator
// - possible (bit) flags are:
//    1 = forward edge
//    2 = backward edge
//    4 = shortcut edge

#endif //TRANSIT_NODE_ROUTING_DDSGLOADER_H
