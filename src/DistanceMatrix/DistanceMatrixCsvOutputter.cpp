/******************************************************************************
* File:             DistanceMatrixCsvOutputter.cpp
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/

#include <fstream>
#include "DistanceMatrixCsvOutputter.h"
#include "constants.h"

void DistanceMatrixCsvOutputter::store(DistanceMatrix &dm, const string &path) {
    printf("Storing the distance matrix.\n");
    ofstream output;
    output.open(path + ".csv");
    if (!output.is_open()) {
        printf("Couldn't open file '%s'!", (path + ".xdm").c_str());
    }

    const vector<dist_t> &distances = dm.getRawData();
    const auto nodesCnt = dm.nodes();
    for (unsigned int i = 0; i < distances.size(); i++) {
        output << distances[i];

        if ((i + 1) % nodesCnt == 0) {
            output << endl;
        } else {
            output << ",";
        }
    }
}
