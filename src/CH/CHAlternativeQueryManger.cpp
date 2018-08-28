//
// Author: Xenty (Michal Cvach)
// Created on: 27.8.18
//

#include <climits>
#include "CHAlternativeQueryManger.h"

//______________________________________________________________________________________________________________________
CHAlternativeQueryManager::CHAlternativeQueryManager(vector<unsigned int> & x, const Graph & g) : ranks(x), graph(g) {
    unsigned int n = graph.nodes();
    forwardDist.resize(n, ULLONG_MAX);
    backwardDist.resize(n, ULLONG_MAX);
    //forwardReached.resize(n, false);
    //backwardReached.resize(n, false);
    forwardSettled.resize(n, false);
    backwardSettled.resize(n, false);
}

//______________________________________________________________________________________________________________________
long long unsigned int CHAlternativeQueryManager::findDistance(const unsigned int source, const unsigned int target) {

    forwardQ.push(source, 0);
    backwardQ.push(target, 0);

    bool forwardFinished = false;
    bool backwardFinished = false;

    forwardDist[source] = 0;
    backwardDist[target] = 0;
    forwardChanged.push_back(source);
    backwardChanged.push_back(target);

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

            forwardSettled[curNode] = true;
            if (backwardSettled[curNode]) {
                long long unsigned int newUpperboundCandidate = curLen + backwardDist[curNode];
                if (newUpperboundCandidate < upperbound) {
                    upperbound = newUpperboundCandidate;
                }
            }

            relaxForwardEdges(curNode, curLen);

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

            backwardSettled[curNode] = true;
            if (forwardSettled[curNode]) {
                long long unsigned int newUpperboundCandidate = curLen + forwardDist[curNode];
                if (newUpperboundCandidate < upperbound) {
                    upperbound = newUpperboundCandidate;
                }
            }

            relaxBackwardEdges(curNode, curLen);

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
void CHAlternativeQueryManager::relaxForwardEdges(unsigned int curNode, long long unsigned int curLen) {
    const vector<pair<unsigned int, unsigned long long int>> & neighbours = graph.outgoingEdges(curNode);
    for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
        //forwardReached[(*iter).first] = true;
        if (ranks[(*iter).first] > ranks[curNode]) {
            long long unsigned int newlen = curLen + (*iter).second;

            if (newlen < forwardDist[(*iter).first]) {
                if (forwardDist[(*iter).first] == ULLONG_MAX) {
                    forwardQ.push((*iter).first, newlen);
                    forwardChanged.push_back((*iter).first);
                } else {
                    forwardQ.decreaseKey((*iter).first, newlen);
                }
                forwardDist[(*iter).first] = newlen;
            }
        }
    }
}

//______________________________________________________________________________________________________________________
void CHAlternativeQueryManager::relaxBackwardEdges(unsigned int curNode, long long unsigned int curLen) {
    const vector<pair<unsigned int, unsigned long long int>> & neighbours = graph.incomingEdges(curNode);
    for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
        //forwardReached[(*iter).first] = true;
        if(ranks[(*iter).first] > ranks[curNode]) {
            long long unsigned int newlen = curLen + (*iter).second;

            if (newlen < backwardDist[(*iter).first]) {
                if (backwardDist[(*iter).first] == ULLONG_MAX) {
                    backwardQ.push((*iter).first, newlen);
                    backwardChanged.push_back((*iter).first);
                } else {
                    backwardQ.decreaseKey((*iter).first, newlen);
                }
                backwardDist[(*iter).first] = newlen;
            }
        }
    }
}



//______________________________________________________________________________________________________________________
void CHAlternativeQueryManager::prepareStructuresForNextQuery() {
    for (unsigned int i = 0; i < forwardChanged.size(); i++) {
        forwardDist[forwardChanged[i]] = ULLONG_MAX;
        forwardSettled[forwardChanged[i]] = false;
        //forwardReached[forwardChanged[i]] = false;
    }
    forwardChanged.clear();

    for (unsigned int i = 0; i < backwardChanged.size(); i++) {
        backwardDist[backwardChanged[i]] = ULLONG_MAX;
        backwardSettled[backwardChanged[i]] = false;
        //backwardReached[backwardChanged[i]] = false;
    }
    backwardChanged.clear();

    forwardQ.clean();
    backwardQ.clean();
}