//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#include <climits>
#include "CHFlagsGraphQueryManager.h"

//______________________________________________________________________________________________________________________
CHFlagsGraphQueryManager::CHFlagsGraphQueryManager(vector<unsigned int> & x, const FlagsGraph & g) : ranks(x), graph(g) {
    unsigned int n = graph.nodes();
    forwardDist.resize(n, ULLONG_MAX);
    backwardDist.resize(n, ULLONG_MAX);
    forwardReached.resize(n, false);
    backwardReached.resize(n, false);
    forwardSettled.resize(n, false);
    backwardSettled.resize(n, false);
    forwardStalled.resize(n, false);
    backwardStalled.resize(n, false);
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

    forwardDist[source] = 0;
    backwardDist[target] = 0;
    forwardChanged.push_back(source);
    backwardChanged.push_back(target);
    forwardReached[source] = true;
    backwardReached[target] = true;

    bool forward = false;
    upperbound = ULLONG_MAX;

    while (! (forwardQ.empty() && backwardQ.empty())) {
        //printf("Currently in queues: forward: %lu, backward: %lu (top: %llu)\n", forwardQ.size(), backwardQ.size(), backwardQ.top().weight);
        //printf("Currently in queues: forward: %lu, backward: %lu\n", forwardQ.size(), backwardQ.size());
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

            if (forwardSettled[curNode] || forwardStalled[curNode]) {
                continue;
            }

            forwardSettled[curNode] = true;
            if (backwardSettled[curNode]) {
                long long unsigned int newUpperboundCandidate = curLen + backwardDist[curNode];
                if (newUpperboundCandidate < upperbound) {
                    upperbound = newUpperboundCandidate;
                }
            }

            const vector<Edge> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                if ((*iter).backward && forwardReached[(*iter).targetNode]) {
                    long long unsigned int newdistance = forwardDist[(*iter).targetNode] + (*iter).weight;
                    if (newdistance < curLen) {
                        forwardDist[curNode] = newdistance;
                        forwardStall(curNode, newdistance);
                    }
                }

                if (! (*iter).forward) {
                    continue;
                }
                //forwardReached[(*iter).first] = true;

                if (ranks[(*iter).targetNode] > ranks[curNode]) {
                    long long unsigned int newlen = curLen + (*iter).weight;

                    if (newlen < forwardDist[(*iter).targetNode]) {
                        forwardQ.push(DijkstraNode((*iter).targetNode, newlen));
                        if (forwardDist[(*iter).targetNode] == ULLONG_MAX) {
                            forwardChanged.push_back((*iter).targetNode);
                        }
                        forwardDist[(*iter).targetNode] = newlen;
                        forwardReached[(*iter).targetNode] = true;
                        forwardStalled[(*iter).targetNode] = false;
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

            if (backwardSettled[curNode] || backwardStalled[curNode]) {
                continue;
            }

            backwardSettled[curNode] = true;
            if (forwardSettled[curNode]) {
                long long unsigned int newUpperboundCandidate = curLen + forwardDist[curNode];
                if (newUpperboundCandidate < upperbound) {
                    upperbound = newUpperboundCandidate;
                }
            }

            const vector<Edge> & neighbours = graph.nextNodes(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                if ((*iter).forward && backwardReached[(*iter).targetNode]) {
                    long long unsigned int newdistance = backwardDist[(*iter).targetNode] + (*iter).weight;
                    if (newdistance < curLen) {
                        backwardStall(curNode, newdistance);
                    }
                }

                if (! (*iter).backward) {
                    continue;
                }
                //forwardReached[(*iter).first] = true;

                if(ranks[(*iter).targetNode] > ranks[curNode]) {
                    long long unsigned int newlen = curLen + (*iter).weight;

                    if (newlen < backwardDist[(*iter).targetNode]) {
                        backwardQ.push(DijkstraNode((*iter).targetNode, newlen));
                        if (backwardDist[(*iter).targetNode] == ULLONG_MAX) {
                            backwardChanged.push_back((*iter).targetNode);
                        }
                        backwardDist[(*iter).targetNode] = newlen;
                        backwardReached[(*iter).targetNode] = true;
                        backwardStalled[(*iter).targetNode] = false;
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
        forwardStalled[curNode] = true;
        forwardStallChanged.push_back(curNode);

        const vector<Edge> & neighbours = graph.nextNodes(curNode);
        for (auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
            if (! (*iter).forward) {
                continue;
            }

            if (forwardReached[(*iter).targetNode]) {
                long long unsigned int newdistance = curDist + (*iter).weight;

                if (newdistance < forwardDist[(*iter).targetNode]) {
                    if (! forwardStalled[(*iter).targetNode]) {
                        stallQueue.push(DijkstraNode((*iter).targetNode, newdistance));
                        if (forwardDist[(*iter).targetNode] == ULLONG_MAX) {
                            forwardChanged.push_back((*iter).targetNode);
                        }
                        forwardDist[(*iter).targetNode] = newdistance;
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
        backwardStalled[curNode] = true;
        backwardStallChanged.push_back(curNode);

        const vector<Edge> & neighbours = graph.nextNodes(curNode);
        for (auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
            if (! (*iter).backward) {
                continue;
            }

            if (backwardReached[(*iter).targetNode]) {
                long long unsigned int newdistance = curDist + (*iter).weight;

                if (newdistance < backwardDist[(*iter).targetNode]) {
                    if (! backwardStalled[(*iter).targetNode]) {
                        stallQueue.push(DijkstraNode((*iter).targetNode, newdistance));
                        if (backwardDist[(*iter).targetNode] == ULLONG_MAX) {
                            backwardChanged.push_back((*iter).targetNode);
                        }
                        backwardDist[(*iter).targetNode] = newdistance;
                    }
                }
            }
        }
    }
}

//______________________________________________________________________________________________________________________
void CHFlagsGraphQueryManager::prepareStructuresForNextQuery() {
    for (unsigned int i = 0; i < forwardChanged.size(); i++) {
        forwardDist[forwardChanged[i]] = ULLONG_MAX;
        forwardSettled[forwardChanged[i]] = false;
        forwardReached[forwardChanged[i]] = false;
    }
    forwardChanged.clear();

    for (unsigned int i = 0; i < backwardChanged.size(); i++) {
        backwardDist[backwardChanged[i]] = ULLONG_MAX;
        backwardSettled[backwardChanged[i]] = false;
        backwardReached[backwardChanged[i]] = false;
    }
    backwardChanged.clear();

    for (unsigned int i = 0; i < forwardStallChanged.size(); i++) {
        forwardStalled[forwardStallChanged[i]] = false;
    }
    forwardStallChanged.clear();

    for (unsigned int i = 0; i < backwardStallChanged.size(); i++) {
        backwardStalled[backwardStallChanged[i]] = false;
    }
    backwardStallChanged.clear();

    // forwardQ.clean();
    // backwardQ.clean();
}