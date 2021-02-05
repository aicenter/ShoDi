/******************************************************************************
* File:             CsvGraphLoader.h
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/27/20 
*****************************************************************************/

#ifndef SHORTEST_PATHS_CSVGRAPHLOADER_H
#define SHORTEST_PATHS_CSVGRAPHLOADER_H


#include "GraphLoader.h"

#define NOMINMAX // prevents the min and max macro definitions from windows.h, which are introduced in p-ranav-csv2
#include <string>
#include <csv2/reader.hpp>

typedef csv2::Reader<csv2::delimiter<','>, csv2::quote_character<'"'>,
                     csv2::first_row_is_header<false>,
                     csv2::trim_policy::trim_characters<' ', '\t', '\r', '\n'>>
    DefaultCSVReader;

class CsvGraphLoader : public GraphLoader {
private:
    DefaultCSVReader reader;
    string inputFile;
public:
    CsvGraphLoader(string inputFile);

    void loadGraph(BaseGraph &graph, unsigned int precisionLoss) override;

    unsigned int nodes() override;

    ~CsvGraphLoader() override = default;

};


#endif //SHORTEST_PATHS_CSVGRAPHLOADER_H
