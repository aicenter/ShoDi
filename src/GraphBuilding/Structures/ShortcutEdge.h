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
// Created on: 30.8.18
//

#ifndef TRANSIT_NODE_ROUTING_SHORTCUTEDGE_H
#define TRANSIT_NODE_ROUTING_SHORTCUTEDGE_H

/**
 * This structure is used during the preprocessing phase to keep track of all the created shortcuts.
 * Each instance represents one shortcut with all it's info necessary for additional preprocessing.
 */
class ShortcutEdge {
public:
    /**
     * A simple constructor.
     *
     * @param a[in] A source node for the shortcut edge.
     * @param b[in] A target node for the shortcut edge.
     * @param c[in] The weight of the shortcut edge.
     * @param d[in] The middle node for the shortcut edge.
     */
    ShortcutEdge(unsigned int a, unsigned int b, unsigned int c, unsigned int d);

    unsigned int weight;
    unsigned int sourceNode;
    unsigned int targetNode;
    unsigned int middleNode;
};


#endif //TRANSIT_NODE_ROUTING_SHORTCUTEDGE_H
