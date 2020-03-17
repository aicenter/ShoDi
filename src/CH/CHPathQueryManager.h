//
// Author: Xenty (Michal Cvach)
// Created on: 7.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPATHQUERYMANAGER_H
#define TRANSIT_NODE_ROUTING_CHPATHQUERYMANAGER_H

#include <vector>
#include <queue>
#include "../GraphBuilding/Structures/FlagsGraphWithUnpackingData.h"
#include "../GraphBuilding/Structures/SimpleEdge.h"

using namespace std;

// ~~~ WARNING ~~~
// WARNING: This class doesn't work correctly yet. This class returns correct distances, but incorrect paths.
// The unpacking routine doesn't work properly and it even sometimes gives longer paths than Dijkstra (longer in terms
// of edges), so either it's unpacking some edges that aren't shortcuts, or it's unpacking edges incorrectly
// (for example getting an incorrect middle node for an edge would result in unpacking wrong edges later, so it could
// give more edges in Dijkstra). Until this is resolved and the class is fixed, this shouldn't be used anywhere.
// ~~~ END OF WARNING ~~~

// This class is responsible for the Contraction Hierarchies 'path' queries - when we require the actual path and not
// only the distance between two points.
//______________________________________________________________________________________________________________________
class CHPathQueryManager {
public:
    CHPathQueryManager(FlagsGraphWithUnpackingData & g);
    unsigned int findDistanceOutputPath(const unsigned int source, const unsigned int target);
    unsigned int findDistanceOnly(const unsigned int source, const unsigned int target);
    unsigned int findPath(const unsigned int source, const unsigned int target, vector<pair<unsigned int, unsigned int>> & edges, vector<unsigned int> & edgeLengths);
    unsigned int findPath(const unsigned int source, const unsigned int target, vector<pair<unsigned int, unsigned int>> & edges);
    unsigned int findPath(const unsigned int source, const unsigned int target, vector<SimpleEdge> & path);
    void printEdgesForwardShortcut(const unsigned int source, const unsigned int target);
    void printEdgesBackwardShortcut(const unsigned int source, const unsigned int target);
protected:
    unsigned int processQuery(const unsigned int source, const unsigned int target);
    void forwardStall(unsigned int stallnode, unsigned int stalldistance);
    void backwardStall(unsigned int stallnode, unsigned int stalldistance);
    void outputPath(const unsigned int meetingNode);
    void fillPathInfo(const unsigned int meetingNode, vector<pair<unsigned int, unsigned int>> & edges, vector<unsigned int> & edgeLengths);
    void fillPathInfoEdgesOnly(const unsigned int meetingNode, vector<pair<unsigned int, unsigned int>> & edges);
    void fillPathInfoEdgesOnly(const unsigned int meetingNode, vector<SimpleEdge> & edges);
    void fillFromPath(const unsigned int meetingNode, vector<pair<unsigned int, unsigned int> > & fromPath);
    void fillToPath(const unsigned int meetingNode, vector<pair<unsigned int, unsigned int> > & toPath);
    void unpackPrevious(vector<pair<unsigned int, unsigned int> > & fromPath);
    void unpackFollowing(vector<pair<unsigned int, unsigned int> > & fromPath);
    void getPreviousPathPart(vector<pair<unsigned int, unsigned int> > & fromPath, vector<pair<unsigned int, unsigned int>> & edges, vector<unsigned int> & edgeLengths);
    void getFollowingPathPart(vector<pair<unsigned int, unsigned int> > & fromPath, vector<pair<unsigned int, unsigned int>> & edges, vector<unsigned int> & edgeLengths);
    void getPreviousPathPartEdgesOnly(vector<pair<unsigned int, unsigned int> > & fromPath, vector<pair<unsigned int, unsigned int>> & edges);
    void getFollowingPathPartEdgesOnly(vector<pair<unsigned int, unsigned int> > & fromPath, vector<pair<unsigned int, unsigned int>> & edges);
    void getPreviousPathPartEdgesOnly(vector<pair<unsigned int, unsigned int> > & fromPath, vector<SimpleEdge> & path);
    void getFollowingPathPartEdgesOnly(vector<pair<unsigned int, unsigned int> > & fromPath, vector<SimpleEdge> & path);
    void unpackForwardEdge(unsigned int s, unsigned int t);
    void unpackBackwardEdge(unsigned int s, unsigned int t);
    void getForwardEdge(unsigned int s, unsigned int t, vector<pair<unsigned int, unsigned int>> & edges, vector<unsigned int> & edgeLengths);
    void getBackwardEdge(unsigned int s, unsigned int t, vector<pair<unsigned int, unsigned int>> & edges, vector<unsigned int> & edgeLengths);
    void getForwardEdgeWithoutLength(unsigned int s, unsigned int t, vector<pair<unsigned int, unsigned int>> & edges);
    void getBackwardEdgeWithoutLength(unsigned int s, unsigned int t, vector<pair<unsigned int, unsigned int>> & edges);
    void getForwardEdgeWithoutLength(unsigned int s, unsigned int t, vector<SimpleEdge> & path);
    void getBackwardEdgeWithoutLength(unsigned int s, unsigned int t, vector<SimpleEdge> & path);
    FlagsGraphWithUnpackingData & graph;
    unsigned int upperbound;
    unsigned int meetingNode;
    vector<unsigned int> forwardChanged;
    vector<unsigned int> backwardChanged;
    vector<unsigned int> forwardStallChanged;
    vector<unsigned int> backwardStallChanged;
    void prepareStructuresForNextQuery();
};


#endif //TRANSIT_NODE_ROUTING_CHPATHQUERYMANAGER_H
