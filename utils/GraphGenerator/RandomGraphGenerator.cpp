//
// Author: Xenty (Michal Cvach)
// Created on: 7.8.18
//

#include <vector>
#include <set>
#include <cstdlib>
#include <queue>
#include <cstdio>
#include <ctime>

#define NODES_CNT 100
#define MAX_LENGTH 4000

using namespace std;

// Auxiliary function for validating whether the graph is connected. This is managed by a simple BFS search.
// We run a BFS from one node and after it ends, we check if all nodes were visited. If they weren't, we add more random
// edges and then check again.
//______________________________________________________________________________________________________________________
bool validateConnected( vector<vector<pair<unsigned int, unsigned int> > > & graph) {
    vector<bool> visited(NODES_CNT, false);

    queue<unsigned int> q;
    q.push(0);
    visited[0] = true;

    while(! q.empty()) {
        unsigned int current = q.front();
        q.pop();

        for(auto iter = graph[current].begin(); iter != graph[current].end(); ++iter) {
            if (! visited[(*iter).first]) {
                visited[(*iter).first] = true;
                q.push((*iter).first);
            }
        }
    }

    for(unsigned int i = 0; i < NODES_CNT; i++) {
        if (! visited[i]) {
            return false;
        }
    }

    return true;
}

// This program can generate a random graph with a certain amount of nodes. The graph is guaranteed to be connected.
// The generated graph is undirected (more precisely for every edge v->u an edge u->v with the same weight is also
// added). The graph is then printed out in a format which can be loaded by the Loader class. There is no guaranteed
// average degree of the generated graph and generated graphs tend to have a higher average degree than real world
// road graphs.
//______________________________________________________________________________________________________________________
int main() {
    vector<vector<pair<unsigned int, unsigned int> > > graph;

    graph.resize(NODES_CNT);
    unsigned int edgesCnt = 0;

    srand (time(NULL));

    while(! validateConnected(graph)) {
        for (unsigned int i = 0; i < NODES_CNT * 3; i++) {
            unsigned int source = rand() % static_cast<unsigned int>(NODES_CNT);
            unsigned int target = rand() % static_cast<unsigned int>(NODES_CNT);
            unsigned int length = rand() % static_cast<unsigned int>(MAX_LENGTH);
            if (source != target) {
                graph[source].push_back(make_pair(target, length));
                graph[target].push_back(make_pair(source, length));
                edgesCnt += 2;
            }
        }
    }

    printf("c This is an synthetic graph creatd by RandomGraphGenerator in the DIMACS format.\n");
    printf("p sp %u %u\n", NODES_CNT, edgesCnt);
    for(unsigned int i = 0; i < NODES_CNT; i++) {
        for(auto iter = graph[i].begin(); iter != graph[i].end(); ++iter) {
            printf("a %u %u %u\n", i + 1, (*iter).first + 1, (*iter).second);
        }
    }

    return 0;
}


