//
// Author: Xenty (Michal Cvach)
// Created on: 22.8.18
//

#include <fstream>
#include "ShrinkingGraph.h"

//______________________________________________________________________________________________________________________
ShrinkingGraph::ShrinkingGraph(unsigned int n) : UpdateableGraph(n) {

}

//______________________________________________________________________________________________________________________
void ShrinkingGraph::flushGraph(string filePath) {
    long long unsigned int records = 0;
    for (unsigned int i = 0; i < followingNodes.size(); i++) {
        records += followingNodes[i].size();
    }
    for (unsigned int i = 0; i < previousNodes.size(); i++) {
        records += previousNodes[i].size();
    }

    ofstream output;
    output.open(filePath + "_ch_graph");
    if( ! output.is_open() ) {
        printf("Couldn't open file '%s'!", (filePath + "_ch_graph").c_str());
    }

    output << "c This file was created by the preprocessing process of Contraction Hiearchies." << endl;
    output << "c This graph is only usable in combination with the _ranks file and will only work" << endl;
    output << "c with the Contraction Hierarchies. Running Dijkstra on this Graph isn't guaranteed" << endl;
    output << "c to provide correct results." << endl;
    output << "p sp " << followingNodes.size() << " " << records << endl;
    for (unsigned int i = 0; i < followingNodes.size(); i++) {
        for (auto iter = followingNodes.at(i).begin(); iter != followingNodes.at(i).end(); ++iter) {
            output << "b " << i+1 << " " << (*iter).first + 1 << " " << (*iter).second << endl;
        }
    }
    for (unsigned int i = 0; i < previousNodes.size(); i++) {
        for (auto iter = previousNodes.at(i).begin(); iter != previousNodes.at(i).end(); ++iter) {
            output << "c " << i+1 << " " << (*iter).first + 1 << " " << (*iter).second << endl;
        }
    }

    output.close();
}

//______________________________________________________________________________________________________________________
void ShrinkingGraph::removeUnnecesarryEdges(vector<unsigned int> & ranks) {
    removeFollowing(ranks);
    removePrevious(ranks);
}

//______________________________________________________________________________________________________________________
void ShrinkingGraph::removeFollowing(vector<unsigned int> & ranks) {
    vector < pair < unsigned int, unsigned int > > toBeRemoved;

    for(unsigned int i = 0; i < followingNodes.size(); i++) {
        for(auto iter = followingNodes.at(i).begin(); iter != followingNodes.at(i).end(); ++iter) {
            if(ranks[i] > ranks[(*iter).first]) {
                toBeRemoved.push_back(make_pair(i, (*iter).first));
            }
        }
    }

    for(long long unsigned int i = 0; i < toBeRemoved.size(); i++) {
        followingNodes.at(toBeRemoved[i].first).erase(toBeRemoved[i].second);
    }
}

//______________________________________________________________________________________________________________________
void ShrinkingGraph::removePrevious(vector<unsigned int> & ranks) {
    vector < pair < unsigned int, unsigned int > > toBeRemoved;

    for(unsigned int i = 0; i < previousNodes.size(); i++) {
        for(auto iter = previousNodes.at(i).begin(); iter != previousNodes.at(i).end(); ++iter) {
            if(ranks[i] > ranks[(*iter).first]) {
                toBeRemoved.push_back(make_pair(i, (*iter).first));
            }
        }
    }

    for(long long unsigned int i = 0; i < toBeRemoved.size(); i++) {
        previousNodes.at(toBeRemoved[i].first).erase(toBeRemoved[i].second);
    }
}

