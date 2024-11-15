/******************************************************************************
* File:             AdjGraphLoader.h
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/27/20 
*****************************************************************************/

#ifndef SHORTEST_PATHS_ADJGRAPHLOADER_H
#define SHORTEST_PATHS_ADJGRAPHLOADER_H


#include "GraphLoader.h"

#define NOMINMAX // prevents the min and max macro definitions from windows.h, which are introduced in p-ranav-csv2
#include <string>
#include <csv2/reader.hpp>

typedef csv2::Reader<csv2::delimiter<','>, csv2::quote_character<'"'>,
                     csv2::first_row_is_header<false>,
                     csv2::trim_policy::trim_characters<' ', '\t', '\r', '\n'>>
    DefaultADJReader;

class AdjGraphLoader : public GraphLoader {
private:
    DefaultADJReader reader;
    std::string inputFile;
public:
    explicit AdjGraphLoader(const std::string& inputFile);

    using GraphLoader::loadGraph;

    void loadGraph(BaseGraph &graph, int scaling_factor) override;

    unsigned int nodes() override;

    ~AdjGraphLoader() override = default;

};


#endif //SHORTEST_PATHS_ADJGRAPHLOADER_H
