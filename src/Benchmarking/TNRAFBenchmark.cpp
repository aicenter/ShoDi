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
// Created on: 06.12.19
//

#include "TNRAFBenchmark.h"
#include "../TNRAF/TNRAFDistanceQueryManager.h"
#include "../Timer/Timer.h"
#include "../TNRAF/TNRAFDistanceQueryManagerWithMapping.h"

//______________________________________________________________________________________________________________________
double TNRAFBenchmark::benchmark(const std::vector < std::pair< unsigned int, unsigned int> > & trips, TransitNodeRoutingArcFlagsGraph& graph, std::vector < unsigned int > & distances) {
    TNRAFDistanceQueryManager queryManager(graph);

    Timer tnrTimer("Transit Node Routing with Arc Flags trips benchmark");
    tnrTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    tnrTimer.finish();
    return tnrTimer.getRealTimeSeconds();
}

//______________________________________________________________________________________________________________________
double TNRAFBenchmark::benchmarkWithMapping(const std::vector < std::pair< long long unsigned int, long long unsigned int> > & trips, TransitNodeRoutingArcFlagsGraph& graph, std::vector < unsigned int > & distances, std::string mappingFilePath) {
    TNRAFDistanceQueryManagerWithMapping queryManager(graph, mappingFilePath);

    Timer tnrTimer("Transit Node Routing with Arc Flags trips benchmark with mapping");
    tnrTimer.begin();

    for(size_t i = 0; i < trips.size(); i++) {
        distances[i] = queryManager.findDistance(trips.at(i).first, trips.at(i).second);
    }

    tnrTimer.finish();
    return tnrTimer.getRealTimeSeconds();
}
