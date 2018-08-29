//
// Author: Xenty (Michal Cvach)
// Created on: 27.8.18
//

#include <climits>
#include <queue>
#include "CHAlternativeQueryManger.h"

//______________________________________________________________________________________________________________________
CHAlternativeQueryManager::CHAlternativeQueryManager(vector<unsigned int> & x, const Graph & g) : ranks(x), graph(g) {
    unsigned int n = graph.nodes();
    forwardDist.resize(n, ULLONG_MAX);
    backwardDist.resize(n, ULLONG_MAX);
    forwardReached.resize(n, false);
    backwardReached.resize(n, false);
    forwardSettled.resize(n, false);
    backwardSettled.resize(n, false);
}

//______________________________________________________________________________________________________________________
long long unsigned int CHAlternativeQueryManager::findDistance(const unsigned int source, const unsigned int target) {

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

            if (forwardSettled[curNode]) {
                continue;
            }

            forwardSettled[curNode] = true;
            if (backwardSettled[curNode]) {
                long long unsigned int newUpperboundCandidate = curLen + backwardDist[curNode];
                if (newUpperboundCandidate < upperbound) {
                    upperbound = newUpperboundCandidate;
                }
            }

            const vector<pair<unsigned int, unsigned long long int>> & neighbours = graph.outgoingEdges(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                if (forwardReached[(*iter).first] && existsBackwardEdge(curNode, (*iter).first) != ULLONG_MAX) {

                }

                //forwardReached[(*iter).first] = true;
                if (ranks[(*iter).first] > ranks[curNode]) {
                    long long unsigned int newlen = curLen + (*iter).second;

                    if (newlen < forwardDist[(*iter).first]) {
                        forwardQ.push(DijkstraNode((*iter).first, newlen));
                        if (forwardDist[(*iter).first] == ULLONG_MAX) {
                            forwardChanged.push_back((*iter).first);
                        }
                        forwardDist[(*iter).first] = newlen;
                        forwardReached[(*iter).first] = true;
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

            if (backwardSettled[curNode]) {
                continue;
            }

            backwardSettled[curNode] = true;
            if (forwardSettled[curNode]) {
                long long unsigned int newUpperboundCandidate = curLen + forwardDist[curNode];
                if (newUpperboundCandidate < upperbound) {
                    upperbound = newUpperboundCandidate;
                }
            }

            const vector<pair<unsigned int, unsigned long long int>> & neighbours = graph.incomingEdges(curNode);
            for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
                //forwardReached[(*iter).first] = true;

                if(ranks[(*iter).first] > ranks[curNode]) {
                    long long unsigned int newlen = curLen + (*iter).second;

                    if (newlen < backwardDist[(*iter).first]) {
                        backwardQ.push(DijkstraNode((*iter).first, newlen));
                        if (backwardDist[(*iter).first] == ULLONG_MAX) {
                            backwardChanged.push_back((*iter).first);
                        }
                        backwardDist[(*iter).first] = newlen;
                        backwardReached[(*iter).first] = true;
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
/*void CHAlternativeQueryManager::relaxForwardEdges(unsigned int curNode, long long unsigned int curLen, priority_queue<DijkstraNode> & pq) {
    const vector<pair<unsigned int, unsigned long long int>> & neighbours = graph.outgoingEdges(curNode);
    for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
        //forwardReached[(*iter).first] = true;
        if (ranks[(*iter).first] > ranks[curNode]) {
            long long unsigned int newlen = curLen + (*iter).second;

            if (newlen < forwardDist[(*iter).first]) {
                forwardQ.push(DijkstraNode((*iter).first, newlen));
                forwardDist[(*iter).first] = newlen;
            }
        }
    }
}

//______________________________________________________________________________________________________________________
void CHAlternativeQueryManager::relaxBackwardEdges(unsigned int curNode, long long unsigned int curLen, priority_queue<DijkstraNode> & pq) {
    const vector<pair<unsigned int, unsigned long long int>> & neighbours = graph.incomingEdges(curNode);
    for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
        //forwardReached[(*iter).first] = true;
        if(ranks[(*iter).first] > ranks[curNode]) {
            long long unsigned int newlen = curLen + (*iter).second;

            if (newlen < backwardDist[(*iter).first]) {
                backwardQ.push(DijkstraNode((*iter).first, newlen));
                backwardDist[(*iter).first] = newlen;
            }
        }
    }
}*/

//______________________________________________________________________________________________________________________
long long unsigned int CHAlternativeQueryManager::existsBackwardEdge(const unsigned int x, const unsigned int y) {
    const vector < pair < unsigned int, long long unsigned int > > & neighbours = graph.incomingEdges(x);
    for(unsigned int i = 0; i < neighbours.size(); i++) {
        if(y == neighbours.at(i).first) {
            return neighbours.at(i).second;
        }
    }
    return ULLONG_MAX;
}

//______________________________________________________________________________________________________________________
long long unsigned int CHAlternativeQueryManager::existsForwardEdge(const unsigned int x, const unsigned int y) {
    const vector < pair < unsigned int, long long unsigned int > > & neighbours = graph.outgoingEdges(x);
    for(unsigned int i = 0; i < neighbours.size(); i++) {
        if(y == neighbours.at(i).first) {
            return neighbours.at(i).second;
        }
    }
    return ULLONG_MAX;
}

//______________________________________________________________________________________________________________________
void CHAlternativeQueryManager::prepareStructuresForNextQuery() {
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

   // forwardQ.clean();
   // backwardQ.clean();
}