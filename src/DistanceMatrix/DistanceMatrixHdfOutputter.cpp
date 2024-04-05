/******************************************************************************
* File:             DistanceMatrixHdfOutputter.h
*
* Author:           Jan Horák (horakj31@fel.cvut.cz)
* Created:          03/26/24
*****************************************************************************/

#include <fstream>
#include "DistanceMatrixHdfOutputter.h"
#include "constants.h"
#include "CLI/ProgressBar.hpp"
#include <H5Cpp.h>

void DistanceMatrixHdfOutputter::store(Distance_matrix_travel_time_provider& dm, const std::string& path) {
    printf("Storing the distance matrix.\n");

    // H5::H5File* file;
    /*try {
        file = new H5::H5File(path + ".hdf5", H5F_ACC_TRUNC);
    } catch (const H5::FileIException& e) {
        printf("Couldn't open file '%s'!", (path + ".hdf5").c_str());
        return;
    }*/
    H5::H5File file(path + ".hdf5", H5F_ACC_TRUNC);

    const auto& distances = dm.getRawData();
    const auto nodesCnt = dm.nodes();
    hsize_t dimsf[2] = { nodesCnt, nodesCnt };

    auto dataspace = H5::DataSpace(2, dimsf, nullptr);
    H5::DataSet dataset(file.createDataSet("dm", H5::PredType::STD_U64LE, dataspace));
    
    auto vals = distances.get();
    dataset.write(vals, H5::PredType::NATIVE_UINT);

    dataset.close();
    dataspace.close();

    file.close();
}
