/******************************************************************************
 * File:             CsvGraphLoader.cpp
 *
 * Author:           Jan Neumann (neumaja5@fel.cvut.cz)
 * Created:          07/27/20
 *****************************************************************************/

#include "CsvGraphLoader.h"
#include "CLI/ProgressBar.hpp"
#include "GraphBuilding/Structures/Graph.h"
#include "GraphBuilding/Structures/UpdateableGraph.h"
#include <boost/numeric/conversion/cast.hpp>
#include <algorithm>
#include <cctype>
#include <climits>
#include <cmath>

#define NOMINMAX // prevents the min and max macro definitions from windows.h, which are introduced in p-ranav-csv2
#include <csv2/reader.hpp>

#include <limits>
#include <stdexcept>
#include <string>

CsvGraphLoader::CsvGraphLoader(string inputFile) : inputFile(inputFile) {}

typedef csv2::Reader<csv2::delimiter<','>, csv2::quote_character<'"'>,
                     csv2::first_row_is_header<false>,
                     csv2::trim_policy::trim_whitespace>
    DefaultCSVReader;

inline bool stricmp(const string s1, const string s2) {
  size_t size1 = s1.size();

  if (size1 != s2.size())
    return false;

  for(size_t i = 0; i < size1; ++i) {
    if (tolower(s1[i]) != tolower(s2[i]))
      return false;
  }

  return true;
}

const string NAN_STR = string("nan");

dist_t parse_distance(std::string str) {
  if (stricmp(str, NAN_STR)) {
    return std::numeric_limits<dist_t>::max();
  } else {
    return (dist_t)round(stof(str));
  }
}

std::vector<dist_t> CsvGraphLoader::loadAdjacencyMatrix() {
  DefaultCSVReader reader;

  if (reader.mmap(this->inputFile)) {
    const size_t size = reader.cols();
    if (size != reader.rows())
      throw runtime_error(this->inputFile +
                          " does not contain a square matrix. Found " +
                          to_string(reader.rows()) + " rows and " +
                          to_string(size) + " cols.\n");

    std::vector<dist_t> adj(size * size);

    ProgressBar progress(size, "Loading CSV file:");

    size_t index = 0;
    for (const auto row : reader) {
      for (const auto cell : row) {
        string val;
        cell.read_value(val);
        adj[index++] = parse_distance(val);
      }
      ++progress;
    }

    return adj;
  } else {
    throw runtime_error(string("Error reading file ") + this->inputFile +
                        " using mmap.\n");
  }
}

Graph *CsvGraphLoader::loadGraph() {
  DefaultCSVReader reader;

  if (reader.mmap(this->inputFile)) {
    const size_t size = reader.cols();
    if (size != reader.rows())
      throw runtime_error(this->inputFile +
                          " does not contain a square matrix. Found " +
                          to_string(reader.rows()) + " rows and " +
                          to_string(size) + " cols.\n");

    const dist_t max = std::numeric_limits<dist_t>::max();
    auto *const graph = new Graph(boost::numeric_cast<unsigned int>(size)); // TODO graph could possibly have more than UINT nodes ?

    ProgressBar progress(size, "Loading CSV file:");

    size_t i = 0;
    for (const auto row : reader) {
      size_t j = 0;
      for (const auto cell : row) {
        string val;
        cell.read_value(val);
        const dist_t dist = parse_distance(val);
        if (dist != max)
          graph->addEdge(boost::numeric_cast<unsigned int>(i), boost::numeric_cast<unsigned int>(j), dist);
        ++j;
      }
      ++progress;
      ++i;
    }

    return graph;
  } else {
    throw runtime_error(string("Error reading file ") + this->inputFile +
                        " using mmap.\n");
  }
}

UpdateableGraph *CsvGraphLoader::loadUpdateableGraph() {
  DefaultCSVReader reader;

  if (reader.mmap(this->inputFile)) {
    const size_t size = reader.cols();
    if (size != reader.rows())
      throw runtime_error(this->inputFile +
                          " does not contain a square matrix. Found " +
                          to_string(reader.rows()) + " rows and " +
                          to_string(size) + " cols.\n");

    const dist_t max = std::numeric_limits<dist_t>::max();
    auto *const graph = new UpdateableGraph(boost::numeric_cast<unsigned int>(size)); // TODO graph could possibly have more than UINT nodes ?

    ProgressBar progress(size, "Loading CSV file:");

    size_t i = 0;
    for (const auto row : reader) {
      size_t j = 0;
      for (const auto cell : row) {
        string val;
        cell.read_value(val);
        const dist_t dist = parse_distance(val);
        if (dist != max)
          graph->addEdge(boost::numeric_cast<unsigned int>(i), boost::numeric_cast<unsigned int>(j), dist);
        ++j;
      }
      ++progress;
      ++i;
    }

    return graph;
  } else {
    throw runtime_error(string("Error reading file ") + this->inputFile +
                        " using mmap.\n");
  }
}

void CsvGraphLoader::putAllEdgesIntoUpdateableGraph(UpdateableGraph &graph) {
  DefaultCSVReader reader;

  if (reader.mmap(this->inputFile)) {
    const size_t size = reader.cols();
    if (size != reader.rows())
      throw runtime_error(this->inputFile +
                          " does not contain a square matrix. Found " +
                          to_string(reader.rows()) + " rows and " +
                          to_string(size) + " cols.\n");

    const dist_t max = std::numeric_limits<dist_t>::max();

    ProgressBar progress(size, "Loading CSV file:");

    size_t i = 0;
    for (const auto row : reader) {
      size_t j = 0;
      for (const auto cell : row) {
        string val;
        cell.read_value(val);
        const dist_t dist = parse_distance(val);
        if (dist != max)
          graph.addEdge(boost::numeric_cast<unsigned int>(i), boost::numeric_cast<unsigned int>(j), dist);
        ++j;
      }
      ++progress;
      ++i;
    }

  } else {
    throw runtime_error(string("Error reading file ") + this->inputFile +
                        " using mmap.\n");
  }
}
