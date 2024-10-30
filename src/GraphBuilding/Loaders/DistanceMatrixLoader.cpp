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

template<class IntType>
void fillMatrix(DistanceMatrixInterface* dm, IntType* values, size_t nodes) {
    for(size_t i = 0; i < nodes; i++) {
        for (size_t j = 0; j < nodes; j++) {
            dm->setDistance(
                boost::numeric_cast<unsigned int>(i),
                boost::numeric_cast<unsigned int>(j),
                values[i*nodes + j]);
        }
    }
}

//______________________________________________________________________________________________________________________
DistanceMatrixInterface* DistanceMatrixLoader::loadHDF() {
    H5::H5File file{this->inputFile, H5F_ACC_RDONLY};
    H5::DataSet dataset = file.openDataSet("dm");
    H5::IntType intType = dataset.getIntType();
    auto size = intType.getSize();
    H5::DataSpace space = dataset.getSpace();

    hsize_t dimsf[2];
    space.getSimpleExtentDims(dimsf, nullptr);
    const auto nodes = dimsf[0];

    DistanceMatrixInterface* dm;
    if (size <= 2) {
        auto values = new uint_least16_t[nodes*nodes];
        dm = new Distance_matrix_travel_time_provider<uint_least16_t>(boost::numeric_cast<unsigned int>(nodes));
        dataset.read(values, H5::PredType::NATIVE_UINT_LEAST16);
        fillMatrix(dm, values, nodes);
        delete [] values;
    } else if (size <= 4) {
        auto values = new uint_least32_t[nodes*nodes];
        dm = new Distance_matrix_travel_time_provider<uint_least32_t>(boost::numeric_cast<unsigned int>(nodes));
        dataset.read(values, H5::PredType::NATIVE_UINT_LEAST32);
        fillMatrix(dm, values, nodes);
        delete [] values;
    } else {
        auto values = new dist_t[nodes*nodes];
        dm = new Distance_matrix_travel_time_provider<dist_t>(boost::numeric_cast<unsigned int>(nodes));
        dataset.read(values, H5::PredType::NATIVE_UINT);
        fillMatrix(dm, values, nodes);
        delete [] values;
    }

    return dm;
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