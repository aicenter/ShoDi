//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#include <map>
#include <climits>
#include "../Dijkstra/DijkstraNode.h"
#include "CHPreprocessor.h"
#include "EdgeDifferenceManager.h"

using namespace std;

// Initialization only
//______________________________________________________________________________________________________________________
vector<bool> CHPreprocessor::contracted(0);

//______________________________________________________________________________________________________________________
void CHPreprocessor::preprocessAndSave(string filePath, Graph & graph) {
    auto cmp = [](CHNode left, CHNode right) { return (left.weight) > (right.weight);};
    priority_queue<CHNode, vector<CHNode>, bool (*)(CHNode, CHNode)> priorityQueue(cmp);

    CHPreprocessor::contracted.resize(graph.nodes(), false);
    EdgeDifferenceManager::init(graph.nodes());

    initializePriorityQueue(priorityQueue, graph);

    for(unsigned int i = 0; i < 20; i++) {
        CHNode t = priorityQueue.top();
        printf("At position %u in priority queue: (priority: %i) node %u\n", i, t.weight, t.id);
        priorityQueue.pop();
    }
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::initializePriorityQueue(priority_queue<CHNode, vector<CHNode>, bool (*)(CHNode, CHNode)> & priorityQueue, Graph & graph) {
    for(unsigned int i = 0; i < 20/*graph.nodes()*/; i++) {
        unsigned int shortcuts = calculatePossibleShortcuts(i, graph);
        int edgeDifference = EdgeDifferenceManager::difference(UINT_MAX, i, shortcuts, graph.degree(i));
        printf("Amount of possible shortcuts for node %u: %u, edge difference: %i\n", i, shortcuts, edgeDifference);
        priorityQueue.push(CHNode(i, edgeDifference));
    }
}

//______________________________________________________________________________________________________________________
unsigned int CHPreprocessor::calculatePossibleShortcuts(const unsigned int i, Graph & graph) {
    if (CHPreprocessor::contracted[i]) {
        return 0;
    }

    vector< pair <unsigned int, long long unsigned int> > sources = graph.incomingEdges(i);
    vector< pair <unsigned int, long long unsigned int> > targets = graph.outgoingEdges(i);
    map<pair<unsigned int, unsigned int>, long long unsigned int> distances;
    for(unsigned int j = 0; j < sources.size(); j++) {
        for(unsigned int k = 0; k < targets.size(); k++) {
            if(sources.at(j).first != targets.at(k).first) {
                if (distances.count(make_pair(sources.at(j).first, targets.at(k).first))  == 1) {
                    if(sources.at(j).second + targets.at(k).second < distances.at(make_pair(sources.at(j).first, targets.at(k).first))) {
                        distances[(make_pair(sources.at(j).first, targets.at(k).first))] = sources.at(j).second + targets.at(k).second;
                    }
                } else {
                    distances.insert(make_pair(make_pair(sources.at(j).first, targets.at(k).first),
                                               sources.at(j).second + targets.at(k).second));
                }
            }
        }
    }

    unsigned int addedShortcuts = 0;
    CHPreprocessor::contracted[i] = true;

    for(auto iter = distances.begin(); iter != distances.end(); ++iter) {
        long long unsigned int shortestPathWithnoutI = runRestrictedDijkstra((*iter).first.first, (*iter).first.second, (*iter).second, graph);
        if (shortestPathWithnoutI > (*iter).second) {
            addedShortcuts++;
        }
    }

    CHPreprocessor::contracted[i] = false;

    return addedShortcuts;
}

//______________________________________________________________________________________________________________________
long long unsigned int CHPreprocessor::runRestrictedDijkstra(const unsigned int source, const unsigned int target, const long long unsigned int shortcutLength, const Graph & graph) {
    unsigned int n = graph.nodes();
    long long unsigned int * distance = new long long unsigned int[n];

    for(unsigned int i = 0; i < n; i++) {
        distance[i] = ULLONG_MAX;
    }

    distance[source] = 0;

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) < (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(source, 0));

    while(! q.empty() ) {
        const DijkstraNode & current = q.top();

        if (current.weight > shortcutLength) {
            delete [] distance;
            return ULLONG_MAX;
        }

        if (current.ID == target) {
            delete [] distance;
            return current.weight;
        }

        const vector < pair < unsigned int, long long unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
        for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
            long long unsigned int newDistance = current.weight + neighbours.at(i).second;
            if ( ! CHPreprocessor::contracted[neighbours.at(i).first] && newDistance < distance[neighbours.at(i).first]) {
                distance[neighbours.at(i).first] = newDistance;
                q.push(DijkstraNode(neighbours.at(i).first, newDistance));
            }
        }

        q.pop();

    }

    delete [] distance;
    return ULLONG_MAX;
}