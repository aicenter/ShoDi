//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#include <queue>
#include <climits>
#include <fstream>
#include "../Timer/Timer.h"
#include "../Dijkstra/DijkstraNode.h"
#include "Structures/HopsDijkstraNode.h"
#include "CHPreprocessor.h"
#include "EdgeDifferenceManager.h"

using namespace std;

// Initialization only
//______________________________________________________________________________________________________________________
vector<bool> CHPreprocessor::contracted(0);
vector<unsigned int> CHPreprocessor::preprocessingDegrees(0);
vector<unsigned int> CHPreprocessor::nodeRanks(0);
vector<long long unsigned int> CHPreprocessor::dijkstraDistance(0);
vector<pair<pair<unsigned int, unsigned int>, unsigned int>> CHPreprocessor::unpacking(0);

//______________________________________________________________________________________________________________________
void CHPreprocessor::preprocessAndSave(string filePath, Graph & graph) {
    printf("Started preprocessing!\n");
    Timer preprocessTimer("Contraction Hierarchies preprocessing timer");
    preprocessTimer.begin();

    CHpriorityQueue priorityQueue(graph.nodes());

    CHPreprocessor::contracted.resize(graph.nodes(), false);
    CHPreprocessor::preprocessingDegrees.resize(graph.nodes());
    CHPreprocessor::nodeRanks.resize(graph.nodes());
    CHPreprocessor::dijkstraDistance.resize(graph.nodes(), ULLONG_MAX);
    EdgeDifferenceManager::init(graph.nodes());

    initializePriorityQueue(priorityQueue, graph);
    printf("Initialized priority queue!\n");
    contractNodes(priorityQueue, graph);

    preprocessTimer.finish();
    preprocessTimer.printMeasuredTime();

    printf("Now writing preprocessed data into files.\n");
    Timer writeTimer("Contraction Hierarchies file writing timer");
    writeTimer.begin();

    flushCHgraph(filePath, graph);

    writeTimer.finish();
    writeTimer.printMeasuredTime();
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::preprocessAndSaveWithUnpackingData(string filePath, Graph & graph) {
    printf("Started preprocessing!\n");
    Timer preprocessTimer("Contraction Hierarchies preprocessing timer");
    preprocessTimer.begin();

    CHpriorityQueue priorityQueue(graph.nodes());

    CHPreprocessor::contracted.resize(graph.nodes(), false);
    CHPreprocessor::preprocessingDegrees.resize(graph.nodes());
    CHPreprocessor::nodeRanks.resize(graph.nodes());
    CHPreprocessor::dijkstraDistance.resize(graph.nodes(), ULLONG_MAX);
    EdgeDifferenceManager::init(graph.nodes());

    initializePriorityQueue(priorityQueue, graph);
    printf("Initialized priority queue!\n");
    contractNodesWithUnpackingData(priorityQueue, graph);

    preprocessTimer.finish();
    preprocessTimer.printMeasuredTime();

    printf("Now writing preprocessed data into files.\n");
    Timer writeTimer("Contraction Hierarchies file writing timer");
    writeTimer.begin();

    flushCHgraph(filePath, graph);
    flushUnpackingData(filePath);

    writeTimer.finish();
    writeTimer.printMeasuredTime();
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::flushCHgraph(string & filePath, Graph & graph) {
    flushGraph(filePath, graph);
    flushCHRanks(filePath);
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::flushGraph(string & filePath, Graph & graph) {
    long long unsigned int edges = 0;
    for(unsigned int i = 0; i < graph.nodes(); i++) {
        edges += graph.outgoingEdges(i).size();
    }

    ofstream output;
    output.open(filePath + "_graph");
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (filePath + "_graph").c_str());
    }

    output << "c This file was created by the preprocessing process of Contraction Hiearchies." << endl;
    output << "c This graph file contains original edges plus CH generated shortcuts." << endl;
    output << "c This file should be used in pair with the _ranks file for expected results." << endl;
    output << "p sp " << graph.nodes() << " " << edges << endl;
    for(unsigned int i = 0; i < graph.nodes(); i++) {
        vector < pair < unsigned int, long long unsigned int > > edges = graph.outgoingEdges(i);
        for(unsigned int j = 0; j < edges.size(); j++) {
            output << "a " << i+1 << " " << edges.at(j).first + 1 << " " << edges.at(j).second << endl;
        }
    }

    output.close();

}

//______________________________________________________________________________________________________________________
void CHPreprocessor::flushCHRanks(string & filePath) {
    ofstream output;
    output.open(filePath + "_ranks");
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (filePath + "_ranks").c_str());
    }

    output << nodeRanks.size() << endl;
    for (unsigned int i = 0; i < nodeRanks.size(); i++ ) {
        output << nodeRanks[i] << endl;
    }

    output.close();


}

