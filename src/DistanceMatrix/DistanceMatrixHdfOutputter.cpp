/******************************************************************************
* File:             DistanceMatrixHdfOutputter.h
*
* Author:           Jan Horak (horakj31@fel.cvut.cz)
* Created:          03/26/24
*****************************************************************************/

#include <fstream>
#include "DistanceMatrixHdfOutputter.h"
#include <H5Cpp.h>

void DistanceMatrixHdfOutputter::store(Distance_matrix_travel_time_provider& dm, const std::string& path) {
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
    if (max_dist < UINT8_MAX) {
        datatype = H5::PredType::STD_U8LE;
    }
    else if (max_dist < UINT16_MAX) {
        datatype = H5::PredType::STD_U16LE;
    }
    else if (max_dist < UINT32_MAX) {
        datatype = H5::PredType::STD_U32LE;
    }
    else {
        datatype = H5::PredType::STD_U64LE;
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
