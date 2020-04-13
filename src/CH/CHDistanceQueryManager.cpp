//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#include <climits>
#include "CHDistanceQueryManager.h"
#include "../Dijkstra/DijkstraNode.h"

//______________________________________________________________________________________________________________________
CHDistanceQueryManager::CHDistanceQueryManager(FlagsGraph & g) : graph(g) {

}

// We use the query algorithm that was described in the "Contraction Hierarchies: Faster and Simpler Hierarchical
// Routing in Road Networks" article by Robert Geisberger, Peter Sanders, Dominik Schultes, and Daniel Delling.
// Basically, the query is a modified bidirectional Dijkstra query, where from the source node we only expand following
// nodes with higher contraction rank than the current node and from the target we only expand previous nodes with
// higher contraction rank than the current node. Both scopes will eventually meet in the node with the highest
// contraction rank from all nodes in the path. This implementation additionaly uses the 'stall-on-demand' technique
// described in the same article which noticeably improved the query times.
//______________________________________________________________________________________________________________________
unsigned int CHDistanceQueryManager::findDistance(const unsigned int source, const unsigned int target) {
    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> forwardQ(cmp);
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> backwardQ(cmp);

    forwardQ.push(DijkstraNode(source, 0));
    backwardQ.push(DijkstraNode(target, 0));

    bool forwardFinished = false;
    bool backwardFinished = false;

    graph.data(source).forwardDist = 0;
    graph.data(target).backwardDist = 0;
    forwardChanged.push_back(source);
    backwardChanged.push_back(target);
    graph.data(source).forwardReached = true;
    graph.data(target).backwardReached = true;

    bool forward = false;
    upperbound = UINT_MAX;

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
            unsigned int curLen = forwardQ.top().weight;
            forwardQ.pop();

            if (graph.data(curNode).forwardSettled || graph.data(curNode).forwardStalled) {
                continue;
            }


            graph.data(curNode).forwardSettled = true;
            // Check if the node was already settled in the opposite direction - if yes, we get a new candidate
            // for the shortest path.
            if ( graph.data(curNode).backwardSettled ) {
                unsigned int newUpperboundCandidate = curLen +  graph.data(curNode).backwardDist;
                if (newUpperboundCandidate < upperbound) {
                    upperbound = newUpperboundCandidate;
                }
            }

            // Classic edges relaxation
            const vector<QueryEdge> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                // Here we stall a node if it's reached on a suboptimal path. This can happen in the Contraction
                // Hierarchies query algorithm, because we can reach a node from the wrong direction (for example
                // we reach a node on a suboptimal path in the forward direction, because the actual optimal path
                // will be later found in the backward direction)
                if ((*iter).backward && graph.data((*iter).targetNode).forwardReached) {
                    unsigned int newdistance = graph.data((*iter).targetNode).forwardDist + (*iter).weight;
                    if (newdistance < curLen) {
                        graph.data(curNode).forwardDist = newdistance;
                        //forwardStall(curNode, newdistance); // FIXME - currently fixed stalling giving mismatches by completely removing stalling.
                    }
                }

                if (! (*iter).forward) {
                    continue;
                }

                // This is basically the dijkstra edge relaxation process. Additionaly, we unstall the node
                // if it was stalled previously, because it might be now reached on the optimal path.
                if (graph.data((*iter).targetNode).rank > graph.data(curNode).rank) {
                    unsigned int newlen = curLen + (*iter).weight;

                    if (newlen < graph.data((*iter).targetNode).forwardDist) {
                        forwardQ.push(DijkstraNode((*iter).targetNode, newlen));
                        if (graph.data((*iter).targetNode).forwardDist == UINT_MAX) {
                            forwardChanged.push_back((*iter).targetNode);
                        }
                        graph.data((*iter).targetNode).forwardDist = newlen;
                        graph.data((*iter).targetNode).forwardReached = true;
                        graph.data((*iter).targetNode).forwardStalled = false;
                    }
                }
            }

            if(! forwardQ.empty() && forwardQ.top().weight > upperbound) {
                forwardFinished = true;
            }
            // The backward direction is symmetrical to the forward direction.
        } else {
            if (backwardQ.empty()) {
                break;
            }

            unsigned int curNode = backwardQ.top().ID;
            unsigned int curLen = backwardQ.top().weight;
            backwardQ.pop();

            if (graph.data(curNode).backwardSettled || graph.data(curNode).backwardStalled) {
                continue;
            }

            graph.data(curNode).backwardSettled = true;
            if (graph.data(curNode).forwardSettled) {
                unsigned int newUpperboundCandidate = curLen + graph.data(curNode).forwardDist;
                if (newUpperboundCandidate < upperbound) {
                    upperbound = newUpperboundCandidate;
                }
            }

            const vector<QueryEdge> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                if ((*iter).forward && graph.data((*iter).targetNode).backwardReached) {
                    unsigned int newdistance = graph.data((*iter).targetNode).backwardDist + (*iter).weight;
                    if (newdistance < curLen) {
                        graph.data(curNode).backwardDist = newdistance;
                        //backwardStall(curNode, newdistance); // FIXME - currently fixed stalling giving mismatches by completely removing stalling.
                    }
                }

                if (! (*iter).backward) {
                    continue;
                }

                if(graph.data((*iter).targetNode).rank > graph.data(curNode).rank) {
                    unsigned int newlen = curLen + (*iter).weight;

                    if (newlen < graph.data((*iter).targetNode).backwardDist) {
                        backwardQ.push(DijkstraNode((*iter).targetNode, newlen));
                        if (graph.data((*iter).targetNode).backwardDist == UINT_MAX) {
                            backwardChanged.push_back((*iter).targetNode);
                        }
                        graph.data((*iter).targetNode).backwardDist = newlen;
                        graph.data((*iter).targetNode).backwardReached = true;
                        graph.data((*iter).targetNode).backwardStalled = false;
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

    prepareStructuresForNextQuery();

    return upperbound;
}

// Code for stalling a node in the forward distance. We try to stall additional nodes using BFS as long as we don't
// reach already stalled nodes or nodes that can't be stalled.
//______________________________________________________________________________________________________________________
void CHDistanceQueryManager::forwardStall(unsigned int stallnode, unsigned int stalldistance) {
    queue<DijkstraNode> stallQueue;
    stallQueue.push(DijkstraNode(stallnode, stalldistance));

    while (! stallQueue.empty()) {
        unsigned int curNode = stallQueue.front().ID;
        unsigned int curDist = stallQueue.front().weight;
        stallQueue.pop();
        graph.data(curNode).forwardStalled = true;
        forwardStallChanged.push_back(curNode);

        const vector<QueryEdge> & neighbours = graph.nextNodes(curNode);
        for (auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
            if (! (*iter).forward) {
                continue;
            }

            if (graph.data((*iter).targetNode).forwardReached) {
                unsigned int newdistance = curDist + (*iter).weight;

                if (newdistance < graph.data((*iter).targetNode).forwardDist) {
                    if (! graph.data((*iter).targetNode).forwardStalled) {
                        stallQueue.push(DijkstraNode((*iter).targetNode, newdistance));
                        if (graph.data((*iter).targetNode).forwardDist == UINT_MAX) {
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
void CHDistanceQueryManager::backwardStall(unsigned int stallnode, unsigned int stalldistance) {
    queue<DijkstraNode> stallQueue;
    stallQueue.push(DijkstraNode(stallnode, stalldistance));

    while (! stallQueue.empty()) {
        unsigned int curNode = stallQueue.front().ID;
        unsigned int curDist = stallQueue.front().weight;
        stallQueue.pop();
        graph.data(curNode).backwardStalled = true;
        backwardStallChanged.push_back(curNode);

        const vector<QueryEdge> & neighbours = graph.nextNodes(curNode);
        for (auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
            if (! (*iter).backward) {
                continue;
            }

            if (graph.data((*iter).targetNode).backwardReached) {
                unsigned int newdistance = curDist + (*iter).weight;

                if (newdistance < graph.data((*iter).targetNode).backwardDist) {
                    if (! graph.data((*iter).targetNode).backwardStalled) {
                        stallQueue.push(DijkstraNode((*iter).targetNode, newdistance));
                        if (graph.data((*iter).targetNode).backwardDist == UINT_MAX) {
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
void CHDistanceQueryManager::prepareStructuresForNextQuery() {
    for (unsigned int i = 0; i < forwardChanged.size(); i++) {
        graph.resetForwardInfo(forwardChanged[i]);
    }
    forwardChanged.clear();

    for (unsigned int i = 0; i < backwardChanged.size(); i++) {
       graph.resetBackwardInfo(backwardChanged[i]);
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