//______________________________________________________________________________________________________________________
void CHPreprocessor::flushUnpackingData(string & filePath) {
    ofstream output;
    output.open(filePath + "_unpacking");
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (filePath + "_ranks").c_str());
    }

    output << unpacking.size() << endl;
    for ( unsigned int i = 0; i < unpacking.size(); i++ ) {
        output << unpacking[i].first.first << " " << unpacking[i].first.second << " " << unpacking[i].second << endl;
    }

    output.close();


}

//______________________________________________________________________________________________________________________
void CHPreprocessor::initializePriorityQueue(CHpriorityQueue & priorityQueue, Graph & graph) {
    Timer priorityQTimer("Priority queue initialization");
    priorityQTimer.begin();

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        unsigned int shortcuts = calculatePossibleShortcuts(i, graph);
        preprocessingDegrees[i] = graph.degree(i);
        int edgeDifference = EdgeDifferenceManager::difference(UINT_MAX, i, shortcuts, preprocessingDegrees[i]);
        priorityQueue.pushOnly(i, edgeDifference);
    }
    priorityQueue.buildProperHeap();

    priorityQTimer.finish();
    priorityQTimer.printMeasuredTime();
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::contractNodes(CHpriorityQueue & priorityQueue, Graph & graph) {
    unsigned int CHrank = 1;

    while( ! priorityQueue.empty() ) {
        //priorityQueue.printSomeInfo();
        CHNode current = priorityQueue.front();
        priorityQueue.pop();
        contractOneNode(current.id, graph);
        updateNeighboursPriorities(current.id, graph, priorityQueue);
        nodeRanks[current.id] = CHrank++;



        if(CHrank % 1000 == 0) {
            printf("Contracted %u nodes!\n", CHrank);
        }
    }
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::contractNodesWithUnpackingData(CHpriorityQueue &priorityQueue, Graph &graph) {
    unsigned int CHrank = 1;

    while( ! priorityQueue.empty() ) {
        //priorityQueue.printSomeInfo();
        CHNode current = priorityQueue.front();
        priorityQueue.pop();
        contractOneNodeWithUnpackingData(current.id, graph);
        updateNeighboursPriorities(current.id, graph, priorityQueue);
        nodeRanks[current.id] = CHrank++;



        if(CHrank % 1000 == 0) {
            printf("Contracted %u nodes!\n", CHrank);
        }
    }
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::contractOneNode(const unsigned int x, Graph & graph) {
    contracted[x] = true;
    //printf("Contracting node %u!\n", x);
    adjustNeighbourgDegrees(x, graph);

    map<pair<unsigned int, unsigned int>, long long unsigned int> distances;
    map<unsigned int, set<unsigned int> > sourcesAndTargets;
    getDistancesUsingNode(x, graph, distances, sourcesAndTargets);

    // actually add shortcuts
    for (auto iter = sourcesAndTargets.begin(); iter != sourcesAndTargets.end(); ++iter) {
        long long unsigned int upperBound = longestPossibleShortcut((*iter).first, (*iter).second, distances);
        map<pair<unsigned int, unsigned int>, long long unsigned int> distancesWithoutX;
        oneToManyRestrictedDijkstraWithHopLimit((*iter).first, (*iter).second, upperBound, graph, distancesWithoutX);

        for(auto iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); ++iter2) {
            if (distancesWithoutX.at(make_pair((*iter).first, *iter2)) > distances.at(make_pair((*iter).first, *iter2))) {
                preprocessingDegrees[(*iter).first]++;
                preprocessingDegrees[*iter2]++;
                graph.addEdge((*iter).first, *iter2, distances.at(make_pair((*iter).first, *iter2)));
            }
        }
    }

}

//______________________________________________________________________________________________________________________
void CHPreprocessor::contractOneNodeWithUnpackingData(const unsigned int x, Graph &graph) {
    contracted[x] = true;
    //printf("Contracting node %u!\n", x);
    adjustNeighbourgDegrees(x, graph);

    map<pair<unsigned int, unsigned int>, long long unsigned int> distances;
    map<unsigned int, set<unsigned int> > sourcesAndTargets;
    getDistancesUsingNode(x, graph, distances, sourcesAndTargets);

    // actually add shortcuts
    for (auto iter = sourcesAndTargets.begin(); iter != sourcesAndTargets.end(); ++iter) {
        long long unsigned int upperBound = longestPossibleShortcut((*iter).first, (*iter).second, distances);
        map<pair<unsigned int, unsigned int>, long long unsigned int> distancesWithoutX;
        oneToManyRestrictedDijkstraWithHopLimit((*iter).first, (*iter).second, upperBound, graph, distancesWithoutX);

        for(auto iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); ++iter2) {
            if (distancesWithoutX.at(make_pair((*iter).first, *iter2)) > distances.at(make_pair((*iter).first, *iter2))) {
                preprocessingDegrees[(*iter).first]++;
                preprocessingDegrees[*iter2]++;
                graph.addEdge((*iter).first, *iter2, distances.at(make_pair((*iter).first, *iter2)));
                unpacking.push_back(make_pair(make_pair((*iter).first, *iter2), x));
            }
        }
    }

}

