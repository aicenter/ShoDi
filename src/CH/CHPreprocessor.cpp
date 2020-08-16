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

//______________________________________________________________________________________________________________________
vector<bool> CHPreprocessor::contracted(0);
vector<unsigned int> CHPreprocessor::preprocessingDegrees(0);
vector<unsigned int> CHPreprocessor::dijkstraDistance(0);
vector<ShortcutEdge> CHPreprocessor::allShortcuts;
unordered_map<pair<unsigned int, unsigned int>, unsigned int, CHPreprocessor::pair_hash> CHPreprocessor::distances;
unordered_map<pair<unsigned int, unsigned int>, unsigned int, CHPreprocessor::pair_hash> CHPreprocessor::distancesWithoutX;
vector<unsigned int> CHPreprocessor::sources(0);
vector<unsigned int> CHPreprocessor::targets(0);
unordered_set<unsigned int> CHPreprocessor::targetsSet;
unordered_map<unsigned int, vector<pair<unsigned int, unsigned int > > > CHPreprocessor::buckets;

//______________________________________________________________________________________________________________________
void CHPreprocessor::preprocessForDDSG(UpdateableGraph & graph) {
    printf("Started preprocessing!\n");
    Timer preprocessTimer("Contraction Hierarchies preprocessing timer");
    preprocessTimer.begin();

    CHpriorityQueue priorityQueue(graph.nodes());

    CHPreprocessor::contracted.resize(graph.nodes(), false);
    CHPreprocessor::preprocessingDegrees.resize(graph.nodes());
    CHPreprocessor::dijkstraDistance.resize(graph.nodes(), UINT_MAX);
    EdgeDifferenceManager::init(graph.nodes());

    initializePriorityQueue(priorityQueue, graph);

    contractNodesWithUnpackingData(priorityQueue, graph);
    reinsertShortcuts(graph);

    preprocessTimer.finish();
    preprocessTimer.printMeasuredTime();

    printf("During the preprocessing process, %lu shortcuts were added into the graph.\n", (unsigned long) allShortcuts.size());

    allShortcuts.clear();
    preprocessingDegrees.clear();
    contracted.clear();
    dijkstraDistance.clear();
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::initializePriorityQueue(CHpriorityQueue & priorityQueue, UpdateableGraph & graph) {
    printf("Initializing priority queue.\n");

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        getPossibleShortcuts(i, graph, true);
        unsigned int shortcuts = calculateShortcutsAmount();
        clearStructures();
        preprocessingDegrees[i] = graph.degree(i);
        int edgeDifference = EdgeDifferenceManager::difference(UINT_MAX, i, shortcuts, preprocessingDegrees[i]);
        priorityQueue.pushOnly(i, edgeDifference);
    }
    priorityQueue.buildProperHeap();
    printf("\rPriority queue initialized.\n");


}

