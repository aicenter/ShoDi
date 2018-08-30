//
// Author: Xenty (Michal Cvach)
// Created on: 21.8.18
//

#include <fstream>
#include "UpdateableGraph.h"


//______________________________________________________________________________________________________________________
UpdateableGraph::UpdateableGraph(unsigned int n) {
    this->followingNodes.resize(n);
    this->previousNodes.resize(n);
    this->ranks.resize(n);
}

//______________________________________________________________________________________________________________________
bool UpdateableGraph::addEdge(unsigned int from, unsigned int to, long long unsigned int weight) {
    if (followingNodes.at(from).count(to) == 1) {
        if (followingNodes.at(from).at(to).weight > weight) {
            followingNodes.at(from).at(to).weight = weight;
            previousNodes.at(to).at(from) = weight;
            return true;
        }
        return false;
    } else {
        followingNodes.at(from).insert(make_pair(to, PreprocessingEdgeData(weight, 0, false)));
        previousNodes.at(to).insert(make_pair(from, weight));
        return true;
    }
}

//______________________________________________________________________________________________________________________
bool UpdateableGraph::addShortcutEdge(unsigned int from, unsigned int to, long long unsigned int weight, unsigned int middlenode) {
    if (followingNodes.at(from).count(to) == 1) {
        if (followingNodes.at(from).at(to).weight > weight) {
            followingNodes.at(from).at(to).weight = weight;
            followingNodes.at(from).at(to).middleNode = middlenode;
            followingNodes.at(from).at(to).isShortcut = true;
            previousNodes.at(to).at(from) = weight;
            return true;
        }
        return false;
    } else {
        followingNodes.at(from).insert(make_pair(to, PreprocessingEdgeData(weight, middlenode, true)));
        previousNodes.at(to).insert(make_pair(from, weight));
        return true;
    }
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::removeEdge(unsigned int from, unsigned int to) {
    followingNodes.at(from).erase(to);
    previousNodes.at(to).erase(from);
}

//______________________________________________________________________________________________________________________
const unsigned int UpdateableGraph::nodes() const {
    return followingNodes.size();
}

//______________________________________________________________________________________________________________________
const unordered_map<unsigned int, long long unsigned int> & UpdateableGraph::incomingEdges(const unsigned int x)const {
    return previousNodes.at(x);
}

//______________________________________________________________________________________________________________________
const unordered_map<unsigned int, PreprocessingEdgeData> & UpdateableGraph::outgoingEdges(const unsigned int x)const {
    return followingNodes.at(x);
}

//______________________________________________________________________________________________________________________
const unsigned int UpdateableGraph::degree(unsigned int node)const {
    return followingNodes.at(node).size() + previousNodes.at(node).size();
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::setRank(unsigned int node, unsigned int rank) {
    ranks[node] = rank;
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushInDdsgFormat(string filePath) {
    vector < OutputEdge > edges;
    vector < OutputShortcutEdge > shortcuts;
    printf("Reordering edges for file writing\n");
    prepareEdgesForFlushing(edges, shortcuts);

    ofstream output;
    output.open ( filePath + ".ch", ios::binary );
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (filePath + ".ch").c_str());
    }

    printf("Actually flushing the CH graph into the file\n");
    flushHeader(output);
    flushCnts(output, followingNodes.size(), edges.size(), shortcuts.size());
    flushRanks(output);
    flushOriginalEdges(output, edges);
    flushShortcutEdges(output, shortcuts);
    flushTerminator(output);

    output.close();

}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushHeader(ostream & output) {
    char a, b, c, d;
    a = 0x43;
    b = 0x48;
    c = 0x0d;
    d = 0x0a;
    unsigned int version = 1;
    output.write(&a, sizeof (a));
    output.write(&b, sizeof (b));
    output.write(&c, sizeof (c));
    output.write(&d, sizeof (d));
    output.write((char *) &version, sizeof (version));
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushCnts(ostream & output, const unsigned int nodes, const unsigned int edges, const unsigned int shortcuts) {
    output.write((char *) &nodes, sizeof (nodes));
    output.write((char *) &edges, sizeof (edges));
    output.write((char *) &shortcuts, sizeof (shortcuts));
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushRanks(ostream & output) {
    for(unsigned int i = 0; i < ranks.size(); i++) {
        output.write((char *) &ranks[i], sizeof (unsigned int));
    }
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushOriginalEdges(ostream & output, vector < OutputEdge > & edges) {
    for(unsigned int i = 0; i < edges.size(); i++) {
        output.write((char *) &edges[i].sourceNode, sizeof (unsigned int));
        output.write((char *) &edges[i].targetNode, sizeof (unsigned int));
        output.write((char *) &edges[i].weight, sizeof (unsigned int));
        output.write((char *) &edges[i].flags, sizeof (unsigned int));
    }
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushShortcutEdges(ostream & output, vector < OutputShortcutEdge > & edges) {
    for(unsigned int i = 0; i < edges.size(); i++) {
        output.write((char *) &edges[i].sourceNode, sizeof (unsigned int));
        output.write((char *) &edges[i].targetNode, sizeof (unsigned int));
        output.write((char *) &edges[i].weight, sizeof (unsigned int));
        output.write((char *) &edges[i].flags, sizeof (unsigned int));
        output.write((char *) &edges[i].middleNode, sizeof (unsigned int));
    }
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::flushTerminator(ostream & output) {
    unsigned int footerNum = 0x12345678;
    output.write((char *) &footerNum, sizeof (footerNum));
}

//______________________________________________________________________________________________________________________
void UpdateableGraph::prepareEdgesForFlushing(vector < OutputEdge > & edges, vector < OutputShortcutEdge > & shortcuts) {
    for(unsigned int i = 0; i < followingNodes.size(); i++) {
        for(auto iter = followingNodes[i].begin(); iter != followingNodes[i].end(); ++iter) {
            if (ranks[i] < ranks[(*iter).first]) {
                unsigned int flags = 1;
                if (followingNodes[(*iter).first].count(i) == 1 && followingNodes[(*iter).first].at(i).weight == (*iter).second.weight) {
                    flags += 2;
                    followingNodes[(*iter).first].erase(i);
                }
                if ((*iter).second.isShortcut) {
                    flags += 4;
                    shortcuts.push_back(OutputShortcutEdge(i, (*iter).first, (*iter).second.weight, flags, (*iter).second.middleNode));
                } else {
                    edges.push_back(OutputEdge(i, (*iter).first, (*iter).second.weight, flags));
                }
            } else {
                unsigned int flags = 2;
                if (followingNodes[(*iter).first].count(i) == 1 && followingNodes[(*iter).first].at(i).weight == (*iter).second.weight) {
                    flags += 1;
                    followingNodes[(*iter).first].erase(i);
                }
                if ((*iter).second.isShortcut) {
                    flags += 4;
                    shortcuts.push_back(OutputShortcutEdge((*iter).first, i, (*iter).second.weight, flags, (*iter).second.middleNode));
                } else {
                    edges.push_back(OutputEdge((*iter).first, i, (*iter).second.weight, flags));
                }
            }
        }
    }
}