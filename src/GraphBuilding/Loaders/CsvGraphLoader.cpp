/******************************************************************************
 * File:             CsvGraphLoader.cpp
 *
 * Author:           Jan Neumann (neumaja5@fel.cvut.cz)
 * Created:          07/27/20
 *****************************************************************************/

#include "CsvGraphLoader.h"
#include "csv.hpp"
#include <algorithm>
#include <cctype>
#include <climits>
#include <limits>

CsvGraphLoader::CsvGraphLoader(string inputFile) : inputFile(inputFile) {}

bool charicmp(const char &c1, const char &c2) {
  return std::tolower(c1) == std::tolower(c2);
}

dist_t parse_distance(std::string str) {
  if (std::equal(str.begin(), str.end(), string("nan").begin(), charicmp)) {
    return std::numeric_limits<dist_t>::max();
  } else {
    return (dist_t)round(stof(str));
  }
}

std::vector<dist_t> CsvGraphLoader::loadAdjacencyMatrix() {
  csv::CSVReader reader(this->inputFile);

  // init from the first row
  vector<string> first_row = reader.get_col_names();
  const unsigned int size = first_row.size();
  std::vector<dist_t> adj(size * size);
  cout << size << " nodes" << endl;
  cout << size * size << " values expected" << endl;
  // ProgressBar progress(size);

  size_t index = 0;
  for (; index < size; index++) {
    string str_value = first_row[index];
    adj[index] = parse_distance(str_value);
    // cout << index << " " << str_value << "\n";
  }
  //++progress;

  for (csv::CSVRow &row : reader) {
    for (csv::CSVField &field : row) {
      adj[index++] = parse_distance(field.get<string>());
    }
    //++progress;
  }

  cout << index << " values found" << endl;
  return adj;
}

Graph *CsvGraphLoader::loadGraph() {
  csv::CSVReader reader(this->inputFile);

  vector<string> first_row = reader.get_col_names();
  const unsigned int size = first_row.size();
  auto *const graph = new Graph(size);

  const dist_t max = std::numeric_limits<dist_t>::max();

  for (unsigned int i = 0; i < size; ++i) {
    const dist_t dist = parse_distance(first_row[i]);
    if (dist != max)
      graph->addEdge(0, i, dist);
  }

  unsigned int i = 1;
  for (csv::CSVRow &row : reader) {
    unsigned int j = 0;
    for (csv::CSVField &field : row) {
      const dist_t dist = parse_distance(field.get<string>());
      if (dist != max)
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
  const unsigned int size = first_row.size();
  auto *const graph = new UpdateableGraph(size);

  const dist_t max = std::numeric_limits<dist_t>::max();

  for (unsigned int i = 0; i < size; ++i) {
    const dist_t dist = parse_distance(first_row[i]);
    if (dist != max)
      graph->addEdge(0, i, dist);
  }

  unsigned int i = 1;
  for (csv::CSVRow &row : reader) {
    unsigned int j = 0;
    for (csv::CSVField &field : row) {
      const dist_t dist = parse_distance(field.get<string>());
      if (dist != max)
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
  const unsigned int size = first_row.size();

  const dist_t max = std::numeric_limits<dist_t>::max();

  for (unsigned int i = 0; i < size; ++i) {
    const dist_t dist = parse_distance(first_row[i]);
    if (dist != max)
      graph.addEdge(0, i, dist);
  }

  unsigned int i = 1;
  for (csv::CSVRow &row : reader) {
    unsigned int j = 0;
    for (csv::CSVField &field : row) {
      const dist_t dist = parse_distance(field.get<string>());
      if (dist != max)
        graph.addEdge(i, j, dist);
      ++j;
    }
    ++i;
  }
}
