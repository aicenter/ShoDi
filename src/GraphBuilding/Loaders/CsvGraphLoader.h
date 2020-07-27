//
// Created by Jan Neumann (neumaja5@fel.cvut.cz) on 27.07.20.
//

#ifndef SHORTEST_PATHS_CSVGRAPHLOADER_H
#define SHORTEST_PATHS_CSVGRAPHLOADER_H


#include "GraphLoader.h"

class CsvGraphLoader : public GraphLoader {
private:
    string inputFile;
public:
    CsvGraphLoader(string inputFile);

    vector<int> loadAdjacencyMatrix() override;

    Graph *loadGraph() override;

    UpdateableGraph *loadUpdateableGraph() override;

    void putAllEdgesIntoUpdateableGraph(UpdateableGraph &graph) override;

    ~CsvGraphLoader() override = default;

};


#endif //SHORTEST_PATHS_CSVGRAPHLOADER_H
