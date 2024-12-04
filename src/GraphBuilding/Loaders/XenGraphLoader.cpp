/*
 * MIT License
 *
 * Copyright (c) 2024 Czech Technical University in Prague
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. */

//
// Author: Xenty (Michal Cvach)
// Created on: 31.07.19
//

#include "XenGraphLoader.h"
#include "../../Error/Error.h"
#include "../../Timer/Timer.h"
#include "../Structures/SimpleGraph.h"
#include "../Structures/UpdateableGraph.h"
#include "../../constants.h"
#include <climits>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <type_traits>

//______________________________________________________________________________________________________________________
XenGraphLoader::XenGraphLoader(std::string inputFile)
    : inputFile(inputFile), amountsParsed(false) {
  input.open(inputFile);
  if (!input.is_open()) {
    throw std::runtime_error(std::string("Couldn't open file '") + this->inputFile +
                        "'!");
  }
}

void XenGraphLoader::parseAmounts() {
  if(input.eof())
    input.clear();

  input.seekg(0, std::ios::beg);

  char c1, c2, c3;
  input >> c1 >> c2 >> c3;
  if (c1 != 'X' || c2 != 'G' || c3 != 'I') {
    std::cout
      << "The input file is missing the XenGraph header." << std::endl
      << "Are you sure the input file is in the correct format?" << std::endl
      << "The loading will proceed but the loaded graph might be corrupted."
      << std::endl;
  }

  input >> nodesAmount >> edgesAmount;
  amountsParsed = true;

}

unsigned int XenGraphLoader::nodes() {
  if(!amountsParsed)
    parseAmounts();
  return nodesAmount;
}

size_t XenGraphLoader::edges() {
  if(!amountsParsed)
    parseAmounts();
  return edgesAmount;
}

void XenGraphLoader::parseEdges(BaseGraph &graph, int scaling_factor) {
  unsigned int from, to, oneWayFlag, weight;
  for (size_t i = 0; i < edgesAmount; i++) {
    input >> from >> to >> weight >> oneWayFlag;

    weight /= scaling_factor;

    if (from != to) {
      if (oneWayFlag == 1) {
        graph.addEdge(from, to, weight);
      } else {
        graph.addEdge(from, to, weight);
        graph.addEdge(to, from, weight);
      }
    }
  }
}

void XenGraphLoader::loadGraph(BaseGraph &graph, int scaling_factor) {
  parseAmounts();

  if (graph.handlesDuplicateEdges()) {
    parseEdges(graph, scaling_factor);
  } else {
    SimpleGraph sg(nodesAmount);
    parseEdges(sg, scaling_factor);

    for (unsigned int i = 0; i < nodesAmount; i++) {
      for (auto &p : sg.edges(i)) {
        graph.addEdge(i, p.first, p.second);
      }
    }
  }
}

//______________________________________________________________________________________________________________________
void XenGraphLoader::loadNodesMapping(
    std::unordered_map<long long unsigned int, unsigned int> &mapping) {
  if(input.eof())
    input.clear();

  input.seekg(0, std::ios::beg);

  char c1, c2, c3;
  input >> c1 >> c2 >> c3;
  if (c1 != 'X' || c2 != 'I' || c3 != 'D') {
    std::cout
      << "The input file is missing the XenGraph indices file header." << std::endl
      << "Are you sure the input file is in the correct format?" << std::endl
      << "The loading will proceed but the mapping might be corrupted."
      << std::endl;
  }

  input >> nodesAmount;

  long long unsigned int cur;
  for (unsigned int i = 0; i < nodesAmount; i++) {
    input >> cur;
    mapping.insert(std::make_pair(cur, i));
  }
}
