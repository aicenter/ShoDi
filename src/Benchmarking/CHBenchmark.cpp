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
// Created on: 23.8.18
//

#include "CHBenchmark.h"
#include "../CH/CHDistanceQueryManager.h"
#include "../Timer/Timer.h"
#include "../CH/CHDistanceQueryManagerWithMapping.h"

//______________________________________________________________________________________________________________________
double CHBenchmark::benchmark(const std::vector < std::pair< unsigned int, unsigned int> > & trips, FlagsGraph<NodeData>& graph, std::vector < unsigned int > & distances) {
    CHDistanceQueryManager queryManager(graph);

    Timer chTimer("Contraction hierarchies trips benchmark");
    chTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    chTimer.finish();
    return chTimer.getRealTimeSeconds();
}

//______________________________________________________________________________________________________________________
double CHBenchmark::benchmarkUsingMapping(const std::vector < std::pair< long long unsigned int, long long unsigned int> > & trips, FlagsGraph<NodeData>& graph, std::vector < unsigned int > & distances, std::string mappingFilePath) {
    CHDistanceQueryManagerWithMapping queryManager(graph, mappingFilePath);

    Timer chTimer("Contraction hierarchies trips benchmark with mapping");
    chTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    chTimer.finish();
    return chTimer.getRealTimeSeconds();
}
