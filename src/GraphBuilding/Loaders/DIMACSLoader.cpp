//
// Author: Xenty (Michal Cvach)
// Created on: 25.7.18
//

#include "DIMACSLoader.h"
#include "../../Error/Error.h"
#include "../../Timer/Timer.h"
#include "../Structures/BaseGraph.h"
#include "../Structures/SimpleGraph.h"
#include "../Structures/UpdateableGraph.h"
#include "../../constants.h"
#include <boost/numeric/conversion/cast.hpp>
#include <climits>
#include <fstream>
#include <memory>
#include <string>
#include <type_traits>
#include <ranges>

//______________________________________________________________________________________________________________________
DIMACSLoader::DIMACSLoader(std::string inputFile)
    : inputFile(inputFile), amountsParsed(false) {
  input.open(inputFile);
  if (!input.is_open()) {
    throw std::runtime_error(std::string("Couldn't open file '") + this->inputFile +
                        "'!");
  }
}

void DIMACSLoader::parseAmounts() {
	if(input.eof()) {
		input.clear();
	}

	input.seekg(0, std::ios::beg);

	std::string _;
	input >> _ >> _ >> nodesAmount >> edgesAmount;

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


void DIMACSLoader::loadGraph(BaseGraph &graph, int scaling_factor) {
  parseAmounts();

  if (graph.handlesDuplicateEdges()) {
	  parseEdges(graph, scaling_factor);
  } else {
    SimpleGraph sg(nodesAmount);
	  parseEdges(graph, scaling_factor);

    for (unsigned int i = 0; i < nodesAmount; i++) {
      for (auto &p : sg.edges(i)) {
        graph.addEdge(i, p.first, p.second);
      }
    }
  }
}

//______________________________________________________________________________________________________________________
void DIMACSLoader::parseEdges(BaseGraph& graph, int scaling_factor) {
	size_t loadededgescnt = 0;

	std::string marker;
	unsigned int from, to;
	dist_t weight;

	while(loadededgescnt < edgesAmount && input >> marker >> from >> to >> weight) {
		if(marker[0] == 'a') {
			weight /= static_cast<dist_t>(scaling_factor);
			if(from != to) {
				--from;
				--to;
				graph.addEdge(from, to, weight);
			}
			loadededgescnt++;
		}
	}
}


