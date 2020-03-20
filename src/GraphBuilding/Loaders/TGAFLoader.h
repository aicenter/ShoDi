//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#ifndef CONTRACTION_HIERARCHIES_TGAFLOADER_H
#define CONTRACTION_HIERARCHIES_TGAFLOADER_H


#include "TNRGLoader.h"
#include "../Structures/TransitNodeRoutingArcFlagsGraph.h"

// Class used for loading the Transit Node Routing with Arc Flags data structure. I use a simple binary format for the
// structure, this function can load those files and initialize TransitNodeRoutingArcFlagsGraph instances from them,
// which can then be used to answer queries.
//______________________________________________________________________________________________________________________
class TGAFLoader : public TNRGLoader {
public:
    TGAFLoader(string inputFile);
    TransitNodeRoutingArcFlagsGraph * loadTNRAFforDistanceQueries();
protected:
    void parseFirstLine(ifstream & input, unsigned int & nodes, unsigned int & edges, unsigned int & tnodesAmount, unsigned int & regionsCnt);
    void parseRanks(ifstream & input, TransitNodeRoutingArcFlagsGraph * graph, unsigned int nodes);
    void parseRegions(ifstream & input, TransitNodeRoutingArcFlagsGraph & graph, unsigned int nodes);
    void parseAccessNodes(ifstream & input, TransitNodeRoutingArcFlagsGraph & graph, unsigned int nodes, unsigned int regionsCnt);
};


#endif //CONTRACTION_HIERARCHIES_TGAFLOADER_H
