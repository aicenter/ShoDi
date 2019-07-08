//
// Author: Xenty (Michal Cvach)
// Created on: 8.7.19
//

#include "../../Timer/Timer.h"
#include "DDSGFLoader.h"
#include <fstream>

//______________________________________________________________________________________________________________________
DDSGFLoader::DDSGFLoader(string inputFile) : inputFile(inputFile) {

}

// This function reads the input file and puts all the Contraction Hierarchies data into a FPointFlagsGraph instance.
//______________________________________________________________________________________________________________________
FPointFlagsGraph * DDSGFLoader::loadFlagsGraph() {
    ifstream input;
    input.open(inputFile, ios::binary);

    if ( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
        exit(1);
    }

    Timer loadTimer("DDSGF FPointGraph loading timer");
    loadTimer.begin();

    if ( verifyHeader(input) == false ) {
        printf("Something was wrong with the header.\nFile should start with 'CH\\r\\n' followed by the");
        printf(" version '1', but it didn't.\n");
        exit(1);
    }

    printf("Header was verified and suggests the file is a correct DDSGF CH file.\n");

    unsigned int nodes, edges, shortcutEdges;
    loadCnts(input, nodes, edges, shortcutEdges);
    FPointFlagsGraph * graph = new FPointFlagsGraph(nodes);

    loadRanks(input, nodes, *graph);
    loadOriginalEdges(input, edges, *graph);
    loadShortcutEdges(input, shortcutEdges, *graph);

    if ( verifyFooter(input) == false ) {
        printf("The file didn't end the expected way!\nThis file should have ended with an unsigned int");
        printf(" with value '0x12345678', but it didn't.\nThe file could be corrupted, so using it");
        printf(" might provide unexpected and incorrect results.\n");
    }

    loadTimer.finish();
    loadTimer.printMeasuredTime();

    printf("FPointGraph seems to be loaded correctly!\n");

    return graph;
}

//______________________________________________________________________________________________________________________
FPointFlagsGraphWithUnpackingData * DDSGFLoader::loadFlagsGraphWithUnpackingData() {
    ifstream input;
    input.open(inputFile, ios::binary);

    if ( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
        exit(1);
    }

    Timer loadTimer("DDSGF FPointGraph loading timer");
    loadTimer.begin();

    if ( verifyHeader(input) == false ) {
        printf("Something was wrong with the header.\nFile should start with 'CH\\r\\n' followed by the");
        printf(" version '1', but it didn't.\n");
        exit(1);
    }

    printf("Header was verified and suggests the file is a correct DDSGF CH file.\n");

    unsigned int nodes, edges, shortcutEdges;
    loadCnts(input, nodes, edges, shortcutEdges);
    printf("FPointGraph should have %u unpacking pairs\n", shortcutEdges);
    FPointFlagsGraphWithUnpackingData * graph = new FPointFlagsGraphWithUnpackingData(nodes);

    loadRanks(input, nodes, *graph);
    loadOriginalEdges(input, edges, *graph);
    loadShortcutEdgesWithUnpackingData(input, shortcutEdges, *graph);

    if ( verifyFooter(input) == false ) {
        printf("The file didn't end the expected way!\nThis file should have ended with an unsigned int");
        printf(" with value '0x12345678', but it didn't.\nThe file could be corrupted, so using it");
        printf(" might provide unexpected and incorrect results.\n");
    }

    loadTimer.finish();
    loadTimer.printMeasuredTime();

    printf("FPointGraph seems to be loaded correctly!\n");

    return graph;
}

//______________________________________________________________________________________________________________________
void DDSGFLoader::loadRanks(ifstream & input, unsigned int nodes, FPointFlagsGraph & graph) {
    for(unsigned int i = 0; i < nodes; i++) {
        unsigned int rank;
        input.read((char*)&rank, sizeof(rank));
        graph.data(i).rank = rank;
    }
}

