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
// Created on: 28.10.19
//

#ifndef CONTRACTION_HIERARCHIES_SIMPLEEDGE_H
#define CONTRACTION_HIERARCHIES_SIMPLEEDGE_H

/**
 * A simple representation of one edge. This class is used during the queries where we are interested in the actual
 * shortest path and not just the shortest distance. In those cases, a std::vector of instances of this class is returned
 * each instance representing one edge of the path.
 */
class SimpleEdge {
public:
    /**
     * A simple constructor.
     *
     * @param f[in] Source node for the edge.
     * @param t[in] Target node for the edge.
     */
    SimpleEdge(unsigned int f, unsigned int t);

    /**
     * A simple constructor used to basically copy an edge.
     *
     * @param se[in] Another instance of the class that we want to create a copy of.
     */
    SimpleEdge(const SimpleEdge & se);

    unsigned int from;
    unsigned int to;
};


#endif //CONTRACTION_HIERARCHIES_SIMPLEEDGE_H
