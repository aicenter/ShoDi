/******************************************************************************
* File:             DistanceMatrixXdmOutputter.cpp
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/

#include <fstream>
#include <filesystem>
#include "DistanceMatrixXdmOutputter.h"
#include "../constants.h"

void DistanceMatrixXdmOutputter::store(Distance_matrix_travel_time_provider<dist_t>& dm, const std::string &path) {
    const unsigned int nodesCnt = dm.nodes();
    const auto& distances = dm.getRawData();

    std::filesystem::space_info si = std::filesystem::space(".");
    unsigned long long total_bytes = nodesCnt * nodesCnt * sizeof(dist_t);
    if (total_bytes > si.available) {
        throw std::runtime_error("Not enough free disk space." + std::to_string(total_bytes/1024) + "KiB required");
    }

    printf("Storing the distance matrix.\n");
    std::ofstream output;
    output.open(path + ".xdm", std::ios::binary);
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
            output.write((char *) &distances[((size_t) i) * ((size_t) nodesCnt) + ((size_t) j)], sizeof(distances[((size_t) i) * ((size_t) nodesCnt) + ((size_t) j)]));

    output.close();
}