//______________________________________________________________________________________________________________________
void CHPreprocessor::contractNodesWithUnpackingData(CHpriorityQueue &priorityQueue, UpdateableGraph &graph) {
    unsigned int CHrank = 1;

    while( ! priorityQueue.empty() ) {
        CHNode current = priorityQueue.front();
        priorityQueue.pop();

        getPossibleShortcuts(current.id, graph, true);
        unsigned int shortcuts = calculateShortcutsAmount();
        int newweight = EdgeDifferenceManager::difference(UINT_MAX, current.id, shortcuts, preprocessingDegrees[current.id]);

        if( ! priorityQueue.empty() && newweight > priorityQueue.front().weight) {
            clearStructures();
            priorityQueue.insert(current.id, newweight);
        } else {
            contracted[current.id] = true;
            adjustNeighboursDegrees(current.id, graph);
            actuallyAddShortcutsWithUnpackingData(graph, current.id);
            removeContractedNodeEdges(graph, current.id);
            clearStructures();
            updateNeighboursPriorities(current.id, graph, priorityQueue);
            graph.setRank(current.id, CHrank++);

            if(graph.nodes() - CHrank < 2000) {
                if(CHrank % 10 == 0) {
                    printf("\rContracted %u nodes!", CHrank);
                }
            } else if(CHrank % 1000 == 0) {
                printf("\rContracted %u nodes!", CHrank);
            }
        }

    }

    printf("\rAll nodes contracted!\n");
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::reinsertShortcuts(UpdateableGraph & graph) {
    for(unsigned int i = 0; i < allShortcuts.size(); i++) {
        graph.addShortcutEdge(allShortcuts[i].sourceNode, allShortcuts[i].targetNode, allShortcuts[i].weight, allShortcuts[i].middleNode);
    }
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::adjustNeighboursDegrees(const unsigned int x, UpdateableGraph & graph) {
    unordered_map < unsigned int, unsigned int > previousNodes = graph.incomingEdges(x);
    unordered_map < unsigned int, PreprocessingEdgeData > nextNodes = graph.outgoingEdges(x);

    for(auto iter = previousNodes.begin(); iter != previousNodes.end(); ++iter) {
        if (! contracted[(*iter).first]) {
            preprocessingDegrees[(*iter).first]--;
        }
    }
    for(auto iter = nextNodes.begin(); iter != nextNodes.end(); ++iter) {
        if (! contracted[(*iter).first]) {
            preprocessingDegrees[(*iter).first]--;
        }
    }
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::updateNeighboursPriorities(const unsigned int x, UpdateableGraph & graph, CHpriorityQueue & priorityQueue) {
    unordered_map < unsigned int, unsigned int > previousNodes = graph.incomingEdges(x);
    unordered_map < unsigned int, PreprocessingEdgeData > nextNodes = graph.outgoingEdges(x);

    unordered_set < unsigned int > alreadyUpdated;
    for(auto iter = previousNodes.begin(); iter != previousNodes.end(); ++iter) {
        if (alreadyUpdated.count((*iter).first) == 0 && ! contracted[(*iter).first]) {
            getPossibleShortcuts((*iter).first, graph, true);
            unsigned int shortcuts = calculateShortcutsAmount();
            clearStructures();
            int newEdgeDifference = EdgeDifferenceManager::difference(x, (*iter).first, shortcuts, preprocessingDegrees[(*iter).first]);
            priorityQueue.changeValue((*iter).first, newEdgeDifference);
        }
    }
    for(auto iter = nextNodes.begin(); iter != nextNodes.end(); ++iter) {
        if (alreadyUpdated.count((*iter).first) == 0 && ! contracted[(*iter).first]) {
            getPossibleShortcuts((*iter).first, graph, true);
            unsigned int shortcuts = calculateShortcutsAmount();
            clearStructures();
            int newEdgeDifference = EdgeDifferenceManager::difference(x, (*iter).first, shortcuts, preprocessingDegrees[(*iter).first]);
            priorityQueue.changeValue((*iter).first, newEdgeDifference);
        }
    }

}

//______________________________________________________________________________________________________________________
void CHPreprocessor::getPossibleShortcuts(const unsigned int i, UpdateableGraph & graph, bool deep) {
    getDistancesUsingNode(i, graph);
    CHPreprocessor::contracted[i] = true;
    manyToManyWithBuckets(graph, deep);
    CHPreprocessor::contracted[i] = false;
}

//______________________________________________________________________________________________________________________
unsigned int CHPreprocessor::calculateShortcutsAmount() {
    unsigned int addedShortcuts = 0;
    for(auto iter1 = sources.begin(); iter1 != sources.end(); ++iter1) {
        for(auto iter2 = targets.begin(); iter2 != targets.end(); ++iter2) {
            if(*iter1 != *iter2) {
                if (distancesWithoutX.at(make_pair(*iter1, *iter2)) > distances.at(make_pair(*iter1, *iter2))) {
                    addedShortcuts++;
                }
            }
        }
    }

    return addedShortcuts;
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::actuallyAddShortcutsWithUnpackingData(UpdateableGraph & graph, unsigned int x) {
    for(auto iter1 = sources.begin(); iter1 != sources.end(); ++iter1) {
        for(auto iter2 = targets.begin(); iter2 != targets.end(); ++iter2) {
            if(*iter1 != *iter2) {
                if (distancesWithoutX.at(make_pair(*iter1, *iter2)) > distances.at(make_pair(*iter1, *iter2))) {
                    preprocessingDegrees[*iter1]++;
                    preprocessingDegrees[*iter2]++;
                    if (graph.addShortcutEdge(*iter1, *iter2, distances.at(make_pair(*iter1, *iter2)), x)) {
                        allShortcuts.push_back(ShortcutEdge((*iter1), (*iter2), distances.at(make_pair(*iter1, *iter2)), x));
                    }
                }
            }
        }
    }
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::removeContractedNodeEdges(UpdateableGraph & graph, unsigned int x) {
    for(auto iter = sources.begin(); iter != sources.end(); ++iter) {
        graph.removeEdge((*iter), x);
    }

    for(auto iter = targets.begin(); iter != targets.end(); ++iter) {
        graph.removeEdge(x, (*iter));
    }
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::clearStructures() {
    distances.clear();
    distancesWithoutX.clear();
    sources.clear();
    targets.clear();
    targetsSet.clear();
    buckets.clear();
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::getDistancesUsingNode(const unsigned int i, UpdateableGraph & graph ) {
    unordered_map <unsigned int, unsigned int> i_sources = graph.incomingEdges(i);
    unordered_map <unsigned int, PreprocessingEdgeData> i_targets = graph.outgoingEdges(i);
    for(auto iter1 = i_sources.begin(); iter1 != i_sources.end(); ++iter1) {
        for(auto iter2 = i_targets.begin(); iter2 != i_targets.end(); ++iter2) {
            if((*iter1).first != (*iter2).first && ! contracted[(*iter1).first] && ! contracted[(*iter2).first]) {
                if (distances.count(make_pair((*iter1).first, (*iter2).first))  == 1) {
                    if((*iter1).second + (*iter2).second.weight < distances.at(make_pair((*iter1).first, (*iter2).first))) {
                        distances[(make_pair((*iter1).first, (*iter2).first))] = (*iter1).second + (*iter2).second.weight;
                    }
                } else {
                    distances.insert(make_pair(make_pair((*iter1).first, (*iter2).first),
                                               (*iter1).second + (*iter2).second.weight));
                }
            }
        }
    }

    for(auto iter = i_sources.begin(); iter != i_sources.end(); ++iter) {
        if (! contracted[(*iter).first]) {
            sources.push_back((*iter).first);
        }
    }
    for(auto iter = i_targets.begin(); iter != i_targets.end(); ++iter) {
        if (! contracted[(*iter).first]) {
            targets.push_back((*iter).first);
        }
    }

    targetsSet.insert(targets.begin(), targets.end());
}

//______________________________________________________________________________________________________________________
unsigned int CHPreprocessor::longestPossibleShortcut(const unsigned int source) {
    unsigned int longest = 0;
    for(auto iter = targets.begin(); iter != targets.end(); ++iter) {
        //printf("Will find distance for: %u -> %u\n", source, *iter);
        if(*iter != source) {
            if (distances.at(make_pair(source, *iter)) > longest) {
                longest = distances.at(make_pair(source, (*iter)));
            }
        }
    }
    return longest;
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::manyToManyWithBuckets(UpdateableGraph & graph, bool deep) {
    unsigned int lowestBucketVal = UINT_MAX;
    for(unsigned int i = 0; i < targets.size(); i++) {
        initBuckets(targets[i], graph, lowestBucketVal);
    }

    unsigned int searchspace = 1000;
    unsigned int hops = 4;
    if (! deep) {
        searchspace = 100;
        hops = 2;
    }

    for(unsigned int i = 0; i < sources.size(); i++) {
        unsigned int longestShortcut = longestPossibleShortcut(sources[i]);
        oneToManyWithBuckets(sources[i], longestShortcut - lowestBucketVal, graph, hops, searchspace);
    }
}

//______________________________________________________________________________________________________________________
void CHPreprocessor::initBuckets(const unsigned int x, UpdateableGraph & graph, unsigned int & lowestBucketVal) {
    const unordered_map<unsigned int, unsigned int> neighbours = graph.incomingEdges(x);
    for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
        if (! contracted[(*iter).first]) {
            if (buckets.count((*iter).first) == 0) {
                buckets.insert(make_pair((*iter).first, vector<pair<unsigned int, unsigned int>>()));
            }
            buckets.at((*iter).first).push_back(make_pair(x, (*iter).second));
            if((*iter).second < lowestBucketVal) {
                lowestBucketVal = (*iter).second;
            }
        }
    }

}

//______________________________________________________________________________________________________________________
void CHPreprocessor::oneToManyWithBuckets(const unsigned int source, const unsigned int upperBound, UpdateableGraph & graph, unsigned int hoplimit, unsigned int maxexpanded) {
    vector<unsigned int> nodesWithChangedDistances;
    unsigned int targetsAmount = (unsigned int) targets.size();
    unsigned int targetsFound = 0;
    unsigned int expanded = 0;

    dijkstraDistance[source] = 0;
    nodesWithChangedDistances.push_back(source);

    auto cmp = [](HopsDijkstraNode left, HopsDijkstraNode right) { return (left.weight) > (right.weight);};
    priority_queue<HopsDijkstraNode, vector<HopsDijkstraNode>, decltype(cmp)> q(cmp);
    q.push(HopsDijkstraNode(source, 0, 0));

    while(! q.empty() ) {
        const HopsDijkstraNode current = q.top();
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

        if (targetsSet.count(current.ID) == 1) {
            targetsFound++;
            if (targetsFound == targetsAmount) {
                break;
            }
        }

        const unordered_map < unsigned int, PreprocessingEdgeData > & neighbours = graph.outgoingEdges(current.ID);
        for ( auto n_iter = neighbours.begin(); n_iter != neighbours.end(); ++n_iter ) {
            if (buckets.count((*n_iter).first) == 1 ) {
                for(auto iter = buckets[(*n_iter).first].begin(); iter != buckets[(*n_iter).first].end(); ++iter) {
                    unsigned int newDistUsingBucket = current.weight + (*n_iter).second.weight + (*iter).second;
                    if (newDistUsingBucket < dijkstraDistance[(*iter).first]) {
                        dijkstraDistance[(*iter).first] = newDistUsingBucket;
                        nodesWithChangedDistances.push_back((*iter).first);
                    }
                }
            }
            unsigned int newDistance = current.weight + (*n_iter).second.weight;
            if (! CHPreprocessor::contracted[(*n_iter).first] && newDistance < dijkstraDistance[(*n_iter).first]) {
                dijkstraDistance[(*n_iter).first] = newDistance;
                nodesWithChangedDistances.push_back((*n_iter).first);
                q.push(HopsDijkstraNode((*n_iter).first, newDistance, current.hops + 1));
            }
        }

        q.pop();

    }

    for(auto iter = targets.begin(); iter != targets.end(); ++iter) {
        distancesWithoutX.insert(make_pair(make_pair(source, *iter), dijkstraDistance[*iter]));
    }

    for(unsigned int i = 0; i < nodesWithChangedDistances.size(); i++) {
        dijkstraDistance[nodesWithChangedDistances[i]] = UINT_MAX;
    }

}
