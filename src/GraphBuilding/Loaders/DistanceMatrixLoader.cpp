//
// Author: Xenty (Michal Cvach)
// Created on: 05.10.19
//

#include <fstream>
#include "DistanceMatrixLoader.h"
#include "../../Timer/Timer.h"
#include <H5Cpp.h>
#include <boost/numeric/conversion/cast.hpp>

//______________________________________________________________________________________________________________________
DistanceMatrixLoader::DistanceMatrixLoader(std::string inputFile) {
    this->inputFile = inputFile;
}

//______________________________________________________________________________________________________________________
Distance_matrix_travel_time_provider<dist_t>* DistanceMatrixLoader::loadXDM() {
    std::ifstream input;
    input.open(this->inputFile, std::ios::binary);
    if( ! input.is_open() ) {
        printf("Couldn't open file '%s'!", this->inputFile.c_str());
    }

    unsigned int nodes;
    parseHeader(input, nodes);

    Distance_matrix_travel_time_provider<dist_t>* distanceMatrix = new Distance_matrix_travel_time_provider<dist_t>(nodes);

    parseDistances(input, nodes, *distanceMatrix);

    input.close();

    return distanceMatrix;
}

//______________________________________________________________________________________________________________________
Distance_matrix_travel_time_provider<dist_t>* DistanceMatrixLoader::loadHDF() {
    H5::H5File file{this->inputFile, H5F_ACC_RDONLY};
    H5::DataSet dataset = file.openDataSet("dm");
    auto t = dataset.getDataType();
    H5::DataSpace space = dataset.getSpace();

    hsize_t dimsf[2];
    space.getSimpleExtentDims(dimsf, nullptr);
    const auto nodes = dimsf[0];

    auto values = new int[nodes*nodes];
    dataset.read(values, H5::PredType::NATIVE_INT);

    auto* distanceMatrix = new Distance_matrix_travel_time_provider<dist_t>(boost::numeric_cast<unsigned int>(nodes));

    for(size_t i = 0; i < nodes; i++) {
        for (size_t j = 0; j < nodes; j++) {
            //std::cout << "distance from " << i << " to " << j << " is " << values[i*nodes + j] << std::endl;
            distanceMatrix->setDistance(
                boost::numeric_cast<unsigned int>(i),
                boost::numeric_cast<unsigned int>(j),
                values[i*nodes + j]);
        }
    }

    delete [] values;
    return distanceMatrix;
}

//______________________________________________________________________________________________________________________
void DistanceMatrixLoader::parseHeader(std::ifstream & input, unsigned int & nodes) {
    char c1, c2, c3;
    input.read(&c1, sizeof(c1));
    input.read(&c2, sizeof(c2));
    input.read(&c3, sizeof(c3));
    if (c1 != 'X' || c2 != 'D' || c3 != 'M') {
        printf("The input file is missing the Distance Matrix header.\n");
        printf("Are you sure the input file is in the correct format?\n");
        printf("The loading will proceed but the loaded distance matrix might be corrupted.\n");
    }

    input.read((char *) &nodes, sizeof(nodes));
}

//______________________________________________________________________________________________________________________
void DistanceMatrixLoader::parseDistances(std::ifstream & input, const unsigned int nodes, Distance_matrix_travel_time_provider<dist_t>& distanceMatrix) {
    unsigned int distance;
    for(unsigned int i = 0; i < nodes; i++) {
        for(unsigned int j = 0; j < nodes; j++) {
            input.read((char *) &distance, sizeof(distance));
            distanceMatrix.setDistance(i, j, distance);
        }
    }
}