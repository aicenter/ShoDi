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
     */
    virtual void loadGraph(BaseGraph &graph) = 0;

    /**
     * Retrieve amount of nodes in graph
     */
    virtual unsigned int nodes() = 0;

    virtual ~GraphLoader() = default;
};


#endif //SHORTEST_PATHS_GRAPHLOADER_H
