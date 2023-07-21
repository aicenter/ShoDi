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

//______________________________________________________________________________________________________________________
unsigned int CHDistanceQueryManager::findDistance(const unsigned int start, const unsigned int goal) {
    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> forwardQ(cmp);
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, decltype(cmp)> backwardQ(cmp);

    forwardQ.push(DijkstraNode(start, 0));
    backwardQ.push(DijkstraNode(goal, 0));

    bool forwardFinished = false;
    bool backwardFinished = false;

    graph.data(start).forwardDist = 0;
    graph.data(goal).backwardDist = 0;
    forwardChanged.push_back(start);
    backwardChanged.push_back(goal);
    graph.data(start).forwardReached = true;
    graph.data(goal).backwardReached = true;

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
            const std::vector<QueryEdge> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                if ((*iter).backward && graph.data((*iter).targetNode).forwardReached) {
                    unsigned int newdistance = graph.data((*iter).targetNode).forwardDist + (*iter).weight;
                    if (newdistance < curLen) {
                        graph.data(curNode).forwardDist = newdistance;
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

            const std::vector<QueryEdge> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                if ((*iter).forward && graph.data((*iter).targetNode).backwardReached) {
                    unsigned int newdistance = graph.data((*iter).targetNode).backwardDist + (*iter).weight;
                    if (newdistance < curLen) {
                        graph.data(curNode).backwardDist = newdistance;
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

//______________________________________________________________________________________________________________________
void CHDistanceQueryManager::prepareStructuresForNextQuery() {
    for(size_t i = 0; i < forwardChanged.size(); i++) {
        graph.resetForwardInfo(forwardChanged[i]);
    }
    forwardChanged.clear();

    for(size_t i = 0; i < backwardChanged.size(); i++) {
       graph.resetBackwardInfo(backwardChanged[i]);
    }
    backwardChanged.clear();

    for(size_t i = 0; i < forwardStallChanged.size(); i++) {
        graph.resetForwardStall(forwardStallChanged[i]);
    }
    forwardStallChanged.clear();

    for(size_t i = 0; i < backwardStallChanged.size(); i++) {
        graph.resetBackwardStall(backwardStallChanged[i]);
    }
    backwardStallChanged.clear();
}