//
// Author: Xenty (Michal Cvach)
// Created on: 7.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPATHQUERYMANAGER_H
#define TRANSIT_NODE_ROUTING_CHPATHQUERYMANAGER_H

#include <map>
#include <vector>
#include "../GraphBuilder/Graph.h"

using namespace std;

// This class is responsible for the Contraction Hierarchies 'path' queries - when we require the actual path and not
// only the distance between two points.
//______________________________________________________________________________________________________________________
class CHPathQueryManager {
public:
    CHPathQueryManager(vector<unsigned int> & x, map < pair < unsigned int, unsigned int >, unsigned int > & y);
    long long unsigned int findPath(const unsigned int source, const unsigned int target, const Graph & graph);
private:
    void outputPath(const unsigned int meetingNode, const unsigned int * fromPrev, const unsigned int * toPrev);
    void fillFromPath(const unsigned int meetingNode, const unsigned int * fromPrev, vector<pair<unsigned int, unsigned int> > & fromPath);
    void fillToPath(const unsigned int meetingNode, const unsigned int * toPrev, vector<pair<unsigned int, unsigned int> > & toPath);
    void unpackPrevious(vector<pair<unsigned int, unsigned int> > & fromPath);
    void unpackFollowing(vector<pair<unsigned int, unsigned int> > & fromPath);
    void unpackEdge(unsigned int s, unsigned int t);
    map < pair < unsigned int, unsigned int >, unsigned int > & unpackingData;
    vector < unsigned int > & ranks;
};


#endif //TRANSIT_NODE_ROUTING_CHPATHQUERYMANAGER_H
