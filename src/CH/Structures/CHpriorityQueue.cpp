//
// Author: Xenty (Michal Cvach)
// Created on: 2.8.18
//

#include <climits>
#include <cstdio>
#include "CHpriorityQueue.h"

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
    mapping[x] = content.size() - 1;
    bubbleUp(content.size() - 1);
}

//______________________________________________________________________________________________________________________
void CHpriorityQueue::pushOnly(const unsigned int x, const int y) {
    content.push_back(CHNode(x, y));
    mapping[x] = content.size() - 1;
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
    swap(0, content.size()-1);
    content.pop_back();
    bubbleDown(0);
}

//______________________________________________________________________________________________________________________
bool CHpriorityQueue::empty() {
    return content.empty();
}

//______________________________________________________________________________________________________________________
void CHpriorityQueue::buildProperHeap() {
    for (int i = (int) ((content.size()/2) - 1); i >= 0; i-- ) {
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
