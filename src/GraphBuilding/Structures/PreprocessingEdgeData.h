//
// Author: Xenty (Michal Cvach)
// Created on: 29.8.18
//

#ifndef TRANSIT_NODE_ROUTING_PREPROCESSINGEDGE_H
#define TRANSIT_NODE_ROUTING_PREPROCESSINGEDGE_H

/**
 * This structure represents one edge during the preprocessing phase. We have to keep track of whether the edge
 * is a shortcut or not and if it's a shortcut we also need to know it's middle node.
 */
class PreprocessingEdgeData {
public:
    /**
     * A simple constructor.
     *
     * @param b[in] Weight of the edge the data correspond to.
     * @param c[in] The middle node for the edge the data correspond to. Not used if 'isShortcut' is set to false.
     * @param d[in] Value for the 'isShortcut' flag. Should be true if the data correspond to a shortcut edge, false
     * otherwise.
     */
    PreprocessingEdgeData(const unsigned int b, const unsigned int c, const bool d);

    /**
     * A constructor that will create a copy of an instance of this class.
     *
     * @param data[in] Another instance of this class that we want to create a copy of.
     */
    PreprocessingEdgeData(const PreprocessingEdgeData & data);

    unsigned int weight;
    unsigned int middleNode;
    bool isShortcut;
};


#endif //TRANSIT_NODE_ROUTING_PREPROCESSINGEDGE_H
