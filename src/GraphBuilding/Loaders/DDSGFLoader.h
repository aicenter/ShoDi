//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#ifndef CONTRACTION_HIERARCHIES_DDSGFLOADER_H
#define CONTRACTION_HIERARCHIES_DDSGFLOADER_H

#include "../Structures/FloatingPointStructures/FPointFlagsGraphWithUnpackingData.h"
#include <string>


using namespace std;

// This class is responsible for loading the Floating Point Contraction Hierarchy files.
// To save the Contraction Hierarchies, we use a format similar to the binary format used by the Karlsruhe researchers
// in their implementation. The format is described at the bottom of this file.
//______________________________________________________________________________________________________________________
class DDSGFLoader {
private:
    string inputFile;
    bool verifyHeader(ifstream & input);
    bool verifyFooter(ifstream & input);
    void loadCnts(ifstream & input, unsigned int & nodes, unsigned int & edges, unsigned int & shortcutEdges);
    void loadRanks(ifstream & input, unsigned int nodes, FPointFlagsGraph & graph);
    void loadOriginalEdges(ifstream & input, unsigned int edges, FPointFlagsGraph & graph);
    void loadShortcutEdges(ifstream & input, unsigned int shortcutEdges, FPointFlagsGraph & graph);
    void loadShortcutEdgesWithUnpackingData(ifstream & input, unsigned int shortcutEdges, FPointFlagsGraphWithUnpackingData & graph);
public:
    DDSGFLoader(string inputFile);
    FPointFlagsGraph * loadFlagsGraph();
    FPointFlagsGraphWithUnpackingData * loadFlagsGraphWithUnpackingData();
};

// ~~~ DESCRIPTION OF THE CH FORMAT ~~~
// - a binary file, 32/64-bit-interger organized
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
//        double: weight
//        unsigned int: flags
//    m2 times, shortcut edges:
//        unsigned int: source node
//        unsigned int: target node
//        double: weight
//        unsigned int: flags
//        unsigned int: shortcut middle node
//    unsigned int: 0x12345678 as terminator
// - possible (bit) flags are:
//    1 = forward edge
//    2 = backward edge
//    4 = shortcut edge

#endif //CONTRACTION_HIERARCHIES_DDSGFLOADER_H
