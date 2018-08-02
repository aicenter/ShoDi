//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#include <queue>
#include <climits>
#include <set>
#include "../Dijkstra/DijkstraNode.h"
#include "CHPreprocessor.h"
#include "EdgeDifferenceManager.h"

using namespace std;

// Initialization only
//______________________________________________________________________________________________________________________
vector<bool> CHPreprocessor::contracted(0);
vector<unsigned int> CHPreprocessor::preprocessingDegrees(0);
vector<unsigned int> CHPreprocessor::nodeRanks(0);

//______________________________________________________________________________________________________________________
void CHPreprocessor::preprocessAndSave(string filePath, Graph & graph) {
    CHpriorityQueue priorityQueue(graph.nodes());

    CHPreprocessor::contracted.resize(graph.nodes(), false);
    CHPreprocessor::preprocessingDegrees.resize(graph.nodes());
    CHPreprocessor::nodeRanks.resize(graph.nodes());
    EdgeDifferenceManager::init(graph.nodes());

    initializePriorityQueue(priorityQueue, graph);
    contractNodes(priorityQueue, graph);

    flushCHgraph(filePath, graph);

    /*for(unsigned int i = 0; i < 20; i++) {
        CHNode t = priorityQueue.front();
        printf("At position %u in priority queue: (priority: %i) node %u\n", i, t.weight, t.id);
        priorityQueue.pop();
    }*/
}

//______________________________________________________________________________________________________________________
void flushCHgraph(string & filePath, Graph & graph) {
    long long unsigned int edges = 0;
    for(unsigned int i = 0; i < graph.nodes(); i++) {
        edges += graph.outgoingEdges(i).size();
    }


}

//______________________________________________________________________________________________________________________
void CHPreprocessor::initializePriorityQueue(CHpriorityQueue & priorityQueue, Graph & graph) {
    for(unsigned int i = 0; i < 20/*graph.nodes()*/; i++) {
        unsigned int shortcuts = calculatePossibleShortcuts(i, graph);
        preprocessingDegrees[i] = graph.degree(i);
        int edgeDifference = EdgeDifferenceManager::difference(UINT_MAX, i, shortcuts, preprocessingDegrees[i]);
        printf("Amount of possible shortcuts for node %u: %u, edge difference: %i\n", i, shortcuts, edgeDifference);
        priorityQueue.pushOnly(i, edgeDifference);
    }
    priorityQueue.buildProperHeap();
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::contractNodes(CHpriorityQueue & priorityQueue, Graph & graph) {
    unsigned int CHrank = 1;

    while( ! priorityQueue.empty() ) {
        CHNode current = priorityQueue.front();
        contractOneNode(current.id, graph);
        updateNeighboursPriorities(current.id, graph, priorityQueue);
        nodeRanks[current.id] = CHrank++;
    }
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::contractOneNode(const unsigned int x, Graph & graph) {
    contracted[x] = true;

    adjustNeighbourgDegrees(x, graph);

    map<pair<unsigned int, unsigned int>, long long unsigned int> distances;
    getDistancesUsingNode(x, graph, distances);

    // actually add shortcuts
    for(auto iter = distances.begin(); iter != distances.end(); ++iter) {
        long long unsigned int shortestPathWithnoutI = runRestrictedDijkstra((*iter).first.first, (*iter).first.second, (*iter).second, graph);
        if (shortestPathWithnoutI > (*iter).second) {
            preprocessingDegrees[(*iter).first.first]++;
            preprocessingDegrees[(*iter).first.second]++;
            graph.addEdge((*iter).first.first, (*iter).first.second, (*iter).second);
        }
    }


}

//______________________________________________________________________________________________________________________
void CHPreprocessor::adjustNeighbourgDegrees(const unsigned int x, Graph & graph) {
    vector < pair < unsigned int, long long unsigned int > > previousNodes = graph.incomingEdges(x);
    vector < pair < unsigned int, long long unsigned int > > nextNodes = graph.outgoingEdges(x);

    for(unsigned int i = 0; i < previousNodes.size(); i++) {
        if (! contracted[previousNodes.at(i).first]) {
            preprocessingDegrees[previousNodes.at(i).first]--;
        }
    }
    for(unsigned int i = 0; i < nextNodes.size(); i++) {
        if (! contracted[nextNodes.at(i).first]) {
            preprocessingDegrees[nextNodes.at(i).first]--;
        }
    }
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::updateNeighboursPriorities(const unsigned int x, Graph & graph, CHpriorityQueue & priorityQueue) {
    vector < pair < unsigned int, long long unsigned int > > previousNodes = graph.incomingEdges(x);
    vector < pair < unsigned int, long long unsigned int > > nextNodes = graph.outgoingEdges(x);

    set < unsigned int > alreadyUpdated;
    for(unsigned int i = 0; i < previousNodes.size(); i++) {
        if (alreadyUpdated.count(previousNodes.at(i).first) == 0 && ! contracted[previousNodes.at(i).first]) {
            unsigned int shortcuts = calculatePossibleShortcuts(previousNodes.at(i).first, graph);
            int newEdgeDifference = EdgeDifferenceManager::difference(x, previousNodes.at(i).first, shortcuts, preprocessingDegrees[previousNodes.at(i).first]);
            priorityQueue.changeValue(previousNodes.at(i).first, newEdgeDifference);
        }
    }
    for(unsigned int i = 0; i < nextNodes.size(); i++) {
        if (alreadyUpdated.count(nextNodes.at(i).first) == 0 && ! contracted[nextNodes.at(i).first]) {
            unsigned int shortcuts = calculatePossibleShortcuts(nextNodes.at(i).first, graph);
            int newEdgeDifference = EdgeDifferenceManager::difference(x, nextNodes.at(i).first, shortcuts, preprocessingDegrees[nextNodes.at(i).first]);
            priorityQueue.changeValue(nextNodes.at(i).first, newEdgeDifference);
        }
    }

}

//______________________________________________________________________________________________________________________
unsigned int CHPreprocessor::calculatePossibleShortcuts(const unsigned int i, Graph & graph) {
    if (CHPreprocessor::contracted[i]) {
        return 0;
    }

    map<pair<unsigned int, unsigned int>, long long unsigned int> distances;
    getDistancesUsingNode(i, graph, distances);

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
void CHPreprocessor::getDistancesUsingNode(const unsigned int i, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distances ) {
    vector< pair <unsigned int, long long unsigned int> > sources = graph.incomingEdges(i);
    vector< pair <unsigned int, long long unsigned int> > targets = graph.outgoingEdges(i);
    for(unsigned int j = 0; j < sources.size(); j++) {
        for(unsigned int k = 0; k < targets.size(); k++) {
            if(sources.at(j).first != targets.at(k).first && ! contracted[sources.at(j).first] && ! contracted[targets.at(k).first]) {
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