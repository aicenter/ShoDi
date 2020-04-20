//
// Author: Xenty (Michal Cvach)
// Created on: 02.12.19
//

#ifndef CONTRACTION_HIERARCHIES_REGIONSSTRUCTURE_H
#define CONTRACTION_HIERARCHIES_REGIONSSTRUCTURE_H

#include <vector>

using namespace std;

/**
 * Auxiliary structure that is used during the Arc Flags computation. This structure gathers information about the
 * partitioning of the graph into k regions.
 */
class RegionsStructure {
public:
    /**
     * Initializes the region structure.
     *
     * @param nodesCnt[in] The amount of nodes in the graph.
     * @param regionsCnt[in] The amount of regions we will be working with.
     */
    RegionsStructure(
            unsigned int nodesCnt,
            unsigned int regionsCnt);

    /**
     * Allows us to assign a certain node to a certain region.
     *
     * @param nodeId[in] The node we want to assign to some region.
     * @param regionId[in] The region we are assigning the node to.
     */
    void addNode(
            unsigned int nodeId,
            unsigned int regionId);

    /**
     * Returns a vector of nodes assigned to a certain region.
     *
     * @param regionId[in] The region we are interested in.
     * @return Reference to a vector containing all the nodes corresponding to the given region.
     */
    vector < unsigned int > & nodesInRegion(
            unsigned int regionId);

    /**
     * Returns a region for a given node.
     *
     * @param nodeId[in] The node we are interested in.
     * @return The region ID for the given node.
     */
    unsigned int getRegion(
            unsigned int nodeId);

    /**
     * Returns the amount of regions we are working with.
     *
     * @return The number of regions.
     */
    unsigned int getRegionsCnt();
private:
    vector < vector < unsigned int > > regions;
    vector < unsigned int > mapping;
    unsigned int regionsCnt;
};


#endif //CONTRACTION_HIERARCHIES_REGIONSSTRUCTURE_H
