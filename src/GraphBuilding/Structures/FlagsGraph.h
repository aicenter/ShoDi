//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#ifndef TRANSIT_NODE_ROUTING_FLAGSGRAPH_H
#define TRANSIT_NODE_ROUTING_FLAGSGRAPH_H

#include <utility>
#include <vector>
#include "QueryEdge.h"
#include "UpdateableGraph.h"
#include "../../CH/Structures/NodeData.h"


/**
 * Class representing a graph for specifically for the CH query algorithm.
 * We use the representation suggested in the "Contraction Hierarchies: Faster and Simpler Hierarchical
 * Routing in Road Networks" article by Robert Geisberger, Peter Sanders, Dominik Schultes, and Daniel Delling.
 * Basically, for example Dijkstra requires undirected edges to be saved at both nodes. This representation
 * only saves every edge at the incident node with the lower rank. This makes sense, because we only expand edges
 * going from lower ranked nodes to higher ranked nodes in the query algorithm anyway. We store additional flags
 * "forward" and "backward" with each edge to determine in which direction the edge should be expanded.
 */
class FlagsGraph {
protected:
    std::vector<std::vector<QueryEdge> > neighbours;
    std::vector<NodeData> nodesData;

    /**
     * Auxiliary function used to load original edges into the FlagsGraph.
     *
     * @param edges[in] A std::vector of the original edges (edges that are not shortcuts).
     */
    void processOriginalEdges(std::vector<OutputEdge> &edges);

    /**
     * Auxiliary function used to load shortcut edges into the FlagsGraph.
     *
     * @param shortcuts[in] A std::vector of the shortcut edges (must have middle node set).
     */
    void processShortcuts(std::vector<OutputShortcutEdge> &shortcuts);

public:
    /**
     * A simple constructor.
     *
     * @param n[in] The number of nodes in the graph.
     */
    explicit FlagsGraph(unsigned int n);

    /**
     * A constructor that build a FlagsGraph based on an instance of UpdateableGraph. This is intented to be used with
     * an UpdateableGraph that was already processed using the CHPreprocessor. This initializes a FlagsGraph which can
     * be used for further CH queries without the need to flush the CH into a file during the process.
     *
     * @param g[in] An instance of the UpdateableGraph that will be used to initialize an instance of the Flags Graph.
     */
    explicit FlagsGraph(UpdateableGraph &g);

    /**
     * An explicit virtual destructor to avoid undefined behavior.
     */
    virtual ~FlagsGraph();

    /**
     * Puts all edges currently present in the graph into one std::vector.
     *
     * @param allEdges[out] A std::vector that will be filled with edges from the graph.
     */
    void getEdgesForFlushing(std::vector<std::pair<unsigned int, QueryEdge> > &allEdges);

    /**
     * Adds one edge into the graph. Does not do any checks. An edge should always be added to the incident node with
     * the lower rank. This is not enforced here. When loading Contraction Hierarchies, this is handled already in the
     * loading functions.
     *
     * @param from[in] The source node of the edge.
     * @param to[in] The target node of the edge.
     * @param weight[in] The weight of the edge.
     * @param fw[in] The flag that denotes whether this edge can be used in the forward direction.
     * @param bw[in] The flag that denotes whether this edge can be used in the backward direction.
     */
    void addEdge(unsigned int from, unsigned int to, unsigned int weight, bool fw, bool bw);

    /**
     * Returns the number of nodes in the graph.
     *
     * @return The number of nodes in the graph.
     */
    unsigned int nodes() const;

    /**
     * Returns all neighbours of a given node 'x'. Keep in mind that this returns neighbour both in the forward and
     * backward direction, so we always have to check if the neighbour is in the correct direction when
     * expanding a node.
     *
     * @param x[in] The ID of the node we are interested in.
     * @return Vector of edges that are incident with 'x' (They can have 'x' both as source and as target).
     */
    const std::vector<QueryEdge> &nextNodes(unsigned int x) const;

    /**
     * Returns the data for a certain node.
     *
     * @param node[in] The node we are interested in.
     * @return Reference to the data about the node.
     */
    virtual NodeData &data(unsigned int node);

    /**
     * Auxiliary function used to reset some data that could be changed during queries to their initial state so that
     * following queries still return correct values.
     *
     * @param node[in] The node we want to reset.
     */
    virtual void resetForwardInfo(const unsigned int node);

    /**
     * Auxiliary function used to reset some data that could be changed during queries to their initial state so that
     * following queries still return correct values.
     *
     * @param node[in] The node we want to reset.
     */
    virtual void resetBackwardInfo(const unsigned int node);

    /**
     *
     * @param node
     */
    virtual void resetForwardStall(const unsigned int node);

    /**
     *
     * @param node
     */
    virtual void resetBackwardStall(const unsigned int node);
};


#endif //TRANSIT_NODE_ROUTING_FLAGSGRAPH_H
