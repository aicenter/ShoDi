//
// Author: Xenty (Michal Cvach)
// Created on: 27.8.18
//

#include <climits>
#include "QueryPriorityQueue.h"

//______________________________________________________________________________________________________________________
QueryPriorityQueue::QueryPriorityQueue() {
    content.reserve(16);
    //mapping.resize(n, UINT_MAX);
}

//______________________________________________________________________________________________________________________
void QueryPriorityQueue::push(const unsigned int x, const long long unsigned int y) {
    //printf("Added: %llu\n", y);
    content.push_back(DijkstraNode(x, y));
    mapping.insert(make_pair(x, content.size() - 1));
    //mapping[x] = content.size() - 1;
    bubbleUp(content.size() - 1);
}

//______________________________________________________________________________________________________________________
void QueryPriorityQueue::decreaseKey(const unsigned int x, const long long unsigned int y) {
    //printf("Decreased key: %u to %llu\n", x, y);
    unsigned int position = mapping[x];
    //printf("Started decreasing key %u at position: %u\n", x, position);
    content[position].weight = y;
    bubbleUp(position);
    //printf("Decreased (bubbled down)!\n");
}

//______________________________________________________________________________________________________________________
DijkstraNode & QueryPriorityQueue::top() {
    return content[0];
}

//______________________________________________________________________________________________________________________
void QueryPriorityQueue::pop() {
    swap(0, content.size()-1);
    content.pop_back();
    bubbleDown(0);
}

//______________________________________________________________________________________________________________________
bool QueryPriorityQueue::empty() {
    return content.empty();
}

//______________________________________________________________________________________________________________________
void QueryPriorityQueue::bubbleDown(const unsigned int i) {
    unsigned int cur = i;
    while(cur * 2 + 1 < content.size()) {
        if (cur * 2 + 2 < content.size()) { // both sons
            unsigned int lowerPosition = (content[cur * 2 + 1].weight < content[cur * 2 + 2].weight) ? cur * 2 + 1 : cur * 2 + 2;
            long long unsigned int lowerWeight = content[lowerPosition].weight;
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
void QueryPriorityQueue::bubbleUp(const unsigned int i) {
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
void QueryPriorityQueue::swap(const unsigned int x, const unsigned int y) {
    unsigned int tmpi = content[x].ID;
    long long unsigned int tmpw = content[x].weight;
    content[x].ID = content[y].ID;
    content[x].weight = content[y].weight;
    content[y].ID = tmpi;
    content[y].weight = tmpw;
    mapping[content[x].ID] = x;
    mapping[content[y].ID] = y;
}

//______________________________________________________________________________________________________________________
void QueryPriorityQueue::clean() {
    content.clear();
    mapping.clear();
}

//______________________________________________________________________________________________________________________
long unsigned int QueryPriorityQueue::size() {
    return content.size();
}

//______________________________________________________________________________________________________________________
void QueryPriorityQueue::printHeap() {
    for(unsigned int i = 0; i < content.size(); i++) {
        printf("%llu ", content[i].weight);
    }
    printf("\n");
}