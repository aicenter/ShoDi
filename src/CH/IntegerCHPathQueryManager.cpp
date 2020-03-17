//
// Author: Xenty (Michal Cvach)
// Created on: 7.8.18
//

#include <climits>
#include "IntegerCHPathQueryManager.h"
#include "../Dijkstra/IntegerDijkstraNode.h"
#include "../GraphBuilding/Structures/constants_defines.h"

//______________________________________________________________________________________________________________________
IntegerCHPathQueryManager::IntegerCHPathQueryManager(IntegerFlagsGraphWithUnpackingData & g) : graph(g) {

}

// We use the query algorithm that was described in the "Contraction Hierarchies: Faster and Simpler Hierarchical
// Routing in Road Networks" article by Robert Geisberger, Peter Sanders, Dominik Schultes, and Daniel Delling.
// Basically, the query is a modified bidirectional Dijkstra query, where from the source node we only expand following
// nodes with higher contraction rank than the current node and from the target we only expand previous nodes with
// higher contraction rank than the current node. Both scopes will eventually meet in the node with the highest
// contraction rank from all nodes in the path.
//______________________________________________________________________________________________________________________
long long unsigned int IntegerCHPathQueryManager::processQuery(const unsigned int source, const unsigned int target) {
    auto cmp = [](IntegerDijkstraNode left, IntegerDijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<IntegerDijkstraNode, vector<IntegerDijkstraNode>, decltype(cmp)> forwardQ(cmp);
    priority_queue<IntegerDijkstraNode, vector<IntegerDijkstraNode>, decltype(cmp)> backwardQ(cmp);

    forwardQ.push(IntegerDijkstraNode(source, 0));
    backwardQ.push(IntegerDijkstraNode(target, 0));

    bool forwardFinished = false;
    bool backwardFinished = false;

    graph.data(source).forwardDist = 0;
    graph.data(target).backwardDist = 0;
    forwardChanged.push_back(source);
    backwardChanged.push_back(target);
    graph.data(source).forwardReached = true;
    graph.data(target).backwardReached = true;

    bool forward = false;
    upperbound = ULLONG_MAX;
    meetingNode = UINT_MAX;

    while (! (forwardQ.empty() && backwardQ.empty())) {
        // Determine the search direction for the current iteration (forward of backward)
        // The directions take turns unless one of the directions is already finished (that means either the first
        // element in the priority queue already has weight bigger than the upperbound or the queue in that direction
        // doesn't contain any more elements).
        if (forwardFinished || forwardQ.empty()) {
            forward = false;
        } else if (backwardFinished || backwardQ.empty()) {
            forward = true;
        } else {
            forward = ! forward;
        }

        if (forward) {
            if (forwardQ.empty()) {
                break;
            }

            unsigned int curNode = forwardQ.top().ID;
            long long unsigned int curLen = forwardQ.top().weight;
            forwardQ.pop();

            if (graph.data(curNode).forwardSettled || graph.data(curNode).forwardStalled) {
                continue;
            }


            graph.data(curNode).forwardSettled = true;
            // Check if the node was already settled in the opposite direction - if yes, we get a new candidate
            // for the shortest path.
            if ( graph.data(curNode).backwardSettled ) {
                long long unsigned int newUpperboundCandidate = curLen +  graph.data(curNode).backwardDist;
                if (newUpperboundCandidate < upperbound) {
                    upperbound = newUpperboundCandidate;
                    meetingNode = curNode;
                }
            }

            // Classic edges relaxation
            const vector<IntegerQueryEdgeWithUnpackingData> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                // Here we stall a node if it's reached on a suboptimal path. This can happen in the Contraction
                // Hierarchies query algorithm, because we can reach a node from the wrong direction (for example
                // we reach a node on a suboptimal path in the forward direction, because the actual optimal path
                // will be later found in the backward direction)
                /*if ((*iter).backward && graph.data((*iter).targetNode).forwardReached) {
                    long long unsigned int newdistance = graph.data((*iter).targetNode).forwardDist + (*iter).weight;
                    if (newdistance < curLen) {
                        //graph.data(curNode).forwardDist = newdistance;
                        //forwardStall(curNode, newdistance);
                    }
                }*/

                if (! (*iter).forward) {
                    continue;
                }

                // This is basically the dijkstra edge relaxation process. Additionaly, we unstall the node
                // if it was stalled previously, because it might be now reached on the optimal path.
                if (graph.data((*iter).targetNode).rank > graph.data(curNode).rank) {
                    long long unsigned int newlen = curLen + (*iter).weight;

                    if (newlen < graph.data((*iter).targetNode).forwardDist) {
                        forwardQ.push(IntegerDijkstraNode((*iter).targetNode, newlen));
                        if (graph.data((*iter).targetNode).forwardDist == ULLONG_MAX) {
                            forwardChanged.push_back((*iter).targetNode);
                        }
                        graph.data((*iter).targetNode).forwardDist = newlen;
                        graph.data((*iter).targetNode).forwardReached = true;
                        graph.data((*iter).targetNode).forwardStalled = false;
                        graph.setForwardPrev((*iter).targetNode, curNode);
                    }
                }
            }

            if(! forwardQ.empty() && forwardQ.top().weight > upperbound) {
                forwardFinished = true;
            }
            // The backward direction is symetrical to the forward direction.
        } else {
            if (backwardQ.empty()) {
                break;
            }

            unsigned int curNode = backwardQ.top().ID;
            long long unsigned int curLen = backwardQ.top().weight;
            backwardQ.pop();

            if (graph.data(curNode).backwardSettled || graph.data(curNode).backwardStalled) {
                continue;
            }

            graph.data(curNode).backwardSettled = true;
            if (graph.data(curNode).forwardSettled) {
                long long unsigned int newUpperboundCandidate = curLen + graph.data(curNode).forwardDist;
                if (newUpperboundCandidate < upperbound) {
                    upperbound = newUpperboundCandidate;
                    meetingNode = curNode;
                }
            }

            const vector<IntegerQueryEdgeWithUnpackingData> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                /*if ((*iter).forward && graph.data((*iter).targetNode).backwardReached) {
                    long long unsigned int newdistance = graph.data((*iter).targetNode).backwardDist + (*iter).weight;
                    if (newdistance < curLen) {
                        graph.data(curNode).backwardDist = newdistance;
                        //backwardStall(curNode, newdistance);
                    }
                }*/

                if (! (*iter).backward) {
                    continue;
                }

                if(graph.data((*iter).targetNode).rank > graph.data(curNode).rank) {
                    long long unsigned int newlen = curLen + (*iter).weight;

                    if (newlen < graph.data((*iter).targetNode).backwardDist) {
                        backwardQ.push(IntegerDijkstraNode((*iter).targetNode, newlen));
                        if (graph.data((*iter).targetNode).backwardDist == ULLONG_MAX) {
                            backwardChanged.push_back((*iter).targetNode);
                        }
                        graph.data((*iter).targetNode).backwardDist = newlen;
                        graph.data((*iter).targetNode).backwardReached = true;
                        graph.data((*iter).targetNode).backwardStalled = false;
                        graph.setBackwardPrev((*iter).targetNode, curNode);
                    }
                }
            }

            if(! backwardQ.empty() && backwardQ.top().weight > upperbound) {
                backwardFinished = true;
            }
        }

        if (backwardFinished && forwardFinished) {
            break;
        }

    }

    return upperbound;
}

