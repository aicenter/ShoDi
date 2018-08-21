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
vector<pair<pair<unsigned int, unsigned int>, long long unsigned int>> CHPreprocessor::allShortcuts(0);
unordered_map<pair<unsigned int, unsigned int>, long long unsigned int, CHPreprocessor::pair_hash> CHPreprocessor::distances;
unordered_map<pair<unsigned int, unsigned int>, long long unsigned int, CHPreprocessor::pair_hash> CHPreprocessor::distancesWithoutX;
vector<unsigned int> CHPreprocessor::sources(0);
vector<unsigned int> CHPreprocessor::targets(0);
unordered_set<unsigned int> CHPreprocessor::targetsSet;
unordered_map<unsigned int, vector<pair<unsigned int, long long unsigned int > > > CHPreprocessor::buckets;

// This function basically processes the graph by calling all the necessary functions. This variant only creates the
// shortcuts and node ranks, no unpacking data are created so the output can be only used for 'distance' queries
// and not 'path' queries.
//______________________________________________________________________________________________________________________
void CHPreprocessor::preprocessAndSave(string filePath, UpdateableGraph & graph) {
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

// This function basically processes the graph by calling all the necessary functions. This variant creates shortcut
// and node ranks data and also unpacking data for shortcuts. This means that the output can be used both for 'distance'
// and 'path' queries. This variant will take more time and memory than the preprocessAndSave() variant, because
// more information has to be processed and saved.
//______________________________________________________________________________________________________________________
void CHPreprocessor::preprocessAndSaveWithUnpackingData(string filePath, UpdateableGraph & graph) {
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
void CHPreprocessor::flushCHgraph(string & filePath, UpdateableGraph & graph) {
    flushShortcuts(filePath, graph);
    flushCHRanks(filePath);
}

// Simply saves the graph with the shortcuts to a file in a format so that it could be later loaded again.
//______________________________________________________________________________________________________________________
void CHPreprocessor::flushShortcuts(string & filePath, UpdateableGraph & graph) {
    long long unsigned int shortcuts = allShortcuts.size();

    ofstream output;
    output.open(filePath + "_shortcuts");
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (filePath + "_shortcuts").c_str());
    }

    output << shortcuts << endl;
    for(long long unsigned int i = 0; i < shortcuts; i++) {
        output << allShortcuts[i].first.first << " " <<  allShortcuts[i].first.second << " " << allShortcuts[i].second << endl;
    }

    output.close();

}

// Saves the Contraction Hierarchies node ranks to a file.
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

// Saves the unpacking data, which basically means that for every created shortcut during contraction, the node which
// was omitted by the shortcut is saved. Thanks to this data the algorithm can reconstruct the shortest path if the
// actual path is requested and not only distance.
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

// This function is used at the beginning of the preprocessing process. It simply computes the initial weight of each
// node and constructs a priority queue based on those weights.
//______________________________________________________________________________________________________________________
void CHPreprocessor::initializePriorityQueue(CHpriorityQueue & priorityQueue, UpdateableGraph & graph) {
    Timer priorityQTimer("Priority queue initialization");
    priorityQTimer.begin();

    for(unsigned int i = 0; i < graph.nodes(); i++) {
        getPossibleShortcuts(i, graph, true);
        unsigned int shortcuts = calculateShortcutsAmount();
        clearStructures();
        preprocessingDegrees[i] = graph.degree(i);
        int edgeDifference = EdgeDifferenceManager::difference(UINT_MAX, i, shortcuts, preprocessingDegrees[i]);
        priorityQueue.pushOnly(i, edgeDifference);
    }
    priorityQueue.buildProperHeap();

    priorityQTimer.finish();
    priorityQTimer.printMeasuredTime();
}

