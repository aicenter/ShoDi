/******************************************************************************
* File:             CsvGraphLoader.h
*
* Author:           Jan Horak (horakj31@fel.cvut.cz)
* Created:          07/18/24
*****************************************************************************/

#ifndef SHORTEST_PATHS_CSVGRAPHLOADER_H
#define SHORTEST_PATHS_CSVGRAPHLOADER_H


#include "GraphLoader.h"

#define NOMINMAX // prevents the min and max macro definitions from windows.h, which are introduced in p-ranav-csv2
#include <string>
#include <csv2/reader.hpp>

typedef csv2::Reader<csv2::delimiter<'\t'>, csv2::quote_character<'"'>,
                     csv2::first_row_is_header<true>,
                     csv2::trim_policy::trim_characters<' ', '\t', '\r', '\n'>>
    DefaultCSVReader;

class CsvGraphLoader : public GraphLoader {
private:
    DefaultCSVReader nodeReader;
    DefaultCSVReader edgeReader;
    std::string inputPath;
public:
    explicit CsvGraphLoader(const std::string& inputPath);

    using GraphLoader::loadGraph;

    void loadGraph(BaseGraph &graph, int scaling_factor) override;

    unsigned int nodes() override;

    ~CsvGraphLoader() override = default;

    void loadLocations(std::vector<std::pair<double, double>> &locations);
};


#endif //SHORTEST_PATHS_CSVGRAPHLOADER_H