// Finds and returns the distance. Additionally reconstructs the actual path and outputs it to standard output
// in a somewhat human readable format. Mostly useful for debug, for actual queries, findPath() should be a better
// option since it creates a vector of all the edges on the path which can be then processed further.
//______________________________________________________________________________________________________________________
long long unsigned int IntegerCHPathQueryManager::findDistanceOutputPath(const unsigned int source, const unsigned int target) {
    long long unsigned int distance = processQuery(source, target);

    outputPath(meetingNode);

    prepareStructuresForNextQuery();

    return distance;
}

//______________________________________________________________________________________________________________________
long long unsigned int IntegerCHPathQueryManager::findDistanceOnly(const unsigned int source, const unsigned int target) {
    long long unsigned int distance = processQuery(source, target);

    prepareStructuresForNextQuery();

    return distance;
}

// Finds the path, returns the distance, and additionally fills the vector 'edges' with all pairs of 'source->target'
// edges in the original graph (in order as they appear on the path). The vector 'edgeLengths' is filled with respective
// lengths of the edges in the original graph.
//______________________________________________________________________________________________________________________
long long unsigned int IntegerCHPathQueryManager::findPath(const unsigned int source, const unsigned int target, vector<pair<unsigned int, unsigned int>> & edges, vector<unsigned int> & edgeLengths) {
    long long unsigned int distance = processQuery(source, target);

    fillPathInfo(meetingNode, edges, edgeLengths);

    prepareStructuresForNextQuery();

    return distance;
}

