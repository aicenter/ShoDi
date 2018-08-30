//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#include <climits>
#include "CHFlagsGraphQueryManager.h"

//______________________________________________________________________________________________________________________
CHFlagsGraphQueryManager::CHFlagsGraphQueryManager(FlagsGraph & g) : graph(g) {

}

//______________________________________________________________________________________________________________________
long long unsigned int CHFlagsGraphQueryManager::findDistance(const unsigned int source, const unsigned int target) {

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
    upperbound = ULLONG_MAX;

    while (! (forwardQ.empty() && backwardQ.empty())) {
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
            if ( graph.data(curNode).backwardSettled ) {
                long long unsigned int newUpperboundCandidate = curLen +  graph.data(curNode).backwardDist;
                if (newUpperboundCandidate < upperbound) {
                    upperbound = newUpperboundCandidate;
                }
            }

            const vector<QueryEdge> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                if ((*iter).backward && graph.data((*iter).targetNode).forwardReached) {
                    long long unsigned int newdistance = graph.data((*iter).targetNode).forwardDist + (*iter).weight;
                    if (newdistance < curLen) {
                        graph.data(curNode).forwardDist = newdistance;
                        forwardStall(curNode, newdistance);
                    }
                }

                if (! (*iter).forward) {
                    continue;
                }
                //forwardReached[(*iter).first] = true;

                if (graph.data((*iter).targetNode).rank > graph.data(curNode).rank) {
                    long long unsigned int newlen = curLen + (*iter).weight;

                    if (newlen < graph.data((*iter).targetNode).forwardDist) {
                        forwardQ.push(DijkstraNode((*iter).targetNode, newlen));
                        if (graph.data((*iter).targetNode).forwardDist == ULLONG_MAX) {
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
                }
            }

            const vector<QueryEdge> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                if ((*iter).forward && graph.data((*iter).targetNode).backwardReached) {
                    long long unsigned int newdistance = graph.data((*iter).targetNode).backwardDist + (*iter).weight;
                    if (newdistance < curLen) {
                        backwardStall(curNode, newdistance);
                    }
                }

                if (! (*iter).backward) {
                    continue;
                }
                //forwardReached[(*iter).first] = true;

                if(graph.data((*iter).targetNode).rank > graph.data(curNode).rank) {
                    long long unsigned int newlen = curLen + (*iter).weight;

                    if (newlen < graph.data((*iter).targetNode).backwardDist) {
                        backwardQ.push(DijkstraNode((*iter).targetNode, newlen));
                        if (graph.data((*iter).targetNode).backwardDist == ULLONG_MAX) {
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
void CHFlagsGraphQueryManager::forwardStall(unsigned int stallnode, long long unsigned int stalldistance) {
    queue<DijkstraNode> stallQueue;
    stallQueue.push(DijkstraNode(stallnode, stalldistance));

    while (! stallQueue.empty()) {
        unsigned int curNode = stallQueue.front().ID;
        long long unsigned int curDist = stallQueue.front().weight;
        stallQueue.pop();
        graph.data(curNode).forwardStalled = true;
        forwardStallChanged.push_back(curNode);

        const vector<QueryEdge> & neighbours = graph.nextNodes(curNode);
        for (auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
            if (! (*iter).forward) {
                continue;
            }

            if (graph.data((*iter).targetNode).forwardReached) {
                long long unsigned int newdistance = curDist + (*iter).weight;

                if (newdistance < graph.data((*iter).targetNode).forwardDist) {
                    if (! graph.data((*iter).targetNode).forwardStalled) {
                        stallQueue.push(DijkstraNode((*iter).targetNode, newdistance));
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

//______________________________________________________________________________________________________________________
void CHFlagsGraphQueryManager::backwardStall(unsigned int stallnode, long long unsigned int stalldistance) {
    queue<DijkstraNode> stallQueue;
    stallQueue.push(DijkstraNode(stallnode, stalldistance));

    while (! stallQueue.empty()) {
        unsigned int curNode = stallQueue.front().ID;
        long long unsigned int curDist = stallQueue.front().weight;
        stallQueue.pop();
        graph.data(curNode).backwardStalled = true;
        backwardStallChanged.push_back(curNode);

        const vector<QueryEdge> & neighbours = graph.nextNodes(curNode);
        for (auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
            if (! (*iter).backward) {
                continue;
            }

            if (graph.data((*iter).targetNode).backwardReached) {
                long long unsigned int newdistance = curDist + (*iter).weight;

                if (newdistance < graph.data((*iter).targetNode).backwardDist) {
                    if (! graph.data((*iter).targetNode).backwardStalled) {
                        stallQueue.push(DijkstraNode((*iter).targetNode, newdistance));
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

//______________________________________________________________________________________________________________________
void CHFlagsGraphQueryManager::prepareStructuresForNextQuery() {
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