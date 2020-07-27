//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include <cstdio>
#include <climits>
#include <fstream>
#include <cmath>
#include "DistanceMatrix.h"

//______________________________________________________________________________________________________________________
DistanceMatrix::DistanceMatrix(const unsigned int nodes) : nodesCnt(nodes), distances(nodes) {

}

//______________________________________________________________________________________________________________________
DistanceMatrix::DistanceMatrix(vector<int> &&distMatrix) : nodesCnt(sqrt(distMatrix.size())),
                                                           distances(move(distMatrix)) {

}

//______________________________________________________________________________________________________________________
unsigned int DistanceMatrix::findDistance(const unsigned int start, const unsigned int goal) {
    return distances[start * nodesCnt + goal];
}

//______________________________________________________________________________________________________________________
void DistanceMatrix::setDistance(unsigned int source, unsigned int target, unsigned int distance) {
    distances[source * nodesCnt + target] = distance;
}

//______________________________________________________________________________________________________________________
void DistanceMatrix::outputToXdm(const string &path) {
    printf("Outputting the distance matrix.\n");
    ofstream output;
    output.open(path + ".xdm", ios::binary);
    if (!output.is_open()) {
        printf("Couldn't open file '%s'!", (path + ".xdm").c_str());
    }

    char c1, c2, c3;
    c1 = 'X';
    c2 = 'D';
    c3 = 'M';
    output.write(&c1, sizeof(c1));
    output.write(&c2, sizeof(c2));
    output.write(&c3, sizeof(c3));
    output.write((char *) &nodesCnt, sizeof(nodesCnt));

    for (unsigned int i = 0; i < nodesCnt; ++i)
        for (unsigned int j = 0; j < nodesCnt; ++j)
            output.write((char *) &distances[i * nodesCnt + j], sizeof(distances[i * nodesCnt + j]));

    output.close();
}

//______________________________________________________________________________________________________________________
void DistanceMatrix::outputToCsv(const string &path) {
    printf("Outputting the distance matrix.\n");
    ofstream output;
    output.open(path + ".csv");
    if (!output.is_open()) {
        printf("Couldn't open file '%s'!", (path + ".xdm").c_str());
    }

    for (size_t i = 0; i < distances.size(); i++) {
        output << distances[i];

        if ((i + 1) % nodesCnt == 0) {
            output << endl;
        } else {
            output << ",";
        }
    }
}

//______________________________________________________________________________________________________________________
void DistanceMatrix::printInfo() {
    int half = INT_MAX / 2;
    unsigned int halfCnt = 0;
    unsigned int maxCnt = 0;
    for (unsigned int i = 0; i < nodesCnt; ++i) {
        for (unsigned int j = 0; j < nodesCnt; ++j) {
            if (distances[i * nodesCnt + j] == INT_MAX) {
                maxCnt++;
            }
            if (distances[i * nodesCnt + j] >= half) {
                halfCnt++;
            }
        }
    }

    const unsigned int optCount = distances.size();
    printf("Computed distance matrix info.\n");
    printf("Distance matrix contains %u INF values. That is %f %%.\n", maxCnt, (double) maxCnt / optCount);
    printf("Distance matrix contains %u values that are at least half of INT_MAX. That is %f %%.\n", halfCnt,
           (double) halfCnt / optCount);
}
