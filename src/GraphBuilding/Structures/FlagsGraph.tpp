//
// Author: Xenty (Michal Cvach)
// Created on: 28.8.18
//

#include <climits>
#include <boost/numeric/conversion/cast.hpp>

//______________________________________________________________________________________________________________________
template<class T> FlagsGraph<T>::FlagsGraph(unsigned int n) {
    neighbours.resize(n);
    nodesData.resize(n);
}

//______________________________________________________________________________________________________________________
template<class T> FlagsGraph<T>::FlagsGraph(UpdateableGraph & g) {
    auto nodesCnt = g.nodes();
    neighbours.resize(nodesCnt);
    nodesData.resize(nodesCnt);

    for(unsigned int i = 0; i < nodesCnt; i++) {
        data(i).rank = g.getRank(i);
    }

    std::vector < OutputEdge > edges;
    std::vector < OutputShortcutEdge > shortcuts;
    g.prepareEdgesForFlushingWithReinsert(edges, shortcuts);

    processOriginalEdges(edges);
    processShortcuts(shortcuts);
}

//______________________________________________________________________________________________________________________
template<class T> FlagsGraph<T>::~FlagsGraph() = default;

//______________________________________________________________________________________________________________________
template<class T> void FlagsGraph<T>::getEdgesForFlushing(std::vector < std::pair< unsigned int, QueryEdge > > & allEdges) {
    const unsigned int nbs = boost::numeric_cast<unsigned int>(neighbours.size());
    for(unsigned int i = 0; i < nbs; i++) {
        const unsigned int nbs2 = boost::numeric_cast<unsigned int>(neighbours[i].size());
        for(unsigned int j = 0; j < nbs2; j++) {
            allEdges.push_back(std::make_pair(i, QueryEdge(neighbours[i][j])));
        }
    }
}

//______________________________________________________________________________________________________________________
template<class T> void FlagsGraph<T>::processOriginalEdges(std::vector < OutputEdge > & edges) {
    for(size_t i = 0; i < edges.size(); i++) {
        unsigned int from, to, weight, flags;
        from = edges[i].sourceNode;
        to = edges[i].targetNode;
        weight = edges[i].weight;
        flags = edges[i].flags;

        bool forward = false;
        bool backward = false;
        if((flags & 1) == 1) {
            forward = true;
        }
        if((flags & 2) == 2) {
            backward = true;
        }
        if ( data(from).rank < data(to).rank ) {
            addEdge(from, to, weight, forward, backward);
        } else {
            addEdge(to, from, weight, forward, backward);
        }
    }
}

//______________________________________________________________________________________________________________________
template<class T> void FlagsGraph<T>::processShortcuts( std::vector < OutputShortcutEdge > & shortcuts) {
    for(size_t i = 0; i < shortcuts.size(); i++) {
        unsigned int from, to, weight, flags;
        from = shortcuts[i].sourceNode;
        to = shortcuts[i].targetNode;
        weight = shortcuts[i].weight;
        flags = shortcuts[i].flags;

        bool forward = false;
        bool backward = false;
        if((flags & 1) == 1) {
            forward = true;
        }
        if((flags & 2) == 2) {
            backward = true;
        }
        if ( data(from).rank < data(to).rank ) {
            addEdge(from, to, weight, forward, backward);
        } else {
            addEdge(to, from, weight, forward, backward);
        }
    }
}

//______________________________________________________________________________________________________________________
template<class T> void FlagsGraph<T>::addEdge(unsigned int from, unsigned int to, unsigned int weight, bool fw, bool bw) {
    neighbours.at(from).push_back(QueryEdge(to, weight, fw, bw));
}

//______________________________________________________________________________________________________________________
template<class T> unsigned int FlagsGraph<T>::nodes() const {
    return boost::numeric_cast<unsigned int>(neighbours.size());
}

//______________________________________________________________________________________________________________________
template<class T> const std::vector< QueryEdge > & FlagsGraph<T>::nextNodes(const unsigned int x)const {
    return neighbours.at(x);
}

//______________________________________________________________________________________________________________________
template<class T> T& FlagsGraph<T>::data(unsigned int node) {
    return nodesData[node];
}

//______________________________________________________________________________________________________________________
template<class T> void FlagsGraph<T>::resetForwardInfo(const unsigned int node) {
    nodesData[node].forwardDist = UINT_MAX;
    nodesData[node].forwardSettled = false;
    nodesData[node].forwardReached = false;

}

//______________________________________________________________________________________________________________________
template<class T> void FlagsGraph<T>::resetBackwardInfo(const unsigned int node) {
    nodesData[node].backwardDist = UINT_MAX;
    nodesData[node].backwardSettled = false;
    nodesData[node].backwardReached = false;
}

//______________________________________________________________________________________________________________________
template<class T> void FlagsGraph<T>::resetForwardStall(const unsigned int node) {
    nodesData[node].forwardStalled = false;
}

//______________________________________________________________________________________________________________________
template<class T> void FlagsGraph<T>::resetBackwardStall(const unsigned int node) {
    nodesData[node].backwardStalled = false;
}

template<class T> const std::vector<std::vector<QueryEdge>>& FlagsGraph<T>::getNeighbours() const {
    return neighbours;
}

template<class T> const std::vector<T> &FlagsGraph<T>::getNodesData() const {
    return nodesData;
}
