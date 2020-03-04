//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#ifndef CONTRACTION_HIERARCHIES_TGAFLOADER_H
#define CONTRACTION_HIERARCHIES_TGAFLOADER_H


#include "TNRGLoader.h"
#include "../Structures/IntegerStructures/TransitNodeRoutingArcFlagsGraph.h"

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
