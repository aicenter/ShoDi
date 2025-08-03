/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#ifndef TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
#define TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include "../GraphBuilding/Structures/UpdateableGraph.h"
#include "../GraphBuilding/Structures/ShortcutEdge.h"
#include "Structures/CHNode.h"
#include "Structures/CHpriorityQueue.h"
#include "Structures/CH_Graph.h"


/*
 * This class is responsible for preprocessing a given graph for the Contraction Hierarchies.
 */
class CHPreprocessor {
public:
    /**
     * The 'main' preprocessing function. The result of calling this function is the UpdateableGraph containing all the
     * given shortcuts. During this function, the original edges will be removed from the graph, so don't forget to
     * add the original edges back into the graph before flushing it.
     *
     * @param graph[in, out] The input graph that will be preprocessed.
     */
    static void preprocessForDDSG(CH_Graph& graph);

private:
    /**
     * This auxiliary function puts all generated shortcuts back into the graph at the end of the preprocessing.
     * This is necessary, because most of the shortcuts will be later removed from the graph to create additional
     * shortcuts.
     *
     * @param graph[in, out] The graph we are working with.
     */
    static void reinsertShortcuts(UpdateableGraph & graph);

    /**
     * This function is used at the beginning of the preprocessing process. It simply computes the initial
     * weight of each node and constructs a priority queue based on those weights.
     *
     * @param priorityQueue[in, out] The priority queue.
     * @param graph[in] The graph that will be used to initialize the queue.
     */
    static void initializePriorityQueue(
            CHpriorityQueue & priorityQueue,
            CH_Graph& graph
    );

    /**
     * This function actually contracts nodes. It simply contracts nodes one after each other until the priority
     * queue is empty. In each step, the node with the currently lowest weight has its weight updated, and if it
     * still has the lowest weight after the update, it is contracted and its neighbours weights are updated.
     * If it does not have the lowest weight after the update, we update the new lowest weight node and repeat this
     * process until we find a constant minimum. This variant additionally generates unpacking data.
     *
     * @param priorityQueue[in, out] The priority queue we are working with.
     * @param graph[in, out] The graph we are working with.
     */
    static void contractNodesWithUnpackingData(
        CHpriorityQueue & priorityQueue,
        UpdateableGraph & graph
    );

    /**
     * Auxiliary function that adjust neighbours degrees when a node is contracted.
     *
     * @param x[in] The node for which we want to adjust its neighbours.
     * @param graph[in, out] The graph we are working with.
     */
    static void adjustNeighboursDegrees(
            const unsigned int x,
            UpdateableGraph & graph);

    /**
     * This function gets the shortcuts lengths, it takes every pair of neighbours of a node i and computes the weight
     * of the shortcut that could be added between those two neighbours if the node i was contracted.
     *
     * @param i[in] The node we are interested in.
     * @param graph[in, out] The graph we are working with.
     */
    static void getDistancesUsingNode(
            const unsigned int i,
            CH_Graph& graph
    );

    /**
     * This function will get a longest shortcut from a specific source to one of the targets, which is then used as an
     * upper bound during the oneToManyWithBuckets() function.
     *
     * @param source[in] The node we are interested in.
     * @return The length (weight) of the longest shortcut that would get added if we contracted 'source'.
     */
    static unsigned int longestPossibleShortcut(
            const unsigned int source);

    /**
     * This function is called after a node is contracted to recalculate the weights of its neighbours. Those weights
     * might have changed, because those neighbours might now have a different amount of edges than before.
     *
     * @param x[in] The node we are interested in.
     * @param graph[in, out] The graph we are working with.
     * @param priorityQueue[in, out] The priority queue we are working with.
     */
    static void updateNeighboursPriorities(
            const unsigned int x,
            UpdateableGraph & graph,
            CHpriorityQueue & priorityQueue);

    /**
     * This function computes the lengths of shortest paths between all source - target pairs that don not contain the
     * contracted node. The search space is limited, so we don not actually always find all shortest path, this is not
     * a problem if we make sure to add a shortcut if we did not find a shortest path that was shorter or equal length
     * as the proposed shortcut.
     *
     * @param graph[in, out] The graph we are working with.
     * @param deep[in] A flag determining whether we want to perform a deep search or only a shallow one. A deep search
     * explores a larger part of the graph meaning it will find more shortest paths, but it takes longer to perform.
     */
    static void manyToManyWithBuckets(
            UpdateableGraph & graph,
            bool deep);

