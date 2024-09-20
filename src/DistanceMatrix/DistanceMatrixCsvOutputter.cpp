/******************************************************************************
* File:             DistanceMatrixCsvOutputter.cpp
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/

#include <fstream>
#include <filesystem>
#include "DistanceMatrixCsvOutputter.h"
#include "../constants.h"
#include "../CLI/ProgressBar.hpp"

void DistanceMatrixCsvOutputter::store(Distance_matrix_travel_time_provider &dm, const std::string &path) {
    const auto& distances = dm.getRawData();
    const auto nodesCnt = dm.nodes();

    dist_t max_dist = 0;
    for (size_t i = 0; i < static_cast<unsigned long long>(nodesCnt) * nodesCnt; i++) {
        max_dist = std::max(max_dist, distances[i]);
    }
    size_t charsPerValue = std::to_string(max_dist).length();
    unsigned long long totalChars = nodesCnt * (nodesCnt*charsPerValue + nodesCnt + 2);
    std::filesystem::space_info si = std::filesystem::space(".");
    if (totalChars > si.available) {
        throw std::runtime_error("Not enough free disk space." + std::to_string(totalChars/1024) + "KiB required");
    }

    printf("Storing the distance matrix.\n");
    std::ofstream output;
    output.open(path + ".csv");
    if (!output.is_open()) {
        printf("Couldn't open file '%s'!", (path + ".csv").c_str());
    }

    ProgressBar progress(nodesCnt);
    auto size = static_cast<size_t>(nodesCnt) * nodesCnt;
    for(size_t i = 0; i < size; i++) {
        output << distances[i];

        if ((i + 1) % nodesCnt == 0) {
            output << std::endl;
            ++progress;
        } else {
            output << ",";
        }
    }
}
