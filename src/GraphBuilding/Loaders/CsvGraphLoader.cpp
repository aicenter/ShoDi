//
// Created by Jan Neumann (neumaja5@fel.cvut.cz) on 27.07.20.
//

#include <climits>
#include "CsvGraphLoader.h"
#include "csv.hpp"

CsvGraphLoader::CsvGraphLoader(string inputFile) : inputFile(inputFile) {

}

int parse_distance(std::string str) {
    if (str == "nan") {
        return INT_MAX;
    } else {
        return (int) round(stof(str));
    }
}

std::vector<int> CsvGraphLoader::loadAdjacencyMatrix() {
    csv::CSVReader reader(this->inputFile);

    //init from the first row
    vector<string> first_row = reader.get_col_names();
    const int size = first_row.size();
    std::vector<int> adj(size * size);
    //ProgressBar progress(size);

    //for (CSVField& field : first_row) {
    //	adj.push_back(parse_distance(field.get<string>()));
    //}
    int index = 0;
    for (; index < size; index++) {
        string str_value = first_row[index];
        adj[index] = parse_distance(str_value);
        //cout << index << " " << str_value << "\n";
    }
    //++progress;

    for (csv::CSVRow &row : reader) {
        for (csv::CSVField &field : row) {
            adj[index++] = (parse_distance(field.get<string>()));
        }
        //++progress;
    }
    return adj;
}

Graph *CsvGraphLoader::loadGraph() {
    csv::CSVReader reader(this->inputFile);

    vector<string> first_row = reader.get_col_names();
    const int size = first_row.size();
    auto *const graph = new Graph(size);

    for (int i = 0; i < size; ++i)
        graph->addEdge(0, i, parse_distance(first_row[i]));

    int i = 1, j = 0;
    for(auto row_it = reader.begin(); row_it != reader.end(); ++row_it, ++i)
        for(auto field_it = row_it->begin(); field_it != row_it->end(); ++field_it, ++j)
            graph->addEdge(i, j, parse_distance(field_it->get<string>()));

    return graph;
}

UpdateableGraph *CsvGraphLoader::loadUpdateableGraph() {
    csv::CSVReader reader(this->inputFile);

    vector<string> first_row = reader.get_col_names();
    const int size = first_row.size();
    auto *const graph = new UpdateableGraph(size);

    for (int i = 0; i < size; ++i)
        graph->addEdge(0, i, parse_distance(first_row[i]));

    int i = 1, j = 0;
    for(auto row_it = reader.begin(); row_it != reader.end(); ++row_it, ++i)
        for(auto field_it = row_it->begin(); field_it != row_it->end(); ++field_it, ++j)
            graph->addEdge(i, j, parse_distance(field_it->get<string>()));

    return graph;
}

void CsvGraphLoader::putAllEdgesIntoUpdateableGraph(UpdateableGraph &graph) {
    csv::CSVReader reader(this->inputFile);

    vector<string> first_row = reader.get_col_names();
    const int size = first_row.size();

    for (int i = 0; i < size; ++i)
        graph.addEdge(0, i, parse_distance(first_row[i]));

    int i = 1, j = 0;
    for(auto row_it = reader.begin(); row_it != reader.end(); ++row_it, ++i)
        for(auto field_it = row_it->begin(); field_it != row_it->end(); ++field_it, ++j)
            graph.addEdge(i, j, parse_distance(field_it->get<string>()));
}
