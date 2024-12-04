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
// Created on: 2.8.18
//

#include <climits>
#include <cstdio>
#include "CHpriorityQueue.h"
#include <boost/numeric/conversion/cast.hpp>

// All methods are pretty standard binary heap methods, so google 'binary heap' and you will probably find articles
// about how it works and why it works this way. Also mapping is added here - we store heap positions for all the
// nodes, so during change value we immediately know where to find the element we want to change.

//______________________________________________________________________________________________________________________
CHpriorityQueue::CHpriorityQueue(const unsigned int nodes) {
    content.reserve(16);
    mapping.resize(nodes, UINT_MAX);
}

//______________________________________________________________________________________________________________________
void CHpriorityQueue::insert(const unsigned int x, const int y) {
    content.push_back(CHNode(x, y));
    mapping[x] = boost::numeric_cast<unsigned int>(content.size() - 1);
    bubbleUp(boost::numeric_cast<unsigned int>(content.size() - 1));
}

//______________________________________________________________________________________________________________________
void CHpriorityQueue::pushOnly(const unsigned int x, const int y) {
    content.push_back(CHNode(x, y));
    mapping[x] = boost::numeric_cast<unsigned int>(content.size() - 1);
}

//______________________________________________________________________________________________________________________
void CHpriorityQueue::changeValue(const unsigned int x, const int y) {
    unsigned int position = mapping[x];
    if (content[position].weight == y) {
        return;
    }
    if (content[position].weight < y) { // bubbleDown
        content[position].weight = y;
        bubbleDown(position);
    } else { // bubbleUp
        content[position].weight = y;
        bubbleUp(position);
    }
}

//______________________________________________________________________________________________________________________
CHNode CHpriorityQueue::front() {
    return content[0];
}

//______________________________________________________________________________________________________________________
void CHpriorityQueue::pop() {
    swap(0, boost::numeric_cast<unsigned int>(content.size() - 1));
    content.pop_back();
    bubbleDown(0);
}

//______________________________________________________________________________________________________________________
bool CHpriorityQueue::empty() {
    return content.empty();
}

//______________________________________________________________________________________________________________________
void CHpriorityQueue::buildProperHeap() {
    for (int i = boost::numeric_cast<int>((content.size()/2) - 1); i >= 0; i--) {
        bubbleDown((unsigned int) i);
    }
}

//______________________________________________________________________________________________________________________
void CHpriorityQueue::bubbleDown(const unsigned int i) {
    unsigned int cur = i;
    while(cur * 2 + 1 < content.size()) {
        if (cur * 2 + 2 < content.size()) { // both sons
            unsigned int lowerPosition = (content[cur * 2 + 1].weight < content[cur * 2 + 2].weight) ? cur * 2 + 1 : cur * 2 + 2;
            int lowerWeight = content[lowerPosition].weight;
            if (content[cur].weight < lowerWeight) {
                return;
            }
            swap(cur, lowerPosition);
            cur = lowerPosition;
        } else { // left son only
            if (content[cur].weight < content[cur * 2 + 1].weight) {
                return;
            }
            swap(cur, cur * 2 + 1);
            cur = cur * 2 + 1;
        }
    }
}

//______________________________________________________________________________________________________________________
void CHpriorityQueue::bubbleUp(const unsigned int i) {
    unsigned int cur = i;
    while (cur != 0) {
        unsigned int father = (cur - 1) / 2;
        if (content[cur].weight >= content[father].weight) {
            return;
        }
        swap(cur, father);
        cur = father;
    }
}

//______________________________________________________________________________________________________________________
void CHpriorityQueue::swap(const unsigned int x, const unsigned int y) {
    unsigned int tmpi = content[x].id;
    int tmpw = content[x].weight;
    content[x].id = content[y].id;
    content[x].weight = content[y].weight;
    content[y].id = tmpi;
    content[y].weight = tmpw;
    mapping[content[x].id] = x;
    mapping[content[y].id] = y;
}
