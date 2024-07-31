/******************************************************************************
* File:             DistanceMatrixCsvOutputter.cpp
*
* Author:           Jan Neumann (neumaja5@fel.cvut.cz)  
* Created:          07/28/20 
*****************************************************************************/

#include <fstream>
#include "DistanceMatrixCsvOutputter.h"
#include "../constants.h"
#include "../CLI/ProgressBar.hpp"

void DistanceMatrixCsvOutputter::store(Distance_matrix_travel_time_provider &dm, const std::string &path) {
    printf("Storing the distance matrix.\n");
    std::ofstream output;
    output.open(path + ".csv");
    if (!output.is_open()) {
        printf("Couldn't open file '%s'!", (path + ".csv").c_str());
    }

    const auto& distances = dm.getRawData();
    const auto nodesCnt = dm.nodes();

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
