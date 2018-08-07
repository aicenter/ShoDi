//
// Author: Xenty (Michal Cvach)
// Created on: 7.8.18
//

#include <queue>
#include <climits>
#include "CHPathQueryManager.h"
#include "../Dijkstra/DijkstraNode.h"

//______________________________________________________________________________________________________________________
CHPathQueryManager::CHPathQueryManager(vector<unsigned int> & x, map < pair < unsigned int, unsigned int >, unsigned int > & y) : CHQueryManager(x), unpackingData(y) {

}

//______________________________________________________________________________________________________________________
long long unsigned int CHPathQueryManager::findPath(const unsigned int source, const unsigned int target, const Graph & graph) {
    unsigned int n = graph.nodes();

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> fromQueue(cmp);
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> toQueue(cmp);

    long long unsigned int * fromDistance = new long long unsigned int[n];
    long long unsigned int * toDistance = new long long unsigned int[n];
    bool * fromClosed = new bool[n];
    bool * toClosed = new bool[n];
    unsigned int * fromPrev = new unsigned int[n];
    unsigned int * toPrev = new unsigned int[n];

    for(unsigned int i = 0; i < n; i++) {
        fromDistance[i] = ULLONG_MAX;
        toDistance[i] = ULLONG_MAX;
        fromClosed[i] = false;
        toClosed[i] = false;
        fromPrev[i] = UINT_MAX;
        toPrev[i] = UINT_MAX;
    }

    fromDistance[source] = 0;
    toDistance[target] = 0;

    fromQueue.push(DijkstraNode(source, 0));
    toQueue.push(DijkstraNode(target, 0));

    long long unsigned int shortestFound = ULLONG_MAX;
    unsigned int meetingNode = UINT_MAX;
    while (! fromQueue.empty() || ! toQueue.empty()) {

        if (! fromQueue.empty()) {
            unsigned int current = fromQueue.top().ID;
            long long unsigned int currentDist = fromQueue.top().weight;
            fromClosed[current] = true;

            if( currentDist < shortestFound ) {
                if (toClosed[current]) {
                    if (currentDist + toDistance[current] < shortestFound) {
                        shortestFound = currentDist + toDistance[current];
                        meetingNode = current;
                    }
                }

                const vector < pair < unsigned int, long long unsigned int > > & following = graph.outgoingEdges(current);
                for (unsigned int i = 0; i < following.size(); i++) {
                    if (ranks[following.at(i).first] > ranks[current]) {
                        long long unsigned int newDist = currentDist + following.at(i).second;

                        if (newDist < fromDistance[following.at(i).first]) {
                            fromDistance[following.at(i).first] = newDist;
                            fromQueue.push(DijkstraNode(following.at(i).first, newDist));
                            fromPrev[following.at(i).first] = current;
                        }
                    }
                }

                fromQueue.pop();
            } else {
                while (! fromQueue.empty()) {
                    fromQueue.pop();
                }
            }
        }

        if (! toQueue.empty()) {
            unsigned int current = toQueue.top().ID;
            long long unsigned int currentDist = toQueue.top().weight;
            toClosed[current] = true;

            if( currentDist < shortestFound ) {
                if (fromClosed[current]) {
                    if (currentDist + fromDistance[current] < shortestFound) {
                        shortestFound = currentDist + fromDistance[current];
                        meetingNode = current;
                    }
                }

                const vector < pair < unsigned int, long long unsigned int > > & previous = graph.incomingEdges(current);
                for (unsigned int i = 0; i < previous.size(); i++) {
                    if (ranks[previous.at(i).first] > ranks[current]) {
                        long long unsigned int newDist = currentDist + previous.at(i).second;

                        if (newDist < toDistance[previous.at(i).first]) {
                            toDistance[previous.at(i).first] = newDist;
                            toQueue.push(DijkstraNode(previous.at(i).first, newDist));
                            toPrev[previous.at(i).first] = current;
                        }
                    }
                }

                toQueue.pop();
            } else {
                while (! toQueue.empty()) {
                    toQueue.pop();
                }
            }
        }

    }

    outputPath(meetingNode, fromPrev, toPrev);

    delete [] fromPrev;
    delete [] toPrev;
    delete [] fromDistance;
    delete [] toDistance;
    delete [] fromClosed;
    delete [] toClosed;
    return shortestFound;
}

//______________________________________________________________________________________________________________________
void CHPathQueryManager::outputPath(const unsigned int meetingNode, const unsigned int * fromPrev, const unsigned int * toPrev) {
    printf("~~~ Outputting shortest path (unpacked from CH) ~~~\n");
    vector<pair<unsigned int, unsigned int> > fromPath;
    vector<pair<unsigned int, unsigned int> > toPath;
    fillFromPath(meetingNode, fromPrev, fromPath);
    fillToPath(meetingNode, toPrev, toPath);
    unpackPrevious(fromPath);
    unpackFollowing(toPath);
    printf("~~~ End of path output ~~~\n");
}

//______________________________________________________________________________________________________________________
void CHPathQueryManager::fillFromPath(const unsigned int meetingNode, const unsigned int * fromPrev, vector<pair<unsigned int, unsigned int> > & fromPath) {
    unsigned int current = meetingNode;
    while(fromPrev[current] != UINT_MAX) {
        fromPath.push_back(make_pair(fromPrev[current], current));
        current = fromPrev[current];
    }
}

//______________________________________________________________________________________________________________________
void CHPathQueryManager::fillToPath(const unsigned int meetingNode, const unsigned int * toPrev, vector<pair<unsigned int, unsigned int> > & toPath) {
    unsigned int current = meetingNode;
    while(toPrev[current] != UINT_MAX) {
        toPath.push_back(make_pair(current, toPrev[current]));
        current = toPrev[current];
    }
}

//______________________________________________________________________________________________________________________
void CHPathQueryManager::unpackPrevious(vector<pair<unsigned int, unsigned int> > & fromPath) {
    for(int i = fromPath.size()-1; i >= 0; i--) {
        unpackEdge(fromPath[i].first, fromPath[i].second);
    }
}

//______________________________________________________________________________________________________________________
void CHPathQueryManager::unpackFollowing(vector<pair<unsigned int, unsigned int> > & toPath) {
    for(unsigned int i = 0; i < toPath.size(); i++) {
        unpackEdge(toPath[i].first, toPath[i].second);
    }
}

//______________________________________________________________________________________________________________________
void CHPathQueryManager::unpackEdge(unsigned int s, unsigned int t) {
    if (unpackingData.count(make_pair(s, t)) == 0) {
        printf("%u -> %u\n", s, t);
        return;
    }

    unsigned int m = unpackingData.at(make_pair(s, t));
    unpackEdge(s, m);
    unpackEdge(m, t);
}