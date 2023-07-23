//
// Author: Xenty (Michal Cvach)
// Created on: 29.8.18
//

#pragma once

/**
 * A simple structure encapsulating all the important information about a node for the Contraction Hierarchies
 * query algorithm.
 */
class NodeData {
public:
    /**
     * A simple constructor that initializes the instance with default values.
     */
    NodeData();

	bool operator==(const NodeData& rhs) const = default;

	unsigned int forwardDist;
    unsigned int backwardDist;
    unsigned int rank;
    bool forwardReached;
    bool backwardReached;
    bool forwardSettled;
    bool backwardSettled;
    bool forwardStalled;
    bool backwardStalled;
};
