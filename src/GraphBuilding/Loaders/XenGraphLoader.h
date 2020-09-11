//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#ifndef CONTRACTION_HIERARCHIES_XENGRAPHLOADER_H
#define CONTRACTION_HIERARCHIES_XENGRAPHLOADER_H

#include <string>
#include <map>
#include <fstream>
#include "../Structures/Graph.h"
#include "../Structures/SimpleGraph.h"
#include "../Structures/UpdateableGraph.h"
#include "GraphBuilding/Structures/BaseGraph.h"
#include "GraphLoader.h"

/**
 * Loader used for loading integer graphs in the XenGraph format. The format is a really simple text format and is
 * described in the README.md of this project. This loader allows the loading into an Graph, which can be used
 * for the Dijkstra implementation, UpdateableGraph, which is used to create a Contraction Hierarchy,
 * and additionally also can read the XenGraph indices mapping files, which must be used when the user wants to query
 * using the original indices from the .geojson files.
 */
class XenGraphLoader : public GraphLoader {
private:
    string inputFile;
    ifstream input;
    bool amountsParsed;
    unsigned int nodesAmount;
    size_t edgesAmount;

    /**
     * Auxiliary function used to parse the edges when loading the graph.
     *
     * @param input[in] The input stream corresponding to the input file.
     * @param graph[in, out] The graph instance that the edges will be inserted into.
     * @param edges[in] The number of edges that need to be loaded.
     * @param[in] precisionLoss This parameter allows us to lose some precision
     * of the weight values. Each loaded weight will be divided by this value before rounding.
     */
    void parseEdges(BaseGraph & graph, unsigned int precisionLoss);

    void parseAmounts();

public:
    /**
     * A simple constructor.
     *
     * @param inputFile[in] A path towards a file that should be loaded using this loader.
     */
    explicit XenGraphLoader(string inputFile);

    /**
     * This function can be used to load a mapping from the original IDs to IDs used by the query algorithms.
     * This is needed when for example using the query algorithms from other programming languages where some other
     * node IDs are used for some reason.
     *
     * @param mapping[out] An unordered map that will be filled with the mapping data.
     */
    void loadNodesMapping(unordered_map <long long unsigned int, unsigned int> & mapping);

    void loadGraph(BaseGraph &graph, unsigned int precisionLoss) override;

    unsigned int nodes() override;

    size_t edges();

    ~XenGraphLoader() override = default;
};


#endif //CONTRACTION_HIERARCHIES_XENGRAPHLOADER_H