// This function actually contracts nodes. It simply contracts nodes one after each other until the priority queue is
// empty. In each step, the node with the currently lowest weight has its weight updated, and if it still has the
// lowest weight after the update, it's contracted and it's neighbours weights are updated. If it hasn't the lowest
// weight after the update, we update the new lowest weight node and repeat this process until we find a constant
// minimum.
//______________________________________________________________________________________________________________________
void CHPreprocessor::contractNodes(CHpriorityQueue & priorityQueue, UpdateableGraph & graph) {
    unsigned int CHrank = 1;

    while( ! priorityQueue.empty() ) {
        CHNode current = priorityQueue.front();
        priorityQueue.pop();

        getPossibleShortcuts(current.id, graph, true);
        unsigned int shortcuts = calculateShortcutsAmount();
        int newweight = EdgeDifferenceManager::difference(UINT_MAX, current.id, shortcuts, preprocessingDegrees[current.id]);

        if(newweight > priorityQueue.front().weight) {
            clearStructures();
            priorityQueue.insert(current.id, newweight);
        } else {
            contracted[current.id] = true;
            adjustNeighbourgDegrees(current.id, graph);
            actuallyAddShortcuts(graph);
            clearStructures();
            updateNeighboursPriorities(current.id, graph, priorityQueue);
            nodeRanks[current.id] = CHrank++;

            if(graph.nodes() - CHrank < 2000) {
                //if(CHrank % 10 == 0) {
                printf("Contracted %u nodes!\n", CHrank);
                //}
            } else if(CHrank % 1000 == 0) {
                printf("Contracted %u nodes!\n", CHrank);
            }
        }

    }
}

// This function actually contracts nodes. It simply contracts nodes one after each other until the priority queue is
// empty. In each step, the node with the currently lowest weight has its weight updated, and if it still has the
// lowest weight after the update, it's contracted and it's neighbours weights are updated. If it hasn't the lowest
// weight after the update, we update the new lowest weight node and repeat this process until we find a constant
// minimum. This variant additionaly generates unpacking data.
//______________________________________________________________________________________________________________________
void CHPreprocessor::contractNodesWithUnpackingData(CHpriorityQueue &priorityQueue, UpdateableGraph &graph) {
    unsigned int CHrank = 1;

    while( ! priorityQueue.empty() ) {
        CHNode current = priorityQueue.front();
        priorityQueue.pop();

        getPossibleShortcuts(current.id, graph, true);
        unsigned int shortcuts = calculateShortcutsAmount();
        int newweight = EdgeDifferenceManager::difference(UINT_MAX, current.id, shortcuts, preprocessingDegrees[current.id]);

        if(newweight > priorityQueue.front().weight) {
            clearStructures();
            priorityQueue.insert(current.id, newweight);
        } else {
            contracted[current.id] = true;
            adjustNeighbourgDegrees(current.id, graph);
            actuallyAddShortcutsWithUnpackingData(graph, current.id);
            clearStructures();
            updateNeighboursPriorities(current.id, graph, priorityQueue);
            nodeRanks[current.id] = CHrank++;

            if(graph.nodes() - CHrank < 2000) {
                //if(CHrank % 10 == 0) {
                printf("Contracted %u nodes!\n", CHrank);
                //}
            } else if(CHrank % 1000 == 0) {
                printf("Contracted %u nodes!\n", CHrank);
            }
        }

    }
}

