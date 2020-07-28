//
// Created by Jan Neumann (neumaja5@fel.cvut.cz) on 28.07.20.
//

#include <fstream>
#include "DistanceMatrixCsvOutputter.h"

void DistanceMatrixCsvOutputter::store(DistanceMatrix &dm, const string &path) {
    printf("Storing the distance matrix.\n");
    ofstream output;
    output.open(path + ".csv");
    if (!output.is_open()) {
        printf("Couldn't open file '%s'!", (path + ".xdm").c_str());
    }

    const vector<int> &distances = dm.getRawData();
    const int nodesCnt = dm.nodes();
    for (size_t i = 0; i < distances.size(); i++) {
        output << distances[i];

        if ((i + 1) % nodesCnt == 0) {
            output << endl;
        } else {
            output << ",";
        }
    }
}
