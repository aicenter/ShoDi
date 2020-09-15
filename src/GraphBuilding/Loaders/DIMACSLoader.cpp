//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include "DIMACSLoader.h"
#include "../../Error/Error.h"
#include "../../Timer/Timer.h"
#include "GraphBuilding/Structures/BaseGraph.h"
#include "GraphBuilding/Structures/SimpleGraph.h"
#include "GraphBuilding/Structures/UpdateableGraph.h"
#include "constants.h"
#include <boost/numeric/conversion/cast.hpp>
#include <climits>
#include <fstream>
#include <memory>
#include <string>
#include <type_traits>

//______________________________________________________________________________________________________________________
DIMACSLoader::DIMACSLoader(string inputFile)
    : inputFile(inputFile), amountsParsed(false) {
  input.open(inputFile);
  if (!input.is_open()) {
    throw runtime_error(string("Couldn't open file '") + this->inputFile +
                        "'!");
  }
}

void DIMACSLoader::parseAmounts() {
  if(input.eof())
    input.clear();

  input.seekg(0, std::ios::beg);

  parseGraphProblemLine(input, nodesAmount, edgesAmount);
  amountsParsed = true;
}

unsigned int DIMACSLoader::nodes() {
  if(!amountsParsed)
    parseAmounts();
  return nodesAmount;
}

size_t DIMACSLoader::edges() {
  if(!amountsParsed)
    parseAmounts();
  return edgesAmount;
}


void DIMACSLoader::loadGraph(BaseGraph &graph, unsigned int precisionLoss) {
  parseAmounts();

  if (graph.handlesDuplicateEdges()) {
    parseEdges(input, graph, edgesAmount, precisionLoss);
  } else {
    SimpleGraph sg(nodesAmount);
    parseEdges(input, graph, edgesAmount, precisionLoss);

    for (unsigned int i = 0; i < nodesAmount; i++) {
      for (auto &p : sg.edges(i)) {
        graph.addEdge(i, p.first, p.second);
      }
    }
  }
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::parseGraphProblemLine(ifstream &input, unsigned int &nodes,
                                         size_t &edges) {
  while (true) {
    string buffer;
    getline(input, buffer);
    if (buffer[0] == 'p') {
      processGraphProblemLine(buffer, nodes, edges);
      return;
    }
  }
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::processGraphProblemLine(string &buffer, unsigned int &nodes,
                                           size_t &edges) {
  size_t position = 5;
  unsigned int tmpnodes = 0;
  while (buffer[position] != ' ') {
    tmpnodes *= 10;
    tmpnodes += (buffer[position] - 48);
    position++;
  }

  position++;
  size_t tmpedges = 0;
  while (position < buffer.size()) {
    tmpedges *= 10;
    tmpedges += (buffer[position] - 48);
    position++;
  }

  nodes = tmpnodes;
  edges = tmpedges;
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::parseEdges(ifstream &input, BaseGraph &graph, size_t edges, unsigned int precisionLoss) {
  size_t loadededgescnt = 0;
  while (loadededgescnt < edges) {
    string buffer;
    getline(input, buffer);
    if (buffer[0] == 'a') {
      unsigned int from, to;
      dist_t weight;
      getEdge(buffer, from, to, weight);

      weight /= precisionLoss;

      if (from != to) {
        graph.addEdge(from, to, weight);
      }
      loadededgescnt++;
    }
  }
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::getEdge(string &buffer, unsigned int &from, unsigned int &to,
                           dist_t &weight) {
  size_t position = 2;
  unsigned int tmpfrom = 0;
  while (buffer[position] != ' ') {
    tmpfrom *= 10;
    tmpfrom += (buffer[position] - 48);
    position++;
  }

  position++;
  unsigned int tmpto = 0;
  while (buffer[position] != ' ') {
    tmpto *= 10;
    tmpto += (buffer[position] - 48);
    position++;
  }

  position++;
  dist_t tmpweight = 0;
  while (position < buffer.size()) {
    tmpweight *= 10;
    tmpweight += (buffer[position] - 48);
    position++;
  }

  from = tmpfrom - 1;
  to = tmpto - 1;
  weight = tmpweight;
}
