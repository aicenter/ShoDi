/******************************************************************************
* File:             DistanceMatrixHdfOutputter.h
*
* Author:           Jan Horak (horakj31@fel.cvut.cz)
* Created:          03/26/24
*****************************************************************************/

#include <fstream>
#include <filesystem>
#include "DistanceMatrixHdfOutputter.h"
#include <H5Cpp.h>

void DistanceMatrixHdfOutputter::store(Distance_matrix_travel_time_provider<dist_t>& dm, const std::string& path) {
    printf("Storing the distance matrix.\n");

    const auto& distances = dm.getRawData();
    const auto nodesCnt = dm.nodes();
    hsize_t dimsf[] = { nodesCnt, nodesCnt };
    dist_t* values = distances.get();

    dist_t max_dist = 0;
    for (size_t i = 0; i < static_cast<unsigned long long>(nodesCnt) * nodesCnt; i++) {
        max_dist = std::max(max_dist, values[i]);
    }

    H5::DataType datatype;
    unsigned int total_bytes;
    if (max_dist < UINT8_MAX) {
        datatype = static_cast<H5::DataType>(H5::PredType::STD_U8LE);
        total_bytes = nodesCnt * nodesCnt;
    }
    else if (max_dist < UINT16_MAX) {
        datatype = static_cast<H5::DataType>(H5::PredType::STD_U16LE);
        total_bytes = 2 * nodesCnt * nodesCnt;
    }
    else if (max_dist < UINT32_MAX) {
        datatype = static_cast<H5::DataType>(H5::PredType::STD_U32LE);
        total_bytes = 4 * nodesCnt * nodesCnt;
    }
    else {
        datatype = static_cast<H5::DataType>(H5::PredType::STD_U64LE);
        total_bytes = 8 * nodesCnt * nodesCnt;
    }

    std::filesystem::space_info si = std::filesystem::space(".");
    if (total_bytes > si.available) {
        throw std::runtime_error("Not enough free disk space." + std::to_string(total_bytes/1024) + "KiB required");
    }

    try {
        H5::H5File file = H5::H5File(path + ".hdf5", H5F_ACC_TRUNC);

        auto dataspace = H5::DataSpace(2, dimsf, nullptr);
        H5::DataSet dataset(file.createDataSet("dm", datatype, dataspace));
        dataset.write(values, H5::PredType::NATIVE_UINT);

        dataset.close();
        dataspace.close();
        file.close();
    } catch (H5::FileIException&) {
        printf("Couldn't open file '%s'!", (path + ".hdf5").c_str());
    }
}
