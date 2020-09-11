//
// Created by Jan Neumann (neumaja5@fel.cvut.cz) on 27.07.20.
//

#ifndef SHORTEST_PATHS_GRAPHLOADER_H
#define SHORTEST_PATHS_GRAPHLOADER_H


#include "constants.h"
#include "../Structures/BaseGraph.h"

#include <vector>

using namespace std;

class GraphLoader {
public:

    /**
     * Function used to load a graph into a BaseGraph instance.
     *
     * @param[in] graph The graph to load the edges into. Amount of nodes must match.
     * @param[in] precisionLoss This parameter allows us to lose some precision
     * of the weight values. Each loaded weight will be divided by this value before rounding.
     */
    virtual void loadGraph(BaseGraph &graph, unsigned int precisionLoss) = 0;

    /**
     * Retrieve amount of nodes in graph
     */
    virtual unsigned int nodes() = 0;

    virtual ~GraphLoader() = default;
};


#endif //SHORTEST_PATHS_GRAPHLOADER_H