// Finds the path, returns the distance, and additionally fills the vector 'edges' with all pairs of 'source->target'
// edges in the original graph (in order as they appear on the path).
//______________________________________________________________________________________________________________________
long long unsigned int IntegerCHPathQueryManager::findPath(const unsigned int source, const unsigned int target, vector<pair<unsigned int, unsigned int>> & edges) {
    long long unsigned int distance = processQuery(source, target);

    fillPathInfoEdgesOnly(meetingNode, edges);

    prepareStructuresForNextQuery();

    return distance;
}

// Finds the path, returns the distance, and additionally fills the vector 'edges' with all pairs of 'source->target'
// edges in the original graph (in order as they appear on the path).
//______________________________________________________________________________________________________________________
long long unsigned int IntegerCHPathQueryManager::findPath(const unsigned int source, const unsigned int target, vector<SimpleEdge> & path) {
    long long unsigned int distance = processQuery(source, target);

    fillPathInfoEdgesOnly(meetingNode, path);

    prepareStructuresForNextQuery();

    return distance;
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::printEdgesForwardShortcut(const unsigned int source, const unsigned int target) {
    unpackForwardEdge(source, target);
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::printEdgesBackwardShortcut(const unsigned int source, const unsigned int target) {
    unpackBackwardEdge(source, target);
}

// Code for stalling a node in the forward distance. We try to stall additional nodes using BFS as long as we don't
// reach already stalled nodes or nodes that can't be stalled.
//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::forwardStall(unsigned int stallnode, long long unsigned int stalldistance) {
    queue<IntegerDijkstraNode> stallQueue;
    stallQueue.push(IntegerDijkstraNode(stallnode, stalldistance));

    while (! stallQueue.empty()) {
        unsigned int curNode = stallQueue.front().ID;
        long long unsigned int curDist = stallQueue.front().weight;
        stallQueue.pop();
        graph.data(curNode).forwardStalled = true;
        forwardStallChanged.push_back(curNode);

        const vector<IntegerQueryEdgeWithUnpackingData> & neighbours = graph.nextNodes(curNode);
        for (auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
            if (! (*iter).forward) {
                continue;
            }

            if (graph.data((*iter).targetNode).forwardReached) {
                long long unsigned int newdistance = curDist + (*iter).weight;

                if (newdistance < graph.data((*iter).targetNode).forwardDist) {
                    if (! graph.data((*iter).targetNode).forwardStalled) {
                        stallQueue.push(IntegerDijkstraNode((*iter).targetNode, newdistance));
                        if (graph.data((*iter).targetNode).forwardDist == ULLONG_MAX) {
                            forwardChanged.push_back((*iter).targetNode);
                        }
                        graph.data((*iter).targetNode).forwardDist = newdistance;
                    }
                }
            }
        }
    }

}

// Code for stalling a node in the backward distance. We try to stall additional nodes using BFS as long as we don't
// reach already stalled nodes or nodes that can't be stalled.
//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::backwardStall(unsigned int stallnode, long long unsigned int stalldistance) {
    queue<IntegerDijkstraNode> stallQueue;
    stallQueue.push(IntegerDijkstraNode(stallnode, stalldistance));

    while (! stallQueue.empty()) {
        unsigned int curNode = stallQueue.front().ID;
        long long unsigned int curDist = stallQueue.front().weight;
        stallQueue.pop();
        graph.data(curNode).backwardStalled = true;
        backwardStallChanged.push_back(curNode);

        const vector<IntegerQueryEdgeWithUnpackingData> & neighbours = graph.nextNodes(curNode);
        for (auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
            if (! (*iter).backward) {
                continue;
            }

            if (graph.data((*iter).targetNode).backwardReached) {
                long long unsigned int newdistance = curDist + (*iter).weight;

                if (newdistance < graph.data((*iter).targetNode).backwardDist) {
                    if (! graph.data((*iter).targetNode).backwardStalled) {
                        stallQueue.push(IntegerDijkstraNode((*iter).targetNode, newdistance));
                        if (graph.data((*iter).targetNode).backwardDist == ULLONG_MAX) {
                            backwardChanged.push_back((*iter).targetNode);
                        }
                        graph.data((*iter).targetNode).backwardDist = newdistance;
                    }
                }
            }
        }
    }
}

