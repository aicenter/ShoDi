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
// Created on: 30.7.18
//

#include <string>
#include "../Dijkstra/BasicDijkstra.h"
#include "../Timer/Timer.h"
#include "DijkstraBenchmark.h"
#include "../GraphBuilding/Loaders/XenGraphLoader.h"

//______________________________________________________________________________________________________________________
double DijkstraBenchmark::benchmark(const std::vector < std::pair< unsigned int, unsigned int> > & trips, const Graph & graph, std::vector < unsigned int > & distances) {
    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = BasicDijkstra::run(trips.at(i).first, trips.at(i).second, graph);
    }

    dijkstraTimer.finish();
    return dijkstraTimer.getRealTimeSeconds();
}

//______________________________________________________________________________________________________________________
double DijkstraBenchmark::benchmarkUsingMapping(const std::vector < std::pair< long long unsigned int, long long unsigned int> > & trips, const Graph & graph, std::vector < unsigned int > & distances, std::string mappingFilePath) {
    XenGraphLoader mappingLoader(mappingFilePath);
    std::unordered_map<long long unsigned int, unsigned int> mapping;
    mappingLoader.loadNodesMapping(mapping);

    Timer dijkstraTimer("Dijkstra trips benchmark");
    dijkstraTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = BasicDijkstra::run(mapping.at(trips.at(i).first), mapping.at(trips.at(i).second), graph);
    }

    dijkstraTimer.finish();
    return dijkstraTimer.getRealTimeSeconds();
}
