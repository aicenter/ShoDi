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

#ifndef CONTRACTION_HIERARCHIES_CHDISTANCEQUERYMANAGERNOMAPPINGAPI_H
#define CONTRACTION_HIERARCHIES_CHDISTANCEQUERYMANAGERNOMAPPINGAPI_H


#include "../CH/CHDistanceQueryManager.h"
#include <string>



/**
 * This class serves as the API which will be used to call the Contraction Hierarchies functionality from Java.
 * It should be enough to generate JNI code for this class using SWIG and then use the generated code in Java.
 * Using this API, one can load a Contraction Hierarchy (basically prepare all the structures for queries),
 * then realize the actual queries, and then clean the structures when they are no longer needed.
 * Please note, that if you do not call the 'clearStructures' function manually from Java, memory leaks will occur.
 *
 * This class is not currently used anywhere. If we wanted to be able to use this class from Java, we would need to add
 * an SWIG interface file (the '.i' suffix files in this directory) for it.
 */
class CHDistanceQueryManagerNoMappingAPI {
public:
    /**
     * Initializes the Contraction Hierarchy for queries. This means loading the graph from the given input file,
     * creating an instance of a 'FlagsGraph', and then also creating an instance of the 'CHDistanceQueryManager',
     * which can then be used to answer queries.
     *
     * @param chFile[in] The path to the Contraction Hierarchy data structure that will be used to answer queries.
     */
    void initializeCH(std::string chFile);

    /**
     * Queries after initialization are pretty straightforward, we just let the initialized QueryManager instance answer
     * the queries.
     *
     * @param start[in] The start node of the query.
     * @param goal[in] The goal node of the query.
     * @return Returns the shortest distance from 'start' to 'goal' or 'UINT_MAX' if goal is not reachable from start.
     */
    unsigned int distanceQuery(unsigned int start, unsigned int goal);

    /**
     * This method clears the structures used for the Contraction Hierarchy. From Java, the user will
     * manipulate a certain Java proxy class corresponding to this class. But as memory management in
     * Java and C++ is different (C++ has no garbage collection), it is necessary to call this function once the
     * user is done with his queries. Without calling this function, memory leaks will be possible and
     * since the structures can be pretty big, it could lead to some serious problems.
     */
    void clearStructures();

private:
    CHDistanceQueryManager<NodeData>* qm;
    FlagsGraph<NodeData>* graph;
};



#endif //CONTRACTION_HIERARCHIES_CHDISTANCEQUERYMANAGERNOMAPPINGAPI_H