//______________________________________________________________________________________________________________________
void CHPreprocessor::adjustNeighbourgDegrees(const unsigned int x, Graph & graph) {
    vector < pair < unsigned int, long long unsigned int > > previousNodes = graph.incomingEdges(x);
    vector < pair < unsigned int, long long unsigned int > > nextNodes = graph.outgoingEdges(x);

    //printf("Contracting node %u, has %lu/%lu neigbours.\n", x, previousNodes.size(), nextNodes.size());
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
    map<unsigned int, set<unsigned int> > sourcesAndTargets;
    getDistancesUsingNode(i, graph, distances, sourcesAndTargets);

    unsigned int addedShortcuts = 0;
    CHPreprocessor::contracted[i] = true;

    for (auto iter = sourcesAndTargets.begin(); iter != sourcesAndTargets.end(); ++iter) {
        long long unsigned int upperBound = longestPossibleShortcut((*iter).first, (*iter).second, distances);
        map<pair<unsigned int, unsigned int>, long long unsigned int> distancesWithoutX;
        oneToManyRestrictedDijkstraWithHopLimit((*iter).first, (*iter).second, upperBound, graph, distancesWithoutX);

        for(auto iter2 = (*iter).second.begin(); iter2 != (*iter).second.end(); ++iter2) {
            if (distancesWithoutX.at(make_pair((*iter).first, *iter2)) > distances.at(make_pair((*iter).first, *iter2))) {
                addedShortcuts++;
            }
        }
    }

    CHPreprocessor::contracted[i] = false;

    return addedShortcuts;
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::getDistancesUsingNode(const unsigned int i, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distances, map<unsigned int, set<unsigned int> > & souAndTar ) {
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
                    if(souAndTar.count(sources.at(j).first) == 1) {
                        souAndTar.at(sources.at(j).first).insert(targets.at(k).first);
                    } else {
                        souAndTar.insert(make_pair(sources.at(j).first, set<unsigned int>()));
                        souAndTar.at(sources.at(j).first).insert(targets.at(k).first);
                    }
                }
            }
        }
    }
}

