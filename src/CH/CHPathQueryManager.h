//
// Author: Xenty (Michal Cvach)
// Created on: 7.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPATHQUERYMANAGER_H
#define TRANSIT_NODE_ROUTING_CHPATHQUERYMANAGER_H

#include <vector>
#include <queue>
#include "../GraphBuilder/FlagsGraphWithUnpackingData.h"

using namespace std;

// This class is responsible for the Contraction Hierarchies 'path' queries - when we require the actual path and not
// only the distance between two points.
//______________________________________________________________________________________________________________________
class CHPathQueryManager {
public:
    CHPathQueryManager(FlagsGraphWithUnpackingData & g);
    long long unsigned int findDistance(const unsigned int source, const unsigned int target);
protected:
    void forwardStall(unsigned int stallnode, long long unsigned int stalldistance);
    void backwardStall(unsigned int stallnode, long long unsigned int stalldistance);
    void outputPath(const unsigned int meetingNode);
    void fillFromPath(const unsigned int meetingNode, vector<pair<unsigned int, unsigned int> > & fromPath);
    void fillToPath(const unsigned int meetingNode, vector<pair<unsigned int, unsigned int> > & toPath);
    void unpackPrevious(vector<pair<unsigned int, unsigned int> > & fromPath);
    void unpackFollowing(vector<pair<unsigned int, unsigned int> > & fromPath);
    void unpackEdge(unsigned int s, unsigned int t);
    FlagsGraphWithUnpackingData & graph;
    long long unsigned int upperbound;
    unsigned int meetingNode;
    vector<unsigned int> forwardChanged;
    vector<unsigned int> backwardChanged;
    vector<unsigned int> forwardStallChanged;
    vector<unsigned int> backwardStallChanged;
    void prepareStructuresForNextQuery();
};


#endif //TRANSIT_NODE_ROUTING_CHPATHQUERYMANAGER_H
