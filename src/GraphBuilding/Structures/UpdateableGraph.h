//
// Author: Xenty (Michal Cvach)
// Created on: 21.8.18
//

#ifndef TRANSIT_NODE_ROUTING_UPDATEABLEGRAPH_H
#define TRANSIT_NODE_ROUTING_UPDATEABLEGRAPH_H

#include <vector>
#include <unordered_map>
#include "PreprocessingEdgeData.h"
#include "OutputEdge.h"
#include "OutputShortcutEdge.h"
#include "Graph.h"

using namespace std;

/**
 * This class is used to represent the graph during the preprocessing phase. This representation supports pretty simple
 * and reasonably quick edge adding and removing. It also keeps track of which edges are shortcut edges and which
 * are original edges. The information in this structure can be directly used to generate a .ch file.
 */
class UpdateableGraph{
protected:
    /**
     * Auxiliary function for the output process.
     *
     * @param output[in] The stream into which the data should be output.
     */
    void flushHeader(
            ostream & output);

    /**
     * Auxiliary function for the output process.
     *
     * @param output[in] The stream into which the data should be output.
     * @param nodes[in] The number of nodes in the graph.
     * @param edges[in] The number of edges in the graph (only original edges are considered here, not shortcuts).
     * @param shortcuts[in] The number of shortcut edges in the graph.
     */
    void flushCnts(
            ostream & output,
            const unsigned int nodes,
            const unsigned int edges,
            const unsigned int shortcuts);

    /**
     * Auxiliary function for the output process.
     *
     * @param output[in] The stream into which the data should be output.
     */
    void flushRanks(
            ostream & output);

    /**
     * Auxiliary function for the output process.
     *
     * @param output[in] The stream into which the data should be output.
     * @param edges[in] A vector containing all the original edges (not shortcuts) in the graph.
     */
    void flushOriginalEdges(
            ostream & output,
            vector < OutputEdge > & edges);

    /**
     * Auxiliary function for the output process.
     *
     * @param output[in] The stream into which the data should be output.
     * @param edges[in] A vector containing all the shortcut edges in the graph.
     */
    void flushShortcutEdges(
            ostream & output,
            vector < OutputShortcutEdge > & edges);

    /**
     * Auxiliary function for the output process.
     *
     * @param output[in] The stream into which the data should be output.
     */
    void flushTerminator(
            ostream & output);

    vector< unordered_map < unsigned int, PreprocessingEdgeData > > followingNodes;
    vector< unordered_map < unsigned int, unsigned int > > previousNodes;
    vector< unsigned int > ranks;

public:
    /**
     * Auxiliary function that creates an exact copy of the graph represented as an instance of the Graph class.
     *
     * @return The new copy of the graph as an instance of the Graph class.
     */
    Graph * createCopy();

    /**
     * Auxiliary function that fills the vectors with edges and shortcut edges that have to be output. This processes
     * the edges so that they fit the Contraction Hierarchies concept of only keeping each edge at the node with the
     * lower rank, so here bidirectional edges are only put in the corresponding vector once with the node with the
     * lower rank as the source node. After this function finishes, the graph is no longer usable, because edges
     * are removed from the graph during the process.
     *
     * @param edges[out] A vector that will contain all the original edges that should remain in the output.
     * @param shortcuts[out] A vector that will contain all the shortcut edges for the output.
     */
    void prepareEdgesForFlushing(
            vector < OutputEdge > & edges,
            vector < OutputShortcutEdge > & shortcuts);

    /**
     * Auxiliary function that fills the vectors with edges and shortcut edges that have to be output. This processes
     * the edges so that they fit the Contraction Hierarchies concept of only keeping each edge at the node with the
     * lower rank, so here bidirectional edges are only put in the corresponding vector once with the node with the
     * lower rank as the source node. After this function, the graph can be used further, because this function keeps
     * track of the removed edges and reinserts them at the end.
     *
     * @param edges[out] A vector that will contain all the original edges that should remain in the output.
     * @param shortcuts[out] A vector that will contain all the shortcut edges for the output.
     */
    void prepareEdgesForFlushingWithReinsert(
            vector < OutputEdge > & edges,
            vector < OutputShortcutEdge > & shortcuts);

    /**
     * A simple constructor.
     *
     * @param n[in] The amount of nodes in the graph.
     */
    explicit UpdateableGraph(
            unsigned int n);

    /**
     * Outputs the graph in the format used for the Contraction Hierarchies. This format is briefly described in the
     * 'DDSGLoader.h' file.
     *
     * @param filePath[in] The desired path where the Contraction Hierarchy should be output.
     */
    void flushInDdsgFormat(
            string filePath);