//______________________________________________________________________________________________________________________
void DDSGFLoader::loadOriginalEdges(ifstream & input, unsigned int edges, FPointFlagsGraph & graph) {
    for(unsigned int i = 0; i < edges; i++) {
        unsigned int from, to, flags;
        double weight;
        input.read((char*)&from, sizeof(from));
        input.read((char*)&to, sizeof(to));
        input.read((char*)&weight, sizeof(weight));
        input.read((char*)&flags, sizeof(flags));

        bool forward = false;
        bool backward = false;
        if((flags & 1) == 1) {
            forward = true;
        }
        if((flags & 2) == 2) {
            backward = true;
        }
        if ( graph.data(from).rank < graph.data(to).rank ) {
            graph.addEdge(from, to, weight, forward, backward);
        } else {
            graph.addEdge(to, from, weight, forward, backward);
        }

    }
}

//______________________________________________________________________________________________________________________
void DDSGFLoader::loadShortcutEdges(ifstream & input, unsigned int shortcutEdges, FPointFlagsGraph & graph) {
    for(unsigned int i = 0; i < shortcutEdges; i++) {
        unsigned int from, to, flags, middleNode;
        double weight;
        input.read((char*)&from, sizeof(from));
        input.read((char*)&to, sizeof(to));
        input.read((char*)&weight, sizeof(weight));
        input.read((char*)&flags, sizeof(flags));
        input.read((char*)&middleNode, sizeof(middleNode));

        bool forward = false;
        bool backward = false;
        if((flags & 1) == 1) {
            forward = true;
        }
        if((flags & 2) == 2) {
            backward = true;
        }
        if ( graph.data(from).rank < graph.data(to).rank ) {
            graph.addEdge(from, to, weight, forward, backward);
        } else {
            graph.addEdge(to, from, weight, forward, backward);
        }

    }
}

//______________________________________________________________________________________________________________________
void DDSGFLoader::loadShortcutEdgesWithUnpackingData(ifstream & input, unsigned int shortcutEdges, FPointFlagsGraphWithUnpackingData & graph) {
    for(unsigned int i = 0; i < shortcutEdges; i++) {
        unsigned int from, to, flags, middleNode;
        double weight;
        input.read((char*)&from, sizeof(from));
        input.read((char*)&to, sizeof(to));
        input.read((char*)&weight, sizeof(weight));
        input.read((char*)&flags, sizeof(flags));
        input.read((char*)&middleNode, sizeof(middleNode));

        bool forward = false;
        bool backward = false;
        if((flags & 1) == 1) {
            forward = true;
            graph.addForwardUnpackingData(from, to, middleNode);
        }
        if((flags & 2) == 2) {
            backward = true;
            graph.addBackwardUnpackingData(from, to, middleNode);
        }
        if ( graph.data(from).rank < graph.data(to).rank ) {
            graph.addEdge(from, to, weight, forward, backward);
        } else {
            graph.addEdge(to, from, weight, forward, backward);
        }

    }
}

//______________________________________________________________________________________________________________________
bool DDSGFLoader::verifyHeader(ifstream & input) {
    char tmp;
    input.read (&tmp, sizeof(tmp));
    if(tmp != 0x43) {
        return false;
    }
    input.read (&tmp, sizeof(tmp));
    if(tmp != 0x48) {
        return false;
    }
    input.read (&tmp, sizeof(tmp));
    if(tmp != 0x0d) {
        return false;
    }
    input.read (&tmp, sizeof(tmp));
    if(tmp != 0x0a) {
        return false;
    }

    unsigned int version;
    input.read((char*)&version, sizeof(version));
    if (version != 1) {
        return false;
    }

    return true;

}

//______________________________________________________________________________________________________________________
bool DDSGFLoader::verifyFooter(ifstream & input) {
    unsigned int footerNum;
    input.read((char*)&footerNum, sizeof(footerNum));
    if (footerNum != 0x12345678) {
        return false;
    }
    return true;
}

//______________________________________________________________________________________________________________________
void DDSGFLoader::loadCnts(ifstream & input, unsigned int & nodes, unsigned int & edges, unsigned int & shortcutEdges) {
    input.read ((char*)&nodes, sizeof(nodes));
    input.read ((char*)&edges, sizeof(edges));
    input.read ((char*)&shortcutEdges, sizeof(shortcutEdges));
}