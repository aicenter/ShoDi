//
// Created by Jan Neumann (neumaja5@fel.cvut.cz) on 28.07.20.
//

#include <fstream>
#include "DistanceMatrixXdmOutputter.h"

void DistanceMatrixXdmOutputter::store(DistanceMatrix &dm, const string &path) {
    printf("Storing the distance matrix.\n");
    ofstream output;
    output.open(path + ".xdm", ios::binary);
    if (!output.is_open()) {
        printf("Couldn't open file '%s'!", (path + ".xdm").c_str());
    }

    const vector<int> &distances = dm.getRawData();
    const unsigned int nodesCnt = dm.nodes();

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
