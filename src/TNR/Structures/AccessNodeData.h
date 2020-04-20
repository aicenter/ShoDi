//
// Author: Xenty (Michal Cvach)
// Created on: 05.08.19
//

#ifndef CONTRACTION_HIERARCHIES_ACCESSNODEDATA_H
#define CONTRACTION_HIERARCHIES_ACCESSNODEDATA_H

/**
 * Auxiliary class containing data about one access node. This is basically a named pair containing the ID of the
// access node and the distance to it.
 */
class AccessNodeData {
public:
    /**
     * A simple constructor to initialize an access node.
     *
     * @param a[in] The new access node ID.
     * @param b[in] The distance to the new access node.
     */
    AccessNodeData(
            unsigned int a,
            unsigned int b);

    unsigned int accessNodeID;
    unsigned int distanceToNode;
};


#endif //CONTRACTION_HIERARCHIES_ACCESSNODEDATA_H
