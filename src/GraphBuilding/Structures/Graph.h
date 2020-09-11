//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#ifndef TRANSIT_NODE_ROUTING_GRAPH_H
#define TRANSIT_NODE_ROUTING_GRAPH_H

#include <utility>
#include <vector>
#include "BaseGraph.h"
#include "SimpleGraph.h"
#include "constants.h"

using namespace std;

/**
 * Class representing a graph.
 * This simple graph representation is used for the Dijkstra Algorithm. For the Contraction Hierarchies,
 * other representation seem to work better. This representation is also used at some points during the preprocessing
 * process for some methods using some modes.
 */
class Graph : public BaseGraph {
private:
    vector< vector < pair< unsigned int, dist_t > > > followingNodes;
    vector< vector < pair< unsigned int, dist_t > > > previousNodes;
public:
    /**
     * A simple constructor.
     *
     * @param n[in] The number of nodes in the graph.
     */
    Graph(unsigned int n);

    /**
     * A constructor that will initialize a Graph instance based on an instance of the SimpleGraph class.
     * SimpleGraph can be used to ensure we will not end up with a multi graph, but for Dijkstra, Graph is needed.
     *
     * @param x[in] An instance of the SimpleGraph class that will be used to initialize an instance of Graph.
     */
    Graph(SimpleGraph & x);

    /**
     * Adds an edge into the graph. This function does not validate that there is not another edge between the two
     * nodes already. In theory, the Graph class can also represent multi graphs. Since we do not want parallel edges
     * most of the time, it might make sense to start with an instance of SimpleGraph first and then transform it to
     * Graph later.
     *
     * @param from[in] Source node of the edge.
     * @param to[in] Target node of the edge.
     * @param weight[in] The weight of the edge.
     */
    bool addEdge(unsigned int from, unsigned int to, dist_t weight) override;

    /**
     * Returns the number of nodes in the graph.
     *
     * @return The number of nodes in the graph.
     */
    unsigned int nodes() const override;

    /**
     * Returns all the edges with the node 'x' as their target node.
     *
     * @param x[in] The target node we are interested in.
     * @return All the edges in the graph that have 'x' as their target node.
     */
    const vector<pair<unsigned int, dist_t>> & incomingEdges(const unsigned int x)const;

    /**
     * Returns all the edges with the node 'x' as their source node.
     *
     * @param x[in] The source node we are interested in.
     * @return All the edges in the graph that have 'x' as their source node.
     */
    const vector<pair<unsigned int, dist_t>> & outgoingEdges(const unsigned int x)const;

    ~Graph() = default;
};

#endif //TRANSIT_NODE_ROUTING_GRAPH_H
