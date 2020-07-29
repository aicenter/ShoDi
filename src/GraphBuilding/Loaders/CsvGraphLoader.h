/******************************************************************************
* File:             CsvGraphLoader.h
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/27/20 
*****************************************************************************/

#ifndef SHORTEST_PATHS_CSVGRAPHLOADER_H
#define SHORTEST_PATHS_CSVGRAPHLOADER_H


#include "GraphLoader.h"

class CsvGraphLoader : public GraphLoader {
private:
    string inputFile;
public:
    CsvGraphLoader(string inputFile);

    vector<dist_t> loadAdjacencyMatrix() override;

    Graph *loadGraph() override;

    UpdateableGraph *loadUpdateableGraph() override;

    void putAllEdgesIntoUpdateableGraph(UpdateableGraph &graph) override;

    ~CsvGraphLoader() override = default;

};


#endif //SHORTEST_PATHS_CSVGRAPHLOADER_H