    /**
     * Tries to insert an edge from one node to another with the given weight into the graph. The edge is not inserted
     * if there already exists an edge connecting the two nodes with a lower weight.
     *
     * @param from[in] The source node for the edge.
     * @param to[in] The target node for the edge.
     * @param weight[in] The weight of the edge (can be travel time for example).
     * @return Returns true if the edge was actually inserted into the graph. Returns false if there was already an edge
     * in the graph connecting 'from' and 'to' with better weight. This ensures we will not end up with a multigraph,
     * as we always only keep one edge at most for each pair of nodes - the one with the lowest weight.
     */
    bool addEdge(
            unsigned int from,
            unsigned int to,
            unsigned int weight);

    /**
     * Tries to insert an shortcut edge from one node to another with the given weight into the graph. The edge is not
     * inserted if there already exists an edge connecting the two nodes with a lower weight. With shortcut edges,
     * we additionally need to keep track of the middle node for the shortcut, in order to be able to later unpack the
     * shortcut if we want to obtain actual path in the original graph.
     *
     * @param from[in] The source node for the shortcut edge.
     * @param to[in] The target node for the shortcut edge.
     * @param weight[in] The weight of the shortcut edge (can be travel time for example).
     * @param middlenode[in] The middle node for the shortcut edge. Used to reconstruct paths in the original graph.
     * @return Returns true if the edge was actually inserted into the graph. Returns false if there was already an edge
     * in the graph connecting 'from' and 'to' with better weight. This ensures we will not end up with a multigraph,
     * as we always only keep one edge at most for each pair of nodes - the one with the lowest weight.
     */
    bool addShortcutEdge(
            unsigned int from,
            unsigned int to,
            unsigned int weight,
            unsigned int middlenode);

    /**
     * Removes an edge from one node to another if there exists one in the graph. (Our graph is never a multigraph,
     * so there is always at most one edge between two nodes).
     *
     * @param from[in] The source node for the edge to be removed.
     * @param to[in] The target node for the edge to be removed.
     */
    void removeEdge(
            unsigned int from,
            unsigned int to);

    /**
     * Assings a rank (used in the Contraction Hierarchies query algorithm to speed up queries) to a node.
     *
     * @param node[in] The node we are assigning the rank to.
     * @param rank[in] The rank for the node.
     */
    void setRank(
            unsigned int node,
            unsigned int rank);

    /**
     * Validates if the edge from one node to another node is an shortcut edge or not.
     *
     * @param from[in] Source node for the edge we are interested in.
     * @param to[in] Target node for the edge we are interested in.
     * @return Returns true if the edge between the two nodes is an shortcut edge, false otherwise. The edge must
     * exist in the graph.
     */
    bool isShortcut(
            const unsigned int from,
            const unsigned int to);

    /**
     * Gets requested amount of highest rank nodes from the Contraction Hierarchy.
     * Those are used in Transit Node Routing as Transit Nodes.
     *
     * @param highestNodes[out] A vector that will be filled with the desired amount of nodes with the highest ranks.
     * The vector must be resized in order to be able to contain 'requestedAmount' beforehand.
     * @param requestedAmount[in] A desired amount of transit nodes.
     */
    void getNodesWithHighestRank(
            vector< unsigned int > & highestNodes,
            unsigned int requestedAmount);

    /**
     * Returns the rank of a node.
     *
     * @param nodeID[in] The node we are interested in.
     * @return The rank of the node we are interested int.
     */
    unsigned int getRank(
            unsigned int nodeID) const;

    /**
     * Auxiliary function that returns the number of nodes in the graph.
     *
     * @return The number of nodes in the graph.
     */
    unsigned int nodes() const;

    /**
     * Returns all the edges with the node 'x' as their target node.
     *
     * @param x[in] The target node we are interested in.
     * @return All the edges in the graph that have 'x' as their target node.
     */
    const unordered_map<unsigned int, unsigned int> & incomingEdges(
            const unsigned int x)const;

    /**
     * Returns all the edges with the node 'x' as their source node.
     *
     * @param x[in] The source node we are interested in.
     * @return All the edges in the graph that have 'x' as their source node.
     */
    const unordered_map<unsigned int, PreprocessingEdgeData> & outgoingEdges(
            const unsigned int x)const;

    /**
     * Returns the degree of a node in the graph.
     *
     * @param node[in] The node we are interested in.
     * @return The degree of the node.
     */
     unsigned int degree(
            unsigned int node)const;
};




#endif //TRANSIT_NODE_ROUTING_UPDATEABLEGRAPH_H
