//
// Author: Xenty (Michal Cvach)
// Created on: 28.10.19
//

#ifndef CONTRACTION_HIERARCHIES_SIMPLEEDGE_H
#define CONTRACTION_HIERARCHIES_SIMPLEEDGE_H

/**
 * A simple representation of one edge. This class is used during the queries where we are interested in the actual
 * shortest path and not just the shortest distance. In those cases, a vector of instances of this class is returned
 * each instance representing one edge of the path.
 */
class SimpleEdge {
public:
    /**
     * A simple constructor.
     *
     * @param f[in] Source node for the edge.
     * @param t[in] Target node for the edge.
     */
    SimpleEdge(unsigned int f, unsigned int t);

    /**
     * A simple constructor used to basically copy an edge.
     *
     * @param se[in] Another instance of the class that we want to create a copy of.
     */
    SimpleEdge(const SimpleEdge & se);

    unsigned int from;
    unsigned int to;
};


#endif //CONTRACTION_HIERARCHIES_SIMPLEEDGE_H
