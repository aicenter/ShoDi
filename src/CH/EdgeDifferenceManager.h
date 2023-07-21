//
// Author: Xenty (Michal Cvach)
// Created on: 1.8.18
//

#ifndef TRANSIT_NODE_ROUTING_EDGEDIFFERENCEMANAGER_H
#define TRANSIT_NODE_ROUTING_EDGEDIFFERENCEMANAGER_H

#include <vector>



/**
 * Auxiliary class than can compute the weight of a node for the contraction order. There are multiple terms that we
 * take into account when computing difference. The most important is edge difference, which means how many shortcuts
 * will be added into the graph after contracting a node minus the node's degree - we prefer nodes, where we add less
 * shortcuts than was the original degree of the node. We also take contracted neighbours into consideration, a node
 * which has less contracted neighbours is more likely to be contracted soon than a node with a lot of contracted
 * neighbours. This slightly helps with uniformity. Other terms can be added to the weight computation in order to get
 * better preprocessing times or even better query times. Things like 'Voronoi Regions', 'Cost of Contraction' or 'Cost
 * of Queries' are all suggested as possible additional terms in the original article.
 */
class EdgeDifferenceManager {
public:
    /**
     * Initializes the manager. Does resize the internal structures in order to be able to compute differences later.
     *
     * @param nodes[in] The number of nodes in the graph.
     */
    static void init(
            const unsigned int nodes);

    /**
     * Computes the edge difference for a given node.
     *
     * @param contractedNode[in] The last contracted node.
     * @param x[in] The node we want to compute the difference for.
     * @param possibleShortcuts[in] The number of shortcuts that could be inserted into the graph if we were to contract
     * 'x' now.
     * @param degree[in] The current degree of 'x'.
     * @return Returns the edge difference for the node 'x'.
     */
    static int difference(
            const unsigned int contractedNode,
            const unsigned int x,
            const unsigned int possibleShortcuts,
            const unsigned int degree);

private:
    static std::vector<unsigned int> neighboursContracted;
    static std::vector<unsigned int> previousContracted;
};


#endif //TRANSIT_NODE_ROUTING_EDGEDIFFERENCEMANAGER_H
