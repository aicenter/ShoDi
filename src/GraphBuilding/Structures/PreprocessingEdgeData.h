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
// Created on: 29.8.18
//

#ifndef TRANSIT_NODE_ROUTING_PREPROCESSINGEDGE_H
#define TRANSIT_NODE_ROUTING_PREPROCESSINGEDGE_H

#include "../../constants.h"

/**
 * This structure represents one edge during the preprocessing phase. We have to keep track of whether the edge
 * is a shortcut or not and if it's a shortcut we also need to know it's middle node.
 */
class PreprocessingEdgeData {
public:
    /**
     * A simple constructor.
     *
     * @param b[in] Weight of the edge the data correspond to.
     * @param c[in] The middle node for the edge the data correspond to. Not used if 'isShortcut' is set to false.
     * @param d[in] Value for the 'isShortcut' flag. Should be true if the data correspond to a shortcut edge, false
     * otherwise.
     */
    PreprocessingEdgeData(const dist_t b, const unsigned int c, const bool d);

    /**
     * A constructor that will create a copy of an instance of this class.
     *
     * @param data[in] Another instance of this class that we want to create a copy of.
     */
    PreprocessingEdgeData(const PreprocessingEdgeData & data);

    dist_t weight;
    unsigned int middleNode;
    bool isShortcut;
};


#endif //TRANSIT_NODE_ROUTING_PREPROCESSINGEDGE_H
