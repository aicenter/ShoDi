//
// Author: Xenty (Michal Cvach)
// Created on: 15.10.19
//

#ifndef CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGGRAPHFORPATHQUERIES_H
#define CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGGRAPHFORPATHQUERIES_H

#include "FlagsGraphWithUnpackingData.h"
#include "../../TNR/Structures/AccessNodeData.h"

// A Transit Node Routing data structure. This variant can be used to also obtain the actual shortest paths and not
// just the shortest distances.
//______________________________________________________________________________________________________________________
class TransitNodeRoutingGraphForPathQueries : public FlagsGraphWithUnpackingData {
public:
    TransitNodeRoutingGraphForPathQueries(unsigned int nodes, unsigned int transitNodesAmount);
    void addUnpackingEdge(unsigned int from, unsigned int to, unsigned int weight);
    const std::vector < std::pair< unsigned int , unsigned int > > & unpackingNeighbours(unsigned int nodeID);

    // From TNRGraph
    bool isLocalQuery(unsigned int source, unsigned int target);
    unsigned int findTNRDistance(unsigned int source, unsigned int target);
    void addMappingPair(unsigned int realID, unsigned int transitNodesID);
    void setDistanceTableValue(unsigned int i, unsigned int j, unsigned int value);
    void addForwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance);
    void addBackwardAccessNode(unsigned int node, unsigned int accessNodeID, unsigned int accessNodeDistance);
    void addForwardSearchSpaceNode(unsigned int sourceNode, unsigned int searchSpaceNode);
    void addBackwardSearchSpaceNode(unsigned int sourceNode, unsigned int searchSpaceNode);
protected:
    std::vector < std::vector < std::pair< unsigned int, unsigned int > > > unpackingGraph;

    // From TNRGraph
    std::vector < std::vector < AccessNodeData > > forwardAccessNodes;
    std::vector < std::vector < AccessNodeData > > backwardAccessNodes;
    std::vector < std::vector < unsigned int > > forwardSearchSpaces;
    std::vector < std::vector < unsigned int > > backwardSearchSpaces;
    std::vector < std::vector < unsigned int > > transitNodesDistanceTable;
    std::unordered_map< unsigned int, unsigned int > transitNodeMapping;
};


#endif //CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGGRAPHFORPATHQUERIES_H
