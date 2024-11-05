//
// Author: Xenty (Michal Cvach)
// Created on: 06.08.19
//

#ifndef CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGGRAPH_H
#define CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGGRAPH_H


#include "FlagsGraph.h"
#include "../../TNR/Structures/AccessNodeData.h"
#include "../../DistanceMatrix/Distance_matrix_travel_time_provider.h"

/**
 * A Transit Node Routing data-structure. This basically contains all data required for the TNR queries. For non-local
 * queries, this structure contains the distance table for transit nodes, access nodes for all nodes, and also the
 * locality filter data. For local queries, Contraction Hierarchies are used, so this structure is built on top of
 * FlagsGraph which alone can be used for the Contraction Hierarchies query algorithm.
 */
template <class T = NodeData>
class TransitNodeRoutingGraph: public FlagsGraph<T> {
public:
    /**
     * A simple constructor.
     *
     * @param nodes[in] The number of nodes in the graph.
     * @param transitNodesAmount[in] The size of the transit node set for the data structure.
     */
    TransitNodeRoutingGraph(
            unsigned int nodes,
            unsigned int transitNodesAmount);

    /**
     * Explicit destructor to avoid undefined behavior.
     */
    ~TransitNodeRoutingGraph();


    const std::vector<std::vector<AccessNodeData>> &getForwardAccessNodes() const;

    const std::vector<std::vector<AccessNodeData>> &getBackwardAccessNodes() const;

    const std::vector<std::vector<unsigned int>> &getForwardSearchSpaces() const;

    const std::vector<std::vector<unsigned int>> &getBackwardSearchSpaces() const;

    const std::vector<std::vector<unsigned int>> &getTransitNodesDistanceTable() const;

    const std::unordered_map<unsigned int, unsigned int> &getTransitNodeMapping() const;

    /**
     * Determines whether the query is local or global. Global queries can be answered using the transit node set,
     * local queries must be answered using some fallback algorithm (Contraction Hierarchies are used here).
     * In our case, a query is considered local if the intersection of the forward search space of start and
     * the backward search space of goal is unempty (this function then returns true).
     * If it is empty, the query is global (this function returns false).
     *
     * @param start[in] The start node of the query.
     * @param goal[in] The goal node of the query.
     * @return Returns true if the query is a local query, false if it is a global query (TNR can be utilized).
     */
    bool isLocalQuery(
            unsigned int start,
            unsigned int goal);

    /**
     * Finds the distance between two nodes based on the TNR data-structure. This is used for the non-local queries.
     * In that case, all pairs of access nodes of start and goal are checked and the shortest distance from those
     * pairs is returned.
     *
     * @param start[in] The start node of the query.
     * @param goal[in] The target node of the query.
     * @return Returns the shortest distance from start to goal or 'UINT_MAX' if they are not connected.
     */
    unsigned int findTNRDistance(
            unsigned int start,
            unsigned int goal);

    /**
     * Used to establish a mapping from IDs in the graph to IDs in the transit node set.
     *
     * @param realID[in] An ID of some node that is also a transit node.
     * @param transitNodesID[in] The ID of the transit node in the transit node set.
     */
    void addMappingPair(
            unsigned int realID,
            unsigned int transitNodesID);

    /**
     * Auxiliary function used to fill the transit node distance table. This is used to establish the full distance
     * table containing distances between all pairs of transit nodes.
     *
     * @param i[in] Row in the transit node distance matrix .
     * @param j[in] Column in the transit node distance matrix.
     * @param value[in] Value for the field in the 'i'-th row and the 'j'-th column of the transit node distance matrix.
     */
    void setDistanceTableValue(
            unsigned int i,
            unsigned int j,
            unsigned int value);

    /**
     * Adds a forward access node for some node.
     *
     * @param node[in] The node we are adding the access node to.
     * @param accessNodeID[in] The ID of the access node.
     * @param accessNodeDistance[in] The distance from 'node' to the access node.
     */
    void addForwardAccessNode(
            unsigned int node,
            unsigned int accessNodeID,
            unsigned int accessNodeDistance);

    /**
     * Adds a backward access node for some node.
     *
     * @param node[in] The node we are adding the access node to.
     * @param accessNodeID[in] The ID of the access node.
     * @param accessNodeDistance[in] The distance from 'node' to the access node.
     */
    void addBackwardAccessNode(
            unsigned int node,
            unsigned int accessNodeID,
            unsigned int accessNodeDistance);

    /**
     * Adds forward search space information for some node. This information is used for the locality filter
     * (the 'isLocalQuery' function).
     *
     * @param sourceNode[in] The node we are adding the search space information to.
     * @param searchSpaceNode[in] A node that is in the search space of 'sourceNode'.
     */
    void addForwardSearchSpaceNode(
            unsigned int sourceNode,
            unsigned int searchSpaceNode);

    /**
     * Adds backward search space information for some node. This information is used for the locality filter
     * (the 'isLocalQuery' function).
     *
     * @param sourceNode[in] The node we are adding the search space information to.
     * @param searchSpaceNode[in] A node that is in the search space of 'sourceNode'.
     */
    void addBackwardSearchSpaceNode(
            unsigned int sourceNode,
            unsigned int searchSpaceNode);

    /**
     * This is only statistics auxiliary function. This function checks how many of the access nodes have incorrect
     * distances. Some access nodes can have incorrect distances due to the nature of Contraction Hierarchies.
     * This function only reports the portion of access nodes with incorrect distances but does not do anything
     * about them. This function will probably be removed in the final version as it only outputs some info that
     * is not really that important if everything is fully functional.
     *
     * @param dm[in] A full distance matrix containing distance for all pairs of nodes in the graph.
     */
    void accessNodesTest(Distance_matrix_travel_time_provider<dist_t>& dm);

protected:
    std::vector < std::vector < AccessNodeData > > forwardAccessNodes;
    std::vector < std::vector < AccessNodeData > > backwardAccessNodes;
    std::vector < std::vector < unsigned int > > forwardSearchSpaces;
    std::vector < std::vector < unsigned int > > backwardSearchSpaces;
    std::vector < std::vector < unsigned int > > transitNodesDistanceTable;
    std::unordered_map< unsigned int, unsigned int > transitNodeMapping;
};

#include "TransitNodeRoutingGraph.tpp"

#endif //CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGGRAPH_H
