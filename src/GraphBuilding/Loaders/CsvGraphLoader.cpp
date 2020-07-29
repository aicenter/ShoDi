//
// Created by Jan Neumann (neumaja5@fel.cvut.cz) on 27.07.20.
//

#include <climits>
#include <boost/algorithm/string.hpp>
#include "CsvGraphLoader.h"
#include "csv.hpp"

CsvGraphLoader::CsvGraphLoader(string inputFile) : inputFile(inputFile) {

}

int parse_distance(std::string str) {
    if (boost::iequals(str, "nan")) {
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
    //cout << size << " nodes" << endl;
    //cout << size * size << " values expected" << endl;
    //ProgressBar progress(size);

    int index = 0;
    for (; index < size; index++) {
        string str_value = first_row[index];
        adj[index] = parse_distance(str_value);
        //cout << index << " " << str_value << "\n";
    }
    //++progress;

    for (csv::CSVRow &row : reader) {
        for (csv::CSVField &field : row) {
            adj[index++] = parse_distance(field.get<string>());
        }
        //++progress;
    }

    //cout << index << " values found" << endl;
    return adj;
}

Graph *CsvGraphLoader::loadGraph() {
    csv::CSVReader reader(this->inputFile);

    vector<string> first_row = reader.get_col_names();
    const int size = first_row.size();
    auto *const graph = new Graph(size);

    for (int i = 0; i < size; ++i) {
        const int dist = parse_distance(first_row[i]);
        if (dist != INT_MAX)
            graph->addEdge(0, i, dist);
    }

    int i = 1;
    for (csv::CSVRow &row : reader) {
        int j = 0;
        for (csv::CSVField &field : row) {
            const int dist = parse_distance(field.get<string>());
            if (dist != INT_MAX)
                graph->addEdge(i, j, dist);
            ++j;
        }
        ++i;
    }

    return graph;
}

UpdateableGraph *CsvGraphLoader::loadUpdateableGraph() {
    csv::CSVReader reader(this->inputFile);

    vector<string> first_row = reader.get_col_names();
    const int size = first_row.size();
    auto *const graph = new UpdateableGraph(size);

    for (int i = 0; i < size; ++i) {
        const int dist = parse_distance(first_row[i]);
        if (dist != INT_MAX)
            graph->addEdge(0, i, dist);
    }

    int i = 1;
    for (csv::CSVRow &row : reader) {
        int j = 0;
        for (csv::CSVField &field : row) {
            const int dist = parse_distance(field.get<string>());
            if (dist != INT_MAX)
                graph->addEdge(i, j, dist);
            ++j;
        }
        ++i;
    }

    return graph;
}

void CsvGraphLoader::putAllEdgesIntoUpdateableGraph(UpdateableGraph &graph) {
    csv::CSVReader reader(this->inputFile);

    vector<string> first_row = reader.get_col_names();
    const int size = first_row.size();

    for (int i = 0; i < size; ++i) {
        const int dist = parse_distance(first_row[i]);
        if (dist != INT_MAX)
            graph.addEdge(0, i, dist);
    }

    int i = 1;
    for (csv::CSVRow &row : reader) {
        int j = 0;
        for (csv::CSVField &field : row) {
            const int dist = parse_distance(field.get<string>());
            if (dist != INT_MAX)
                graph.addEdge(i, j, dist);
            ++j;
        }
        ++i;
    }

}