//______________________________________________________________________________________________________________________
unsigned long long int CHPreprocessor::longestPossibleShortcut(const unsigned int source, set < unsigned int > & targets, map < pair < unsigned int, unsigned int >, unsigned long long int > & distances) {
    long long unsigned int longest = 0;
    for(auto iter = targets.begin(); iter != targets.end(); ++iter) {
        if(distances.at(make_pair(source, (*iter))) > longest) {
            longest = distances.at(make_pair(source, (*iter)));
        }
    }
    return longest;
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::oneToManyRestrictedDijkstraWithHopLimit(const unsigned int source, set<unsigned int> & targets, const long long unsigned int upperBound, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distancesWithoutX, unsigned int hoplimit, unsigned int maxexpanded) {
    vector<unsigned int> nodesWithChangedDistances;
    unsigned int targetsAmount = targets.size();
    unsigned int targetsFound = 0;
    unsigned int expanded = 0;

    dijkstraDistance[source] = 0;
    nodesWithChangedDistances.push_back(source);

    auto cmp = [](HopsDijkstraNode left, HopsDijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<HopsDijkstraNode, vector<HopsDijkstraNode>, decltype(cmp)> q(cmp);
    q.push(HopsDijkstraNode(source, 0, 0));

    while(! q.empty() ) {
        const HopsDijkstraNode & current = q.top();
        expanded++;
        if (expanded > maxexpanded) {
            break;
        }

        if (current.weight > upperBound) {
            break;
        }

        if (current.hops > hoplimit) {
            q.pop();
            continue;
        }

        if (targets.count(current.ID) == 1) {
            targetsFound++;
            if (targetsFound == targetsAmount) {
                break;
            }
        }

        const vector < pair < unsigned int, long long unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
        for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
            long long unsigned int newDistance = current.weight + neighbours.at(i).second;
            if ( ! CHPreprocessor::contracted[neighbours.at(i).first] && newDistance < dijkstraDistance[neighbours.at(i).first]) {
                dijkstraDistance[neighbours.at(i).first] = newDistance;
                nodesWithChangedDistances.push_back(neighbours.at(i).first);
                q.push(HopsDijkstraNode(neighbours.at(i).first, newDistance, current.hops + 1));
            }
        }

        q.pop();

    }

    for(auto iter = targets.begin(); iter != targets.end(); ++iter) {
        distancesWithoutX.insert(make_pair(make_pair(source, *iter), dijkstraDistance[*iter]));
    }

    for(unsigned int i = 0; i < nodesWithChangedDistances.size(); i++) {
        dijkstraDistance[nodesWithChangedDistances[i]] = ULLONG_MAX;
    }

}

//______________________________________________________________________________________________________________________
void CHPreprocessor::oneToManyRestrictedDijkstra(const unsigned int source, set<unsigned int> & targets, const long long unsigned int upperBound, Graph & graph, map<pair<unsigned int, unsigned int>, long long unsigned int> & distancesWithoutX) {
    vector<unsigned int> nodesWithChangedDistances;
    unsigned int targetsAmount = targets.size();
    unsigned int targetsFound = 0;

    dijkstraDistance[source] = 0;
    nodesWithChangedDistances.push_back(source);

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<DijkstraNode, vector<DijkstraNode>, decltype(cmp)> q(cmp);
    q.push(DijkstraNode(source, 0));

    while(! q.empty() ) {
        const DijkstraNode & current = q.top();

        if (current.weight > upperBound) {
            break;
        }

        if (targets.count(current.ID) == 1) {
            targetsFound++;
            if (targetsFound == targetsAmount) {
                break;
            }
        }

        const vector < pair < unsigned int, long long unsigned int > > & neighbours = graph.outgoingEdges(current.ID);
        for ( unsigned int i = 0; i < neighbours.size(); i++ ) {
            long long unsigned int newDistance = current.weight + neighbours.at(i).second;
            if ( ! CHPreprocessor::contracted[neighbours.at(i).first] && newDistance < dijkstraDistance[neighbours.at(i).first]) {
                dijkstraDistance[neighbours.at(i).first] = newDistance;
                nodesWithChangedDistances.push_back(neighbours.at(i).first);
                q.push(DijkstraNode(neighbours.at(i).first, newDistance));
            }
        }

        q.pop();

    }

    for(auto iter = targets.begin(); iter != targets.end(); ++iter) {
        distancesWithoutX.insert(make_pair(make_pair(source, *iter), dijkstraDistance[*iter]));
    }

    for(unsigned int i = 0; i < nodesWithChangedDistances.size(); i++) {
        dijkstraDistance[nodesWithChangedDistances[i]] = ULLONG_MAX;
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

    auto cmp = [](DijkstraNode left, DijkstraNode right) { return (left.weight) > (right.weight);};
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