// Helper function that adjust neighbourg degrees when a node is contracted.
//______________________________________________________________________________________________________________________
void CHPreprocessor::adjustNeighbourgDegrees(const unsigned int x, UpdateableGraph & graph) {
    unordered_map < unsigned int, long long unsigned int > previousNodes = graph.incomingEdges(x);
    unordered_map < unsigned int, long long unsigned int > nextNodes = graph.outgoingEdges(x);

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

// This function is called after a node is contracted to recalculate the weights of it's neighbours. Those weights
// might have changed, because those neighbours might now have a different amount of edges than before.
//______________________________________________________________________________________________________________________
void CHPreprocessor::updateNeighboursPriorities(const unsigned int x, UpdateableGraph & graph, CHpriorityQueue & priorityQueue) {
    unordered_map < unsigned int, long long unsigned int > previousNodes = graph.incomingEdges(x);
    unordered_map < unsigned int, long long unsigned int > nextNodes = graph.outgoingEdges(x);

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

// This function will get all possible shortcuts than can be added after the contraction of a certain node i.
//______________________________________________________________________________________________________________________
void CHPreprocessor::getPossibleShortcuts(const unsigned int i, UpdateableGraph & graph, bool deep) {
    getDistancesUsingNode(i, graph);
    CHPreprocessor::contracted[i] = true;
    manyToManyWithBuckets(graph, deep);
    CHPreprocessor::contracted[i] = false;
}

// This function calculates how many of the shortcuts found by the getPossibleShortcuts() function are actually needed.
// This means validating if there exists a path which doesn't contain the contracted node which is shorter or equal
// length as the possible shortcut. This function returns the number of shortcuts that will have to be added.
// Take into consideration that for the Contraction Hierarchies to work correctly, we are pessimistic in a sense that
// we always add shortcuts if we didn't find a path without the contracted node that is shorter or equal length as the
// shortcut. But the path could exist, only we weren't able to find it, as we have a certain hop limit during our many
// to many search.
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

// This function actually adds shortcuts to the graph - this is used when we're actually contracting the node and not
// only calculating it's weight. The shortcuts are only added if they are necessary, that means only if we didn't find
// a shorter or equal length path without the contracted node.
//______________________________________________________________________________________________________________________
void CHPreprocessor::actuallyAddShortcuts(UpdateableGraph & graph) {
    for(auto iter1 = sources.begin(); iter1 != sources.end(); ++iter1) {
        for(auto iter2 = targets.begin(); iter2 != targets.end(); ++iter2) {
            if(*iter1 != *iter2) {
                graph.removeEdge((*iter1), (*iter2));
                if (distancesWithoutX.at(make_pair(*iter1, *iter2)) > distances.at(make_pair(*iter1, *iter2))) {
                    preprocessingDegrees[*iter1]++;
                    preprocessingDegrees[*iter2]++;
                    if ( graph.addEdge(*iter1, *iter2, distances.at(make_pair(*iter1, *iter2))) ) {
                        allShortcuts.push_back(make_pair(make_pair(*iter1, *iter2), distances.at(make_pair(*iter1, *iter2))));
                    }
                }
            }
        }
    }
}

// This function actually adds shortcuts to the graph - this is used when we're actually contracting the node and not
// only calculating it's weight. The shortcuts are only added if they are necessary, that means only if we didn't find
// a shorter or equal length path without the contracted node. This variant additionally saves the unpacking data,
// that means the source and target of the shortcut and the node omitted by the shortcut.
//______________________________________________________________________________________________________________________
void CHPreprocessor::actuallyAddShortcutsWithUnpackingData(UpdateableGraph & graph, unsigned int x) {
    for(auto iter1 = sources.begin(); iter1 != sources.end(); ++iter1) {
        for(auto iter2 = targets.begin(); iter2 != targets.end(); ++iter2) {
            if(*iter1 != *iter2) {
                graph.removeEdge((*iter1), (*iter2));
                if (distancesWithoutX.at(make_pair(*iter1, *iter2)) > distances.at(make_pair(*iter1, *iter2))) {
                    preprocessingDegrees[*iter1]++;
                    preprocessingDegrees[*iter2]++;
                    if( graph.addEdge(*iter1, *iter2, distances.at(make_pair(*iter1, *iter2))) ) {
                        unpacking.push_back(make_pair(make_pair(*iter1, *iter2), x));
                        allShortcuts.push_back(make_pair(make_pair(*iter1, *iter2), distances.at(make_pair(*iter1, *iter2))));
                    }
                }
            }
        }
    }
}

// Auxiliary funtion that clears all the structures between contracting nodes or computing node weights.
//______________________________________________________________________________________________________________________
void CHPreprocessor::clearStructures() {
    distances.clear();
    distancesWithoutX.clear();
    sources.clear();
    targets.clear();
    targetsSet.clear();
    buckets.clear();
}

// This function gets the shortcuts lengths, it takes every pair of neighbours of a node i and computes the weight
// of the shortcut that could be added between those two neighbours if the node i was contracted.
//______________________________________________________________________________________________________________________
void CHPreprocessor::getDistancesUsingNode(const unsigned int i, UpdateableGraph & graph ) {
    unordered_map <unsigned int, long long unsigned int> i_sources = graph.incomingEdges(i);
    unordered_map <unsigned int, long long unsigned int> i_targets = graph.outgoingEdges(i);
    for(auto iter1 = i_sources.begin(); iter1 != i_sources.end(); ++iter1) {
        for(auto iter2 = i_targets.begin(); iter2 != i_targets.end(); ++iter2) {
            if((*iter1).first != (*iter2).first && ! contracted[(*iter1).first] && ! contracted[(*iter2).first]) {
                if (distances.count(make_pair((*iter1).first, (*iter2).first))  == 1) {
                    if((*iter1).second + (*iter2).second < distances.at(make_pair((*iter1).first, (*iter2).first))) {
                        distances[(make_pair((*iter1).first, (*iter2).first))] = (*iter1).second + (*iter2).second;
                    }
                } else {
                    distances.insert(make_pair(make_pair((*iter1).first, (*iter2).first),
                                               (*iter1).second + (*iter2).second));
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

// This function will get a longest shortcut from a specific source to one of the targets, which is then used as an
// upper bound during the oneToManyWithBuckets() function.
//______________________________________________________________________________________________________________________
unsigned long long int CHPreprocessor::longestPossibleShortcut(const unsigned int source) {
    long long unsigned int longest = 0;
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

// This function computes the lengths of shortest paths between all source - target pairs that don't contain the
// contracted node. The search space is limited, so we don't actually always find all shortest path, this isn't
// a problem if we make sure to add a shortcut if we didn't find a shortest path that was shorter or equal length
// as the proposed shortcut.
//______________________________________________________________________________________________________________________
void CHPreprocessor::manyToManyWithBuckets(UpdateableGraph & graph, bool deep) {
    long long unsigned int lowestBucketVal = ULLONG_MAX;
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
        long long unsigned int longestShortcut = longestPossibleShortcut(sources[i]);
        oneToManyWithBuckets(sources[i], longestShortcut - lowestBucketVal, graph, hops, searchspace);
    }
}

// We use a simple extension of the oneToMany algorithm, we do a one edge backwards search from each of the targets
// and save the information into buckets. When expanding nodes during oneToManyWithBuckets(), we can validate if there
// exists a bucket entry for a node we're expanding, if yes, we immediately know that there exists a path from that
// node to some target node and we also have it's length.
//______________________________________________________________________________________________________________________
void CHPreprocessor::initBuckets(const unsigned int x, UpdateableGraph & graph, long long unsigned int & lowestBucketVal) {
    const unordered_map<unsigned int, long long unsigned int> neighbours = graph.incomingEdges(x);
    for(auto iter = neighbours.begin(); iter != neighbours.end(); ++iter) {
        if (! contracted[(*iter).first]) {
            if (buckets.count((*iter).first) == 0) {
                buckets.insert(make_pair((*iter).first, vector<pair<unsigned int, long long unsigned int>>()));
            }
            buckets.at((*iter).first).push_back(make_pair(x, (*iter).second));
            if((*iter).second < lowestBucketVal) {
                lowestBucketVal = (*iter).second;
            }
        }
    }

}

// This function basically takes one source and tries to find shortest paths to all targets. The algorithm is stopped if
// one of those things happen:
// 1) All targets are found
// 2) The current node weight is higher than the upperBound
// 3) We have reached the space search limit - that means we have either expanded to much nodes or all our paths already
//    have more hops (edges) than the limit.
//______________________________________________________________________________________________________________________
void CHPreprocessor::oneToManyWithBuckets(const unsigned int source, const long long unsigned int upperBound, UpdateableGraph & graph, unsigned int hoplimit, unsigned int maxexpanded) {
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

        const unordered_map < unsigned int, long long unsigned int > & neighbours = graph.outgoingEdges(current.ID);
        for ( auto n_iter = neighbours.begin(); n_iter != neighbours.end(); ++n_iter ) {
            if (buckets.count((*n_iter).first) == 1 ) {
                for(auto iter = buckets[(*n_iter).first].begin(); iter != buckets[(*n_iter).first].end(); ++iter) {
                    long long unsigned int newDistUsingBucket = current.weight + (*n_iter).second + (*iter).second;
                    if (newDistUsingBucket < dijkstraDistance[(*iter).first]) {
                        dijkstraDistance[(*iter).first] = newDistUsingBucket;
                        nodesWithChangedDistances.push_back((*iter).first);
                    }
                }
            }
            long long unsigned int newDistance = current.weight + (*n_iter).second;
            if ( ! CHPreprocessor::contracted[(*n_iter).first] && newDistance < dijkstraDistance[(*n_iter).first]) {
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
        dijkstraDistance[nodesWithChangedDistances[i]] = ULLONG_MAX;
    }

}

/*
//______________________________________________________________________________________________________________________
void CHPreprocessor::oneToManyRestrictedDijkstraWithHopLimit(const unsigned int source, const long long unsigned int upperBound, UpdateableGraph & graph, unsigned int hoplimit, unsigned int maxexpanded) {
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

        if (targetsSet.count(current.ID) == 1) {
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
void CHPreprocessor::oneToManyRestrictedDijkstra(const unsigned int source, const long long unsigned int upperBound, UpdateableGraph & graph) {
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

        if (targetsSet.count(current.ID) == 1) {
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
long long unsigned int CHPreprocessor::runRestrictedDijkstra(const unsigned int source, const unsigned int target, const long long unsigned int shortcutLength, const UpdateableGraph & graph) {
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
*/