//
// Author: Xenty (Michal Cvach)
// Created on: 01.12.19
//

#ifndef CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGARCFLAGSGRAPH_H
#define CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGARCFLAGSGRAPH_H


#include "TransitNodeRoutingGraph.h"
#include "../../TNRAF/Structures/NodeDataRegions.h"
#include "../../TNRAF/Structures/AccessNodeDataArcFlags.h"

/**
 * This class contains all the information required for the Transit Node Routing with Arc Flags query algorithm.
 * This class is an extension of the TransitNodeRoutingGraph. Additionally we have to store regions for all the nodes
 * in the graph, and for each access node we need to store its Arc Flags. The findTNRAFDistance function then utilises
 * those additional information to compute the shortest distance even quicker.
 */
class TransitNodeRoutingArcFlagsGraph : public TransitNodeRoutingGraph<NodeDataRegions> {
public:
    /**
     * A simple constructor.
     *
     * @param nodes[in] The number of nodes in the graph.
     * @param transitNodesAmount[in] The size of the transit node set for the data structure.
     */
    TransitNodeRoutingArcFlagsGraph(
            unsigned int nodes,
            unsigned int transitNodesAmount);

    /**
     * Explicit destructor to avoid undefined behavior.
     */
    ~TransitNodeRoutingArcFlagsGraph() override;

	const std::vector<std::vector<AccessNodeDataArcFlags>>& getForwardAccessNodes1() const;

	const std::vector<std::vector<AccessNodeDataArcFlags>>& getBackwardAccessNodes1() const;

    /**
     * Adds a forward access node to some node.
     *
     * @param node[in] The node we are adding the access node to.
     * @param accessNodeID[in] The ID of the access node.
     * @param accessNodeDistance[in] The distance from 'node' to the access node.
     * @param regionsCnt[in] The amount of regions we are working with (for the Arc Flags).
     * @param regionsFlags[in] Arc Flags for the node represented as an 32 bit unsigned integer.
     * @param powersOf2[in] Auxiliary std::vector containing values useful for transforming the flags from the unsigned
     * integer into a std::vector of bool values.
     */
    void addForwardAccessNode(
            unsigned int node,
            unsigned int accessNodeID,
            unsigned int accessNodeDistance,
            unsigned int regionsCnt,
            uint32_t regionsFlags,
            std::vector<unsigned int> & powersOf2);

    /**
     * Adds a backward access node to some node.
     *
     * @param node[in] The node we are adding the access node to.
     * @param accessNodeID[in] The ID of the access node.
     * @param accessNodeDistance[in] The distance from 'node' to the access node.
     * @param regionsCnt[in] The amount of regions we are working with (for the Arc Flags).
     * @param regionsFlags[in] Arc Flags for the node represented as an 32 bit unsigned integer.
     * @param powersOf2[in] Auxiliary std::vector containing values useful for transforming the flags from the unsigned
     * integer into a std::vector of bool values.
     */
    void addBackwardAccessNode(
            unsigned int node,
            unsigned int accessNodeID,
            unsigned int accessNodeDistance,
            unsigned int regionsCnt,
            uint32_t regionsFlags,
            std::vector<unsigned int> & powersOf2);

    /**
     * Returns the shortest distance from start to goal using the Transit Node Routing with Arc Flags query algorithm.
     * This should only be called when we know that the query is a global query. This function compares all
     * the meaningful combinations of access nodes and then returns the smallest of the distances. This means that
     * we consider all forward access nodes of start and all backward access nodes of goal first. We obtain the region
     * of start, let us call it reg_source, and the region of goal, reg_target. We can then discard all forward access
     * nodes of start for which the flag for reg_target is set to false, and analogically we can discard all backward
     * access nodes of goal for which the flag for reg_source is set to false. For the remaining access nodes,
     * we check all combinations of a access node of start and b access node of goal,
     * we compute sums d(start, a) + d(a, b) + d(b, goal) using the values precomputed in the structures,
     * and then return the smallest result.
     *
     * @param start[in] The start node of the query.
     * @param goal[in] The goal node of the query.
     * @return Returns the shortest distance from start to goal, or 'UINT_MAX' if goal is not reachable from start.
     */
    unsigned int findTNRAFDistance(
            unsigned int start,
            unsigned int goal);

protected:
    /**
     * Auxiliary function used to reset some data that could be changed during queries to their initial state so that
     * following queries still return correct values.
     *
     * @param node[in] The node we want to reset.
     */
    void resetForwardInfo(
            const unsigned int node) override;

    /**
     * Auxiliary function used to reset some data that could be changed during queries to their initial state so that
     * following queries still return correct values.
     *
     * @param node[in] The node we want to reset.
     */
    void resetBackwardInfo(
            const unsigned int node) override;

    std::vector < std::vector < AccessNodeDataArcFlags > > forwardAccessNodes;
    std::vector < std::vector < AccessNodeDataArcFlags > > backwardAccessNodes;
};


#endif //CONTRACTION_HIERARCHIES_TRANSITNODEROUTINGARCFLAGSGRAPH_H
