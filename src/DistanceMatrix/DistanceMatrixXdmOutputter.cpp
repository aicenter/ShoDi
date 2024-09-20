/******************************************************************************
* File:             DistanceMatrixXdmOutputter.cpp
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/

#include <fstream>
#include "DistanceMatrixXdmOutputter.h"
#include "../constants.h"

void DistanceMatrixXdmOutputter::store(Distance_matrix_travel_time_provider &dm, const std::string &path) {
    printf("Storing the distance matrix.\n");
    std::ofstream output;
    output.open(path + ".xdm", std::ios::binary);
    if (!output.is_open()) {
        printf("Couldn't open file '%s'!", (path + ".xdm").c_str());
    }

    const auto& distances = dm.getRawData();
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
            output.write((char *) &distances[((size_t) i) * ((size_t) nodesCnt) + ((size_t) j)], sizeof(distances[((size_t) i) * ((size_t) nodesCnt) + ((size_t) j)]));

    output.close();
}