    /**
     * We use a simple extension of the oneToMany algorithm, we do a one edge backwards search from each of the targets
     * and save the information into buckets. When expanding nodes during oneToManyWithBuckets(), we can validate if
     * there exists a bucket entry for a node we are expanding, if yes, we immediately know that there exists a path
     * from that node to some target node and we also have its length.
     *
     * @param x[in] The node we want to initialize the buckets for.
     * @param graph[in, out] The graph we are working with.
     * @param lowestBucketVal[in, out] Will update the lowest value contained in one of the buckets. This value can be
     * used in the 'manyToManyWithBuckets' function to compute an upper-bound for the one to many searches.
     */
    static void initBuckets(
            const unsigned int x,
            UpdateableGraph & graph,
            unsigned int & lowestBucketVal);

    /**
     * This function basically takes one source and tries to find shortest paths to all targets.
     * The algorithm is stopped if one of those things happen:
     * 1) All targets are found
     * 2) The current node weight is higher than the upperBound
     * 3) We have reached the space search limit - that means we have either expanded to much nodes or all our paths
     * already have more hops (edges) than the limit.
     *
     * @param source[in] The source node for our search.
     * @param upperBound[in] The upper-bound that can be used to stop the search early.
     * @param graph[in, out] The graph we are working with.
     * @param hoplimit[in] The limit of the hops (edges) each path can consist of during the search.
     * @param maxexpanded[in] The limit of the maximum number of nodes expanded during the search.
     */
    static void oneToManyWithBuckets(
            const unsigned int source,
            const unsigned int upperBound,
            UpdateableGraph & graph,
            unsigned int hoplimit = 5,
            unsigned int maxexpanded = 1000);

    /**
     * This function will get all possible shortcuts than can be added after the contraction of a certain node i.
     *
     * @param i[in] The node we are interested in.
     * @param graph[in, out] The graph we are working with.
     * @param deep[in] A flag determining whether we want to perform a deep search or only a shallow one. A deep search
     * explores a larger part of the graph meaning it could eliminate more shortcuts, but it takes longer to perform.
     */
    static void getPossibleShortcuts(
            const unsigned int i,
            CH_Graph& graph,
            bool deep
    );

    /**
     * This function calculates how many of the shortcuts found by the getPossibleShortcuts() function are actually
     * needed. This means validating if there exists a path which does not contain the contracted node which is shorter
     * or equal length as the possible shortcut. This function returns the number of shortcuts that will have to be
     * added. Take into consideration that for the Contraction Hierarchies to work correctly, we are pessimistic
     * in a sense that we always add shortcuts if we did not find a path without the contracted node that is
     * shorter or equal length as the shortcut. But the path could exist, only we were not able to find it,
     * as we have a certain hop limit during our many to many search.
     *
     * @return Returns how many shortcuts are actually needed based on the data computed before the call of this
     * function.
     */
    static unsigned int calculateShortcutsAmount();

    /**
     * This function actually adds shortcuts to the graph - this is used when we are actually contracting the node
     * and not only calculating its weight. The shortcuts are only added if they are necessary, that means only if
     * we did not find a shorter or equal length path without the contracted node. This variant additionally saves
     * the unpacking data, that means the source and target of the shortcut and the node omitted by the shortcut.
     *
     * @param graph[in, out] The graph we are working with.
     * @param x[in] The node we are currently contracting.
     */
    static void actuallyAddShortcutsWithUnpackingData(
            UpdateableGraph & graph,
            unsigned int x);

    /**
     * Auxiliary function that removes all the edges containing the contracted node so they don not slow the following
     * runs of the many to many shortest paths calls.
     *
     * @param graph[in, out] The graph we are working with.
     * @param x[in] The node we are currently contracting.
     */
    static void removeContractedNodeEdges(
            UpdateableGraph & graph,
            unsigned int x);

    /**
     * Auxiliary function that clears all the structures between contracting nodes or computing node weights.
     */
    static void clearStructures();

    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1,T2> &p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);

            return h1 ^ h2;
        }
    };

    static std::vector<bool> contracted;
    static std::vector<unsigned int> preprocessingDegrees;
    static std::vector<unsigned int> dijkstraDistance;
    static std::vector<ShortcutEdge> allShortcuts;
    static std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int, pair_hash> distances;
    static std::unordered_map<std::pair<unsigned int, unsigned int>, unsigned int, pair_hash> distancesWithoutX;
    static std::vector<unsigned int> sources;
    static std::vector<unsigned int> targets;
    static std::unordered_set<unsigned int> targetsSet;
    static std::unordered_map<unsigned int, std::vector<std::pair<unsigned int, unsigned int > > > buckets;



};


#endif //TRANSIT_NODE_ROUTING_CHPREPROCESSOR_H