// Reset information for the nodes that were changed in the current query.
//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::prepareStructuresForNextQuery() {
    for (unsigned int i = 0; i < forwardChanged.size(); i++) {
        graph.resetForwardInfo(forwardChanged[i]);
        graph.resetForwardPrev(forwardChanged[i]);
    }
    forwardChanged.clear();

    for (unsigned int i = 0; i < backwardChanged.size(); i++) {
        graph.resetBackwardInfo(backwardChanged[i]);
        graph.resetBackwardPrev(backwardChanged[i]);
    }
    backwardChanged.clear();

    for (unsigned int i = 0; i < forwardStallChanged.size(); i++) {
        graph.resetForwardStall(forwardStallChanged[i]);
    }
    forwardStallChanged.clear();

    for (unsigned int i = 0; i < backwardStallChanged.size(); i++) {
        graph.resetBackwardStall(backwardStallChanged[i]);
    }
    backwardStallChanged.clear();
}

// This function fills the nodes on the path in the Contraction Hierarchies graph from the fromPrev and toPrev arrays
// by calling the fillFromPath() and fillToPath() functions and then calls the unpackPrevious() and unpackFollowing()
// functions which print the actual path by unpacking the shortcuts and printing only edges in the original graph.
//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::outputPath(const unsigned int meetingNode) {
    if (meetingNode == UINT_MAX) {
        printf("No path was found! Nothing to unpack!\n");
        return;
    }

    printf("~~~ Outputting shortest path (unpacked from CH) ~~~\n");
    vector<pair<unsigned int, unsigned int> > fromPath;
    vector<pair<unsigned int, unsigned int> > toPath;
    fillFromPath(meetingNode, fromPath);
    fillToPath(meetingNode, toPath);
    unpackPrevious(fromPath);
    unpackFollowing(toPath);
    printf("~~~ End of path output ~~~\n");
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::fillPathInfo(const unsigned int meetingNode, vector<pair<unsigned int, unsigned int>> & edges, vector<unsigned int> & edgeLengths) {
    if (meetingNode == UINT_MAX) {
        return;
    }

    vector<pair<unsigned int, unsigned int> > fromPath;
    vector<pair<unsigned int, unsigned int> > toPath;
    fillFromPath(meetingNode, fromPath);
    fillToPath(meetingNode, toPath);
    getPreviousPathPart(fromPath, edges, edgeLengths);
    getFollowingPathPart(toPath, edges, edgeLengths);
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::fillPathInfoEdgesOnly(const unsigned int meetingNode, vector<pair<unsigned int, unsigned int>> & edges) {
    if (meetingNode == UINT_MAX) {
        return;
    }

    vector<pair<unsigned int, unsigned int> > fromPath;
    vector<pair<unsigned int, unsigned int> > toPath;
    fillFromPath(meetingNode, fromPath);
    fillToPath(meetingNode, toPath);
    getPreviousPathPartEdgesOnly(fromPath, edges);
    getFollowingPathPartEdgesOnly(toPath, edges);
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::fillPathInfoEdgesOnly(const unsigned int meetingNode, vector<SimpleEdge> & edges) {
    if (meetingNode == UINT_MAX) {
        return;
    }

    vector<pair<unsigned int, unsigned int> > fromPath;
    vector<pair<unsigned int, unsigned int> > toPath;
    fillFromPath(meetingNode, fromPath);
    fillToPath(meetingNode, toPath);
    getPreviousPathPartEdgesOnly(fromPath, edges);
    getFollowingPathPartEdgesOnly(toPath, edges);
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::fillFromPath(const unsigned int meetingNode, vector<pair<unsigned int, unsigned int> > & fromPath) {
    unsigned int current = meetingNode;
    while(graph.getForwardPrev(current) != UINT_MAX) {
        fromPath.push_back(make_pair(graph.getForwardPrev(current), current));
        current = graph.getForwardPrev(current);
    }
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::fillToPath(const unsigned int meetingNode, vector<pair<unsigned int, unsigned int> > & toPath) {
    unsigned int current = meetingNode;
    while(graph.getBackwardPrev(current) != UINT_MAX) {
        toPath.push_back(make_pair(current, graph.getBackwardPrev(current)));
        current = graph.getBackwardPrev(current);
    }
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::unpackPrevious(vector<pair<unsigned int, unsigned int> > & fromPath) {
    for(int i = fromPath.size()-1; i >= 0; i--) {
        unpackForwardEdge(fromPath[i].first, fromPath[i].second);
    }
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::unpackFollowing(vector<pair<unsigned int, unsigned int> > & toPath) {
    for(unsigned int i = 0; i < toPath.size(); i++) {
        unpackBackwardEdge(toPath[i].first, toPath[i].second);
    }
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::getPreviousPathPart(vector<pair<unsigned int, unsigned int> > & fromPath, vector<pair<unsigned int, unsigned int>> & edges, vector<unsigned int> & edgeLengths) {
    for(int i = fromPath.size()-1; i >= 0; i--) {
        getForwardEdge(fromPath[i].first, fromPath[i].second, edges, edgeLengths);
    }
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::getFollowingPathPart(vector<pair<unsigned int, unsigned int> > & toPath, vector<pair<unsigned int, unsigned int>> & edges, vector<unsigned int> & edgeLengths) {
    for(unsigned int i = 0; i < toPath.size(); i++) {
        getBackwardEdge(toPath[i].first, toPath[i].second, edges, edgeLengths);
    }
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::getPreviousPathPartEdgesOnly(vector<pair<unsigned int, unsigned int> > & fromPath, vector<pair<unsigned int, unsigned int>> & edges) {
    for(int i = fromPath.size()-1; i >= 0; i--) {
        getForwardEdgeWithoutLength(fromPath[i].first, fromPath[i].second, edges);
    }
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::getFollowingPathPartEdgesOnly(vector<pair<unsigned int, unsigned int> > & toPath, vector<pair<unsigned int, unsigned int>> & edges) {
    for(unsigned int i = 0; i < toPath.size(); i++) {
        getBackwardEdgeWithoutLength(toPath[i].first, toPath[i].second, edges);
    }
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::getPreviousPathPartEdgesOnly(vector<pair<unsigned int, unsigned int> > & fromPath, vector<SimpleEdge> & path) {
    for(int i = fromPath.size()-1; i >= 0; i--) {
        getForwardEdgeWithoutLength(fromPath[i].first, fromPath[i].second, path);
    }
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::getFollowingPathPartEdgesOnly(vector<pair<unsigned int, unsigned int> > & toPath, vector<SimpleEdge> & path) {
    for(unsigned int i = 0; i < toPath.size(); i++) {
        getBackwardEdgeWithoutLength(toPath[i].first, toPath[i].second, path);
    }
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::unpackForwardEdge(unsigned int s, unsigned int t) {
    unsigned int m;
    if (graph.data(s).rank < graph.data(t).rank) {
        m = graph.getMiddleNode(s, t, FORWARD);
    } else {
        m = graph.getMiddleNode(t, s, FORWARD);
    }

    if (m == UINT_MAX) {
        printf("%u -> %u (%u)\n", s, t, graph.getDistance(s, t, FORWARD));
        return;
    }

    unpackBackwardEdge(s, m);
    unpackForwardEdge(m, t);
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::unpackBackwardEdge(unsigned int s, unsigned int t) {
    unsigned int m;
    if (graph.data(s).rank < graph.data(t).rank) {
        m = graph.getMiddleNode(s, t, BACKWARD);
    } else {
        m = graph.getMiddleNode(t, s, BACKWARD);
    }

    if (m == UINT_MAX) {
        printf("%u -> %u (%u)\n", s, t, graph.getDistance(s, t, BACKWARD));
        return;
    }

    unpackBackwardEdge(s, m);
    unpackForwardEdge(m, t);
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::getForwardEdge(unsigned int s, unsigned int t, vector<pair<unsigned int, unsigned int>> & edges, vector<unsigned int> & edgeLengths) {
    unsigned int m;
    if (graph.data(s).rank < graph.data(t).rank) {
        m = graph.getMiddleNode(s, t, FORWARD);
    } else {
        m = graph.getMiddleNode(t, s, FORWARD);
    }

    if (m == UINT_MAX) {
        edges.push_back(make_pair(s, t));
        edgeLengths.push_back(graph.getDistance(s, t, FORWARD));
        return;
    }

    getBackwardEdge(s, m, edges, edgeLengths);
    getForwardEdge(m, t, edges, edgeLengths);
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::getBackwardEdge(unsigned int s, unsigned int t, vector<pair<unsigned int, unsigned int>> & edges, vector<unsigned int> & edgeLengths) {
    unsigned int m;
    if (graph.data(s).rank < graph.data(t).rank) {
        m = graph.getMiddleNode(s, t, BACKWARD);
    } else {
        m = graph.getMiddleNode(t, s, BACKWARD);
    }

    if (m == UINT_MAX) {
        edges.push_back(make_pair(s, t));
        edgeLengths.push_back(graph.getDistance(s, t, BACKWARD));
        return;
    }

    getBackwardEdge(s, m, edges, edgeLengths);
    getForwardEdge(m, t, edges, edgeLengths);
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::getForwardEdgeWithoutLength(unsigned int s, unsigned int t, vector<pair<unsigned int, unsigned int>> & edges) {
    unsigned int m;
    if (graph.data(s).rank < graph.data(t).rank) {
        m = graph.getMiddleNode(s, t, FORWARD);
    } else {
        m = graph.getMiddleNode(t, s, FORWARD);
    }

    if (m == UINT_MAX) {
        edges.push_back(make_pair(s, t));
        return;
    }

    getBackwardEdgeWithoutLength(s, m, edges);
    getForwardEdgeWithoutLength(m, t, edges);
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::getBackwardEdgeWithoutLength(unsigned int s, unsigned int t, vector<pair<unsigned int, unsigned int>> & edges) {
    unsigned int m;
    if (graph.data(s).rank < graph.data(t).rank) {
        m = graph.getMiddleNode(s, t, BACKWARD);
    } else {
        m = graph.getMiddleNode(t, s, BACKWARD);
    }

    if (m == UINT_MAX) {
        edges.push_back(make_pair(s, t));
        return;
    }

    getBackwardEdgeWithoutLength(s, m, edges);
    getForwardEdgeWithoutLength(m, t, edges);
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::getForwardEdgeWithoutLength(unsigned int s, unsigned int t, vector<SimpleEdge> & path) {
    unsigned int m;
    if (graph.data(s).rank < graph.data(t).rank) {
        m = graph.getMiddleNode(s, t, FORWARD);
    } else {
        m = graph.getMiddleNode(t, s, FORWARD);
    }

    if (m == UINT_MAX) {
        path.push_back(SimpleEdge(s, t));
        return;
    }

    getBackwardEdgeWithoutLength(s, m, path);
    getForwardEdgeWithoutLength(m, t, path);
}

//______________________________________________________________________________________________________________________
void IntegerCHPathQueryManager::getBackwardEdgeWithoutLength(unsigned int s, unsigned int t, vector<SimpleEdge> & path) {
    unsigned int m;
    if (graph.data(s).rank < graph.data(t).rank) {
        m = graph.getMiddleNode(s, t, BACKWARD);
    } else {
        m = graph.getMiddleNode(t, s, BACKWARD);
    }

    if (m == UINT_MAX) {
        path.push_back(SimpleEdge(s, t));
        return;
    }

    getBackwardEdgeWithoutLength(s, m, path);
    getForwardEdgeWithoutLength(m, t, path);
